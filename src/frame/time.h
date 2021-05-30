#pragma once

#include <coroutine>
#include <time.h>
#include "task.h"
// #include "../client/model.h"

struct timerNode
{
    timerNode *pre;
    timerNode *next;
    clock_t time;
    std::coroutine_handle<> h;
};

using callbBack = std::task<> (*)(timerNode *tn);

timerNode *addTimer(int duration, callbBack cb);//, ClientModel *cm);
int delTimer(timerNode *tn);