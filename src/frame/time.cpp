#include "time.h"

int delTimer(timerNode *tn)
{
    //终止协程运行？
    tn->pre->next = tn->next?tn->next:nullptr;
    tn->next->pre = tn->pre?tn->pre:nullptr;
    delete tn;
}