#include "time.h"

timerNode *timers;

timerNode *addTimer(int duration, callbBack cb)//, ClientModel *cm)
{
    timerNode *tn = new timerNode;
    tn->time = clock() + duration;
    if(timers)
    {
        timerNode *ctn = timers;
        while(tn->time > ctn->time)
        {
            ctn = ctn->next;
        }
            
    }else{
        timers = tn;
    }
    auto h = cb(tn);//, cm);
    tn->h = h.handle_;
    // h.resume();
}

int delTimer(timerNode *tn)
{
    //终止协程运行？
    tn->pre->next = tn->next?tn->next:nullptr;
    tn->next->pre = tn->pre?tn->pre:nullptr;
    delete tn;
}