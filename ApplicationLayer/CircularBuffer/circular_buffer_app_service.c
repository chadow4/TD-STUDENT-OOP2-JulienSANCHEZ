#include <stdio.h>
#include <stdlib.h>
#include "circular_buffer_app_service.h"
#include "i_circular_buffer_repository.h"
#include "i_circular_buffer.h"

#define FILE_DB_REPO "../Persistence/FileDB/CircularBuffer/CIRCULAR_BUFFER"

static circular_buffer cb;

static void generate_next_int()
{
    printf("\n[%d]: ", generator_get_next());
}

static void create(int length)
{
    cb = CircularBuffer_construct(length);
}

int CircularBufferAppService_run_use_case(int c)
{
    if (cb == NULL)
        generate_next_int();

    switch (c)
    {
    case GO_TO_HEAD:
        CircularBuffer_set_current_to_head(cb);
        break;

    case DISPLAY_PRECEDENT:
        printf("(%c)", CircularBuffer_get_char_before_current(cb));
        break;

    case DESTROY_CIRCULAR_BUFFER:
        CircularBuffer_collect(cb);
        break;

    case NEW_CIRCULAR_BUFFER:
        create(CIRCULAR_BUFFER_LENGTH);
        break;

    case SAVE_CIRCULAR_BUFFER:
        ICircularBufferRepository_save(cb);
        ICircularBufferRepository_close();
        break;

    case RESTORE_CIRCULAR_BUFFER: {
        char number[3];

        /* Input string representation of integer from user. */
        fgets(number, 3, stdin);

        /* Convert string representation of number to integer */
        int rank = (int)strtol(number, NULL, 10);

        if (!ICircularBufferRepository_open(FILE_DB_REPO))
            return EXIT_FAILURE;

        cb = ICircularBufferRepository_get_nth_cb(rank);
        break;
    }

    case '\n':
        generate_next_int();
    case ' ':
    case '\t':
        break;

    default:
        CircularBuffer_append_char_at_head(cb, c);
        break;
    }
    return 0;
}
