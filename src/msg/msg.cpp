#include "msg.h"

int Msg::checkPack()
{
    int r = 0;
    for(;;)
    // {
    //     if( =='{')
    //         r++;
    //     else if( == '}')
    //     {
    //         if(r != 0)
    //             r--;
    //     }
    // }
    return r;
}

void Msg::readMsg()
{
    //json
    if(begin == 0 && checked == end)
    {
        checked = end = 0;
    }
    else
    {
        begin = checked;
    }
}