#include "pch.h"
#include "TestFramework.h"
#include "linq.hpp"

using namespace std;
using namespace TestFramework;

TEST_CLASS_BEGIN( ConditionalJudgement )

vector<int> vec = { 0, 13, 40, 12, 50, 12, 60 };
auto linq = Linq::From( vec );
auto linq2 = linq.Cast<double>().Cast<int>();

vector<int> empty = { };
auto emptyLinq = Linq::From( empty );

vector<int> partical = { 13, 12, 60 };
auto particalLinq = Linq::From( partical );

vector<int> notPartical = { 13, 12, 59 };
auto notParticalLinq = Linq::From( notPartical );

TEST_METHOD_BEGIN( All )
Assert::IsTrue( linq.All( []( int value ) { return value >= 0; } ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( All2 )
Assert::IsFalse( linq.All( []( int value ) { return value > 0; } ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Any )
Assert::IsTrue( linq.Any( []( int value ) { return value == 0; } ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Any2 )
Assert::IsFalse( linq.Any( []( int value ) { return value == 1; } ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( None )
Assert::IsTrue( linq.None( []( int value ) { return value == 100; } ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( None2 )
Assert::IsFalse( linq.None( []( int value ) { return value == 0; } ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( SequenceEqual )
Assert::IsTrue( linq.SequenceEqual( linq2 ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( SequenceEqual2 )
Assert::IsFalse( linq.SequenceEqual( particalLinq ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Contain )
Assert::IsTrue( linq.Contain( 0 ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Contain2 )
Assert::IsFalse( linq.Contain( 1 ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Contain3 )
Assert::IsTrue( linq.Contain( particalLinq ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Contain4 )
Assert::IsFalse( linq.Contain( notParticalLinq ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Include )
Assert::IsTrue( linq.Include( 0 ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Include2 )
Assert::IsFalse( linq.Include( 1 ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Include3 )
Assert::IsTrue( linq.Include( particalLinq ) );
TEST_METHOD_END

TEST_METHOD_BEGIN( Include4 )
Assert::IsFalse( linq.Include( notParticalLinq ) );
TEST_METHOD_END

TEST_CLASS_END