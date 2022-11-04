#ifndef CB_INFRA_FILE_SERVICE_H
#define CB_INFRA_FILE_SERVICE_H

#include "circular_buffer.h"

extern int ICircularBufferRepository_save(circular_buffer cb);
extern int ICircularBufferRepository_open(char *name);
extern void ICircularBufferRepository_append(circular_buffer cb);
extern void ICircularBufferRepository_close(void);
extern circular_buffer ICircularBufferRepository_get_nth_buffer(int rank);

#endif