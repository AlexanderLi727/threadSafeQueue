#include <iostream>

#include "mutexQueue.hpp"

int main()
{
	mutexQueue<int, 100000> queue; 
	long long  counter = 0; 

	for( int i = 0; i < 100000; ++i )
	{
		queue.push(std::move(i));
	}
	for( int i = 0; i < 100000; ++i )
	{
		int var = 0; 
		queue.top(var);
		counter += var; 
		queue.pop();
	}
	std::cout << "Counter: " << counter << std::endl; 
	return 0; 
}
