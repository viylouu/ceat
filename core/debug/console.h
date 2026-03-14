#pragma once

#include "../eaw/input.h"

typedef struct eat_console_desc{
    bool enabled;
    eaw_key key;
    void (*command_solver)(char* command);
} eat_console_desc;

extern eat_console_desc console;

void
eat_console_init(
    void
    );

void
eat_console_stop(
    void
    );

void
eat_console_try_do(
    void
    );
