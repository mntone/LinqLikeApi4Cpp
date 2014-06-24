#include "pch.h"
#include "TestFramework.h"
#include "linq.hpp"

using namespace std;
using namespace TestFramework;

TEST_CLASS_BEGIN( BasicOperation )

vector<int> vec = { 0, 13, 40, 12, 50, 12, 60 };
auto linq = Linq::From( vec );

TEST_METHOD_BEGIN( Skip )
Assert::IsEqual( vector<int> { 40, 12, 50, 12, 60 }, linq.Skip( 2 ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( SkipWhile )
Assert::IsEqual( vector<int> { 40, 12, 50, 12, 60 }, linq.SkipWhile( []( int value ) { return value <= 13; } ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Take )
Assert::IsEqual( vector<int> { 0, 13, 40 }, linq.Take( 3 ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( TakeWhile )
Assert::IsEqual( vector<int> { 0, 13, 40, 12 }, linq.TakeWhile( []( int value ) { return value <= 40; } ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Reverse )
Assert::IsEqual( vector<int> { 60, 12, 50, 12, 40, 13, 0 }, linq.Reverse().to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Rotate )
Assert::IsEqual( vector<int> { 40, 12, 50, 12, 60, 0, 13 }, linq.Rotate( 2 ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( OrderBy )
Assert::IsEqual( vector<int> { 0, 12, 12, 13, 40, 50, 60 }, linq.OrderBy().to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( OrderBy2 )
Assert::IsEqual( vector<int> { 60, 50, 40, 13, 12, 12, 0 }, linq.OrderBy( greater<int>() ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( OrderByDescending )
Assert::IsEqual( vector<int> { 60, 50, 40, 13, 12, 12, 0 }, linq.OrderByDescending().to_vector() );
TEST_METHOD_END

TEST_CLASS_END