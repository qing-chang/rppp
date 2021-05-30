#pragma once

#include <coroutine>
#include <time.h>
#include "task.h"

struct timerNode
{
    timerNode *pre;
    timerNode *next;
    clock_t time;
    std::coroutine_handle<> h;
};

using callbBack = std::task<> (*)(timerNode *tn);

timerNode *addTimer(int duration, callbBack cb);
int delTimer(timerNode *tn);