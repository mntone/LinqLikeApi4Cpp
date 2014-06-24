#include "pch.h"
#include "TestFramework.h"
#include "linq.hpp"

using namespace std;
using namespace TestFramework;

TEST_CLASS_BEGIN( BasicCalc )

vector<int> vec = { 0, 13, 40, 12, 50, 12, 60 };
auto linq = Linq::From( vec );

TEST_METHOD_BEGIN( Count )
Assert::IsEqual( vec.size(), linq.Count() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Count2 )
Assert::IsEqual( static_cast<size_t>( 2 ), linq.Count( 12 ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Count3 )
Assert::IsEqual( static_cast<size_t>( 3 ), linq.Count( []( int value ) { return value <= 12; } ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Sum )
Assert::IsEqual( accumulate( cbegin( vec ), cend( vec ), 0 ), linq.Sum() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Average )
Assert::IsEqual( accumulate( cbegin( vec ), cend( vec ), 0 ) / static_cast<int>( vec.size() ), linq.Average() );
TEST_METHOD_END

TEST_METHOD_BEGIN( CastAverage )
Assert::IsEqual( accumulate( cbegin( vec ), cend( vec ), 0 ) / static_cast<float>( vec.size() ), linq.Average<float>() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Mean )
Assert::IsEqual( accumulate( cbegin( vec ), cend( vec ), 0 ) / static_cast<int>( vec.size() ), linq.Mean() );
TEST_METHOD_END

TEST_METHOD_BEGIN( CastMean )
Assert::IsEqual( accumulate( cbegin( vec ), cend( vec ), 0 ) / static_cast<float>( vec.size() ), linq.Mean<float>() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Minimum )
Assert::IsEqual( *min_element( cbegin( vec ), cend( vec ) ), linq.Minimum() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Maximum )
Assert::IsEqual( *max_element( cbegin( vec ), cend( vec ) ), linq.Maximum() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Median )
Assert::IsEqual( static_cast<int>( 13 ), linq.Median() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Variance )
Assert::IsEqual( static_cast<int>( 452 ), linq.Variance() );
TEST_METHOD_END

TEST_METHOD_BEGIN( StandardDeviation )
Assert::IsEqual( static_cast<int>( 21 ), linq.StandardDeviation() );
TEST_METHOD_END

TEST_METHOD_BEGIN( Aggregate )
Assert::IsEqual(
	accumulate( cbegin( vec ), cend( vec ), 1, ::std::multiplies<int>() ),
	linq.Aggregate( 1, []( int x, int y ) { return x * y; } ) );
TEST_METHOD_END

TEST_CLASS_END