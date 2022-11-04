#ifndef CB_APP_SERVICE_H
#define CB_APP_SERVICE_H
#include "generator.h"

#define DESTROY_CIRCULAR_BUFFER 'x'
#define GO_TO_HEAD '>'
#define DISPLAY_PRECEDENT '<'
#define NEW_CIRCULAR_BUFFER 'n'
#define SAVE_CIRCULAR_BUFFER '!'
#define RESTORE_CIRCULAR_BUFFER 'r'
#define CIRCULAR_BUFFER_LENGTH 6

extern int CircularBufferAppService_run_use_case(int c);

#endif