#include "pch.h"
#include "TestFramework.h"
#include "linq.hpp"

using namespace std;
using namespace TestFramework;

TEST_CLASS_BEGIN( Conversion )

vector<int> vec = { 0, 13, 40, 12, 50, 12, 60 };
auto linq = Linq::From( vec );

TEST_METHOD_BEGIN( Cast )
Assert::IsEqual( vector<double> { 0, 13, 40, 12, 50, 12, 60 }, linq.Cast<double>().to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( SkipWhile )
Assert::IsEqual( vector<int> { 0, 169, 1600, 144, 2500, 144, 3600 }, linq.Square().to_vector() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Select )
Assert::IsEqual( vector < int > { 0, 2197, 64000, 1728, 125000, 1728, 216000 }, linq.Select( []( int value ) { return value * value * value; } ).to_vector() );
TEST_METHOD_END

TEST_CLASS_END