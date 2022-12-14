#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "circular_buffer_app_service.h"

//static circular_buffer cb;

static void prompt()
{
    puts("************************************");
    puts("* Instance Circular Buffer created *");
    puts("************************************");
    puts("*************************************************************************");
    puts("At any time you could input a string of char then Enter, e.g, [1]: 1234 ");
    puts("*************************************************************************");
    puts("***********************************************************************************************************************************************");
    puts("* Menu [ n: new instance, > : current:=current, < : read char before, ! : save, x : destroy instance,  r n: read the nth record, Ctr-D: Quit] *");
    puts("***********************************************************************************************************************************************");
}

int main()
{
    prompt();
    CircularBufferAppService_run_use_case('n'); //create the first instance for free -cadeau!

    while (true)
    {

        int c = getchar();
        if (c == EOF)
            break;

        CircularBufferAppService_run_use_case(c);
    }

    printf("\n");
    return EXIT_SUCCESS;
}