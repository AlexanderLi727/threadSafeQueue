#include <gtest/gtest.h>
#include <exception>
#include <iostream>
#include <limits.h>
#include <chrono>
#include <thread>
//#include <gmock/gmock.h>
#include "mutexQueue.hpp"
#include "condvarQueue.hpp"


int main( int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv); 
	return RUN_ALL_TESTS(); 
}

TEST(condvarQueueTest, firstMuliThreadedTestPush)
{
	condvarQueue<int, 100> queue; 
	std::thread th1([&queue](){
			for( int i = 0; i < 20; ++i )
			{
				int val = 0; 
				queue.waitTopPop(val); 
			}

	});

	std::thread th2([&queue](){
			for( int i = 0; i < 20; ++i )
			{
				int val = 0; 
				queue.waitTopPop(val); 
			}

	});

	std::thread th3([&queue](){
			std::this_thread::sleep_for(std::chrono::seconds(5)); 
			for( int i = 0; i < 10; ++i )
			{
				queue.push(std::move(i));
			}
			std::this_thread::sleep_for(std::chrono::seconds(5)); 
			for( int i = 0; i < 10; ++i )
			{
				queue.push(std::move(i));
			}
			std::this_thread::sleep_for(std::chrono::seconds(5)); 
			for( int i = 0; i < 10; ++i )
			{
				queue.push(std::move(i));
			}
			std::this_thread::sleep_for(std::chrono::seconds(5)); 
			for( int i = 0; i < 10; ++i )
			{
				queue.push(std::move(i));
			}

	});


	th1.join(); 
	th2.join(); 
	th3.join(); 

	EXPECT_EQ(queue.isEmpty(), true);

}

TEST(mutexQueueTest, firstMuliThreadedTestPush)
{
	mutexQueue<int, 100000> queue; 
	std::thread th1([&queue](){
			for( int i = 0; i < 60000; ++i )
			{
				queue.push(std::move(i));
			}

	});

	std::thread th2([&queue](){
			for( int i = 0; i < 60000; ++i )
			{
				queue.push(std::move(i));
			}

	});

	std::thread th3([&queue](){
			for( int i = 0; i < 60000; ++i )
			{
				queue.push(std::move(i));
			}

	});

	std::thread th4([&queue](){
			std::this_thread::sleep_for( std::chrono::seconds(5) ); 
			for( int i = 0; i < 100000; ++i )
			{
				queue.pop( );
			}

	});


	th1.join(); 
	th2.join(); 
	th3.join(); 
	th4.join(); 
}

TEST(mutexQueueTest, firstSingleThreadedTestPush)
{
	mutexQueue<int, 3> queue; 
	EXPECT_EQ(queue.push( 100 ), true);
	EXPECT_EQ(queue.push( 200 ), true);
	EXPECT_EQ(queue.push( 300 ), true);
	EXPECT_EQ(queue.push( 400 ), false);
	EXPECT_EQ(queue.push( 400 ), false);

}

TEST(mutexQueueTest, firstSingleThreadedTestEmpty)
{
	mutexQueue<int, 3> queue; 
	EXPECT_EQ(queue.isEmpty(), true);
	EXPECT_EQ(queue.push( 100 ), true);
	EXPECT_EQ(queue.isEmpty(), false);
}

TEST(mutexQueueTest, firstSingleThreadedTestPop)
{
	mutexQueue<int, 3> queue; 
	EXPECT_EQ(queue.push( 100 ), true); 	/* 1 */
	EXPECT_EQ( queue.pop(), true );		/* 0  */
	EXPECT_EQ( queue.pop(), false );	/* 0 */	
	EXPECT_EQ(queue.push( 100 ), true);	/* 1 */		
	EXPECT_EQ(queue.push( 100 ), true);	/* 2 */	
	EXPECT_EQ(queue.push( 100 ), true);	/* 3 */
	EXPECT_EQ(queue.push( 100 ), false);	/* 3 */
	EXPECT_EQ( queue.pop(), true );		/* 2 */
	EXPECT_EQ(queue.push( 100 ), true);	/* 3 */
	EXPECT_EQ( queue.pop(), true );		/* 2 */
	EXPECT_EQ( queue.pop(), true );		/* 1 */
	EXPECT_EQ( queue.pop(), true );		/* 0 */
	EXPECT_EQ( queue.pop(), false );	/* 0 */
	EXPECT_EQ(queue.push( 100 ), true);	/* 1 */		
	EXPECT_EQ(queue.push( 100 ), true);	/* 2 */	
	EXPECT_EQ(queue.push( 100 ), true);	/* 3 */
	EXPECT_EQ( queue.pop(), true );		/* 2 */
	EXPECT_EQ( queue.pop(), true );		/* 1 */
	EXPECT_EQ(queue.push( 100 ), true);	/* 2 */
	EXPECT_EQ(queue.push( 100 ), true);	/* 3 */
}

TEST(mutexQueueTest, firstSingleThreadedTestTop)
{
	mutexQueue<int, 3> queue; 
	int val = INT_MAX; 
	EXPECT_EQ(queue.push( 100 ), true); 	
	EXPECT_EQ(queue.top( val ), true); 
	EXPECT_EQ(val, 100 ); 	
	EXPECT_EQ( queue.pop(), true );		
	EXPECT_EQ(queue.push( 100 ), true); 	
	EXPECT_EQ(queue.push( 200 ), true); 	
	EXPECT_EQ(queue.push( 300 ), true); 	

	EXPECT_EQ(queue.top( val ), true); 
	EXPECT_EQ(val, 100 ); 	
	EXPECT_EQ( queue.pop(), true );		

	EXPECT_EQ(queue.top( val ), true); 
	EXPECT_EQ(val, 200 ); 	
	EXPECT_EQ( queue.pop(), true );		

	EXPECT_EQ(queue.top( val ), true); 
	EXPECT_EQ(val, 300 ); 	
	EXPECT_EQ( queue.pop(), true );		

	EXPECT_EQ(queue.top( val ), false); 
}




