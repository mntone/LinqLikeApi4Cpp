#include "pch.h"
#include "TestFramework.h"
#include "linq.hpp"

using namespace std;
using namespace TestFramework;

TEST_CLASS_BEGIN( Getter )

vector<int> vec = { 0, 13, 40, 12, 50, 12, 60 };
auto linq = Linq::From( vec );

TEST_METHOD_BEGIN( First )
Assert::IsEqual( *vec.cbegin(), linq.First() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Last )
Assert::IsEqual( *--vec.cend(), linq.Last() );
TEST_METHOD_END

TEST_METHOD_BEGIN( At )
Assert::IsEqual( *( vec.cbegin() + 2 ), linq.At( 2 ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( First2 )
Assert::IsEqual( *( vec.cbegin() + 1 ), linq.First( []( int value ) { return value != 0; } ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Last2 )
Assert::IsEqual( *vec.cbegin(), linq.Last( []( int value ) { return value < 12; } ) );
TEST_METHOD_END

TEST_CLASS_END