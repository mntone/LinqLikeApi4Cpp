#include "pch.h"
#include "TestFramework.h"
#include <iostream>

void TestFramework::TestBase::Play()
{
	::std::cout << className_.c_str() << ::std::endl;

	size_t i = 0;
	for( auto&& test : tests_ )
	{
		::std::cout << "[" << i++ << "] " << test.first << ": ";
		try
		{
			test.second();
			::std::cout << "passed";
		}
		catch( const TestFramework::AssertException ex )
		{
			::std::cout << "failed (" << ex.what() << ')';
		}
		::std::cout << ::std::endl;
	}
	::std::cout << ::std::endl;
}

::std::vector<TestFramework::TestBase> TestFramework::testClasses_;

void TestFramework::Run()
{
	for( auto&& testClass : testClasses_ )
	{
		testClass.Play();
	}
}

void TestFramework::Wait()
{
	::std::cout << "Press any key to exit." << ::std::endl;
	getchar();
}