#ifndef CONDVARQUEUE_HPP_
#define CONDVARQUEUE_HPP_

#include <iostream>
#include <array>
#include <mutex>
#include <condition_variable>

template<class T, std::size_t SIZE>
class condvarQueue
{
public:
	condvarQueue() : queueSize( SIZE ), writeIdx(0), readIdx(0), full(false)
	{}

	bool push( T&& val )
	{
		std::lock_guard<std::mutex> lock(m);
		if( full )
		{
			return false; 
		}
		queue[writeIdx] = val; 
		++writeIdx %= queueSize;

		if( writeIdx == readIdx )
		{
			full = true; 
		}
		cv.notify_one(); 
		return true;
	}

	void waitTopPop( T& val )
	{
		std::unique_lock<std::mutex> lock(m); 
		cv.wait( lock, [this](){ return !isEmpty(); }); 
		val = queue[readIdx];
		full = false; 
		++readIdx %= queueSize; 
	}

	bool pop()
	{
		std::lock_guard<std::mutex> lock(m);
		if( isEmpty() )
		{
			return false; 
		}
		full = false; 
		++readIdx %= queueSize; 

		return true; 
	}

	bool top( T& val )
	{
		std::lock_guard<std::mutex> lock(m);
		if( isEmpty() )
			return false; 
		val = queue[readIdx];
		return true; 
	}

	bool isEmpty() const 
	{
		return !full && writeIdx == readIdx;
	}

	std::size_t size() const
	{
		return queueSize; 
	}

private:
	std::array<T, SIZE> queue;
	std::size_t queueSize;
	std::mutex m;
	std::uint64_t writeIdx;
	std::uint64_t readIdx;
	std::condition_variable cv; 
	bool full;
};

#endif 
