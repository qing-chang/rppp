#pragma once

#include <coroutine>
#include <queue>
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
			if (!ch._queue.empty())
				h.resume();
			if (ch.sender && ch.sender != h)
				ch.sender.resume();
		}
		Type await_resume() noexcept(false)
		{
			const Type& obj = std::move(ch._queue.front());
			ch._queue.pop();
			return obj;
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
			ch._queue.push(obj_);
			if (!ch.sender)
				ch.sender = h;
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
        // _queue.push_back(std::move(obj));
    //     if (receiver && receiver != current())
	// 		resume(receiver);
    // }

    inline void clear()
    {
    	_queue.clear();
    }

	inline void touch()
    {
    //     if (receiver && receiver != current())
	// 		resume(receiver);_queue
    }
	
	inline size_t size()
	{
		return _queue.size();
	}

	inline bool empty()
	{
		return _queue.empty();
	}

public:
	std::queue<Type> _queue;
	std::coroutine_handle<> sender, receiver;
};