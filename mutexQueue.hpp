
#ifndef MUTEXQUEUE_HPP_
#define MUTEXQUEUE_HPP_


#include <iostream>
#include <array>
#include <mutex>

template<class T, std::size_t SIZE>
class mutexQueue
{
public:
	mutexQueue() : queueSize( SIZE ), writeIdx(0), readIdx(0), full(false)
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
		return true;
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
	bool full;
};



#endif /* QUEUE_HPP_ */
