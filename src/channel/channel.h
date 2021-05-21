#pragma once

#include <coroutine>
#include <list>
// #include <thread>

template<typename Type>
class Channel
{
public:
	Channel()
	{
		receiver = 0;
	}

	Channel(std::coroutine_handle<> h)
	{
		receiver = h;
	}

	void consumer(std::coroutine_handle<> h)
	{
		receiver = h;
	}

	class channel_reader
	{
	public:
		channel_reader(Channel &ch):ch(ch){}
		bool await_ready() const noexcept(false)
		{
			return false;
		}
		void await_suspend(std::coroutine_handle<> h) noexcept(false)
		{
			if (!ch.receiver)
				ch.receiver = h;
			if (!ch._list.empty())
				h.resume();
		}
		Type await_resume() noexcept(false)
		{
			Type obj = std::move(ch._list.front());
			ch._list.pop_front();
			return std::move(obj);
		}
		Channel &ch;
	};

	class channel_writer
	{
	public:
		channel_writer(const Type &obj,Channel &ch):obj_(obj),ch(ch){}
		bool await_ready() const noexcept(false)
		{
			return false;
		}
		void await_suspend(std::coroutine_handle<> h) noexcept(false)
		{
			ch._list.push_back(obj_);
			if (ch.receiver && ch.receiver != h)
				ch.receiver.resume();
		}
		void await_resume() noexcept(false)
		{
		}
		const Type &obj_;
		Channel &ch;
	};

	channel_reader read()
	{
		return channel_reader(*this);
	}

	channel_writer write(const Type &obj)
    {
		return channel_writer(obj, *this);
    }

	// inline bool write(Type &&obj)
    // {
        // _list.push_back(std::move(obj));
    //     if (receiver && receiver != current())
	// 		resume(receiver);
    // }

    inline void clear()
    {
    	_list.clear();
    }

	inline void touch()
    {
    //     if (receiver && receiver != current())
	// 		resume(receiver);_list
    }
	
	inline size_t size()
	{
		return _list.size();
	}

	inline bool empty()
	{
		return _list.empty();
	}

public:
	std::list<Type> _list;
	std::coroutine_handle<> receiver;
};