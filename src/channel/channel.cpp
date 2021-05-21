#include "../frame/task.h"
#include "channel.h"

Channel<int> ch;

std::task<> snd()
{
	while(1){
        std::cout << "sned:11" << std::endl;
        co_await ch.write(11);
        std::cout << "sned:12" << std::endl;
        co_await ch.write(12);
        std::cout << "sned:13" << std::endl;
        co_await ch.write(13);
    }
}

std::task<> rcv()
{
	while(1){
		std::cout << "rcv:" << co_await ch.read() << std::endl;
	}
}

int main()
{
    auto s = snd();
    s.resume();

    auto r = rcv();
    r.resume();
    while (1)
    {
        s.resume();
    }    
}