#pragma once
#include <map>
#include <functional>
#include <exception>
#include <sstream>

namespace std {

	template<typename T>
	ostream& operator<<( ostream& ostr, const ::std::vector<T>& val )
	{
		ostr << '{';
		for( auto&& v : val )
		{
			ostr << v << ", ";
		}
		return ostr << '}';
	}

}

namespace TestFramework {

	class TestBase
	{
	public:
		void Play();

	protected:
		::std::string className_;
		::std::vector<::std::pair<::std::string, ::std::function<void( void )>>> tests_;
	};

	class AssertException final
		: public ::std::exception
	{
	public:
#ifdef _MSC_VER
		explicit AssertException( const char* const& what )
			: ::std::exception( what )
#else
		AssertException( const char* const what )
			: what_( what )
#endif
		{ }


#ifndef _MSC_VER
	private:
		const char* const what_;
#endif
	};

	class Assert final
	{
	public:
		template<typename T> static void CheckWithBinaryFunction( const T& expected, const T& actual, ::std::function<bool( T, T )> func, const char* what = nullptr )
		{
			if( !func( expected, actual ) )
			{
				throw AssertException( what );
			}
		}

		template<typename T> static void IsEqual( const T& expected, const T& actual )
		{
			::std::ostringstream buf;
			buf << expected << " (expected) is NOT equal to " << actual << " (actual).";
			CheckWithBinaryFunction<T>( expected, actual, ::std::equal_to<T>(), buf.str().c_str() );
		}

		template<typename T> static void IsNotEqual( const T& expected, const T& actual )
		{
			::std::ostringstream buf;
			buf << expected << " (expected) isn't NOT equal to " << actual << " (actual).";
			CheckWithBinaryFunction<T>( expected, actual, ::std::not_equal_to<T>(), buf.str().c_str() );
		}

		static void IsTrue( bool actual )
		{
			::std::ostringstream buf;
			buf << ::std::boolalpha << "true (expected) is NOT equal to " << actual << " (actual).";
			CheckWithBinaryFunction<bool>( true, actual, ::std::equal_to<bool>(), buf.str().c_str() );
		}

		static void IsFalse( bool actual )
		{
			::std::ostringstream buf;
			buf << ::std::boolalpha << "false (expected) is NOT equal to " << actual << " (actual).";
			CheckWithBinaryFunction<bool>( false, actual, ::std::equal_to<bool>(), buf.str().c_str() );
		}
	};

	extern ::std::vector<TestBase> testClasses_;

	void Run();
	void Wait();
}

#define DEFINE_TEST_CLASS( __NAME__ ) class __NAME__ final: public TestFramework::TestBase { public: __NAME__(); };
#define TEST_CLASS_BEGIN( __NAME__ ) DEFINE_TEST_CLASS( __NAME__ ) __NAME__::__NAME__() { className_ = #__NAME__;
#define TEST_CLASS_END }
#define TEST_METHOD_BEGIN( __NAME__ ) tests_.emplace_back( #__NAME__, [=] { 
#define TEST_METHOD_END } );
#define REGISTER_TEST_CLASS( __NAME__ ) TestFramework::testClasses_.push_back( static_cast<TestFramework::TestBase>( __NAME__() ) );