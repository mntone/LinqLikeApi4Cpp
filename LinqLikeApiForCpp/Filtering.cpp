#include "pch.h"
#include "TestFramework.h"
#include "linq.hpp"

using namespace std;
using namespace TestFramework;

TEST_CLASS_BEGIN( Filtering )

vector<int> vec = { 0, 13, 40, 12, 50, 12, 60 };
auto linq = Linq::From( vec );

TEST_METHOD_BEGIN( Where )
Assert::IsEqual( vector<int> { 0, 40, 12, 50, 12, 60 }, linq.Where( []( int value ) { return value % 2 == 0; } ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( EqualTo )
Assert::IsEqual( vector<int> { 12, 12 }, linq.EqualTo( 12 ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( NotEqualTo )
Assert::IsEqual( vector<int> { 0, 13, 40, 50, 60 }, linq.NotEqualTo( 12 ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( LessThan )
Assert::IsEqual( vector<int> { 0, 12, 12 }, linq.LessThan( 13 ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( LessThanOrEqualTo )
Assert::IsEqual( vector<int> { 0, 13, 12, 12 }, linq.LessThanOrEqualTo( 13 ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( GreaterThan )
Assert::IsEqual( vector<int> { 40, 50, 60 }, linq.GreaterThan( 13 ).to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( GreaterThanOrEqualTo )
Assert::IsEqual( vector<int> { 13, 40, 50, 60 }, linq.GreaterThanOrEqualTo( 13 ).to_vector() );
TEST_METHOD_END

TEST_CLASS_END