#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "i_circular_buffer_repository.h"
#include "i_circular_buffer.h"

#define INDEX_SUFFIX ".ndx"
#define DATA_SUFFIX ".rec"
#define FILE_DB_REPO "../Persistence/FileDB/CircularBuffer/CIRCULAR_BUFFER"

struct RECORD_CB
{
    unsigned long data_length;
    int offset_head;
    int offset_current;
    char data[]; //new in c99 !!
};

struct circular_buffer
{
    char *tail;
    unsigned long length;
    char *head;
    char *current;
    bool isFull;
};

struct index
{
    long recordStart;
    size_t recordLength;
};

static FILE *index_stream;
static FILE *data_stream;

int ICircularBufferRepository_save(circular_buffer cb)
{
    if (!ICircularBufferRepository_open(FILE_DB_REPO)){
        return 0;
    }
    
    ICircularBufferRepository_append(cb);

    return 1;
}

void ICircularBufferRepository_close(void)
{
    fclose(index_stream);
    fclose(data_stream);
}

static FILE *auxiliary_open(char *prefix, char *suffix)
{
    int prefixLength = (int)strlen(prefix);
    int suffixLength = (int)strlen(suffix);
    char name[prefixLength + suffixLength + 1];
    strncpy(name, prefix, prefixLength);
    strncpy(name + prefixLength, suffix, suffixLength + 1);
    FILE *stream = fopen(name, "r+");
    if (stream == NULL)
        stream = fopen(name, "w+");
    if (stream == NULL)
        perror(name);
    return stream;
}

int ICircularBufferRepository_open(char *name)
{
    data_stream = auxiliary_open(name, DATA_SUFFIX);
    if (data_stream == NULL){
        return 0;
    }

    index_stream = auxiliary_open(name, INDEX_SUFFIX);
    if (index_stream == NULL) {
        fclose(data_stream);
        return 0;
    }

    return 1;
}

int ICircularBufferRepository_append(circular_buffer cb)
{
    struct index index;
    struct RECORD_CB recordCB;
    recordCB.data_length = cb->length;
    recordCB.offset_current = (cb->current - cb->tail) / sizeof(char);
    recordCB.offset_head = cb->isFull ? cb->length : (cb->head - cb->tail) / sizeof(char);

    int i = 0;
    while (i < recordCB.offset_head) {
        recordCB.data[i] = cb->tail[i];
        i++;
    }

    fseek(data_stream, 0L, SEEK_END);

    size_t length = sizeof(recordCB) + sizeof(char) * recordCB.offset_head;
    index.recordStart = ftell(data_stream);
    index.recordLength = length;

    fwrite(&recordCB, length, 1, data_stream);
    fseek(index_stream, 0L, SEEK_END);
    fwrite(&index, sizeof(index), 1, index_stream);

    return 1;
}

circular_buffer ICircularBufferRepository_get_nth_cb(int rank) {
    struct index index;
    long shift = (rank - 1) * sizeof(index);
    fseek(index_stream, shift, SEEK_SET);
    fread(&index, sizeof(index), 1, index_stream);
    fseek(data_stream, index.recordStart, SEEK_SET);

    struct RECORD_CB recordCB;
    fread(&recordCB.data_length, sizeof(unsigned long), 1, data_stream);
    fread(&recordCB.offset_head, sizeof(int), 1, data_stream);
    fread(&recordCB.offset_current, sizeof(int), 1, data_stream);
    fread(&recordCB.data, sizeof(char) * recordCB.offset_head, 1, data_stream);

    circular_buffer cb = CircularBuffer_construct(recordCB.data_length);

    int i = 0;
    while (i < recordCB.offset_head) {
        CircularBuffer_append_char_at_head(cb, recordCB.data[i]);
        i++;
    }

    cb->current = cb->tail + recordCB.offset_current;

    return cb;
}
