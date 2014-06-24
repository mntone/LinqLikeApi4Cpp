/*
 *	The MIT License (MIT)
 *
 *	Copyright (c) 2014 mntone
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 *	associated documentation files( the "Software" ), to deal in the Software without restriction,
 *	including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *	and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do
 *	so, subject to the following conditions :
 *
 *	The above copyright notice and this permission notice shall be included in all copies or substantial
 *	portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 *	LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN
 *	NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once
#include <algorithm>
#include <type_traits>
#include <numeric>
#include <iterator>
#include <stdexcept>
#include <functional>
#include <cmath>

#ifdef _MSC_VER
#define constexpr inline
#endif

#ifdef __cplusplus_winrt
#define OUTOFRANGEEX throw ref new ::Platform::OutOfBoundsException();
#else
#define OUTOFRANGEEX throw ::std::out_of_range( "Out of range exception." );
#endif

#define ARITHMETICABLECHECK static_assert( ::std::is_arithmetic<typename Details::Wrap<T>::type>::value, "T is arithmeticable only." );

namespace Linq {

#pragma region Detail

	namespace Details {

		template<typename T> struct Wrap { using type = T; };
		template<typename T> constexpr const T& MakeWrap( const T& t ) { return t; }
		template<typename T> constexpr const T& Unwrap( const T& t ) { return t; }

#ifdef __cplusplus_winrt
		template<typename T>
		struct Wrap<T^>
			: ::std::conditional<__is_winrt_agile( T ), T^, ::Platform::Agile<T^>>
		{ };

		template<typename T>
		constexpr ::std::enable_if_t<!__is_winrt_agile( T ), ::Platform::Agile<T^>> MakeWrap( T^ const& t )
		{
			return ::Platform::Agile<T^>( t );
		}

		template<typename T> constexpr T^ Unwrap( const ::Platform::Agile<T^>& a ) { return a.Get(); }
#endif

		template<typename T> constexpr T Power2( T value ) { return value * value; }

		template<typename T> constexpr T Sqrt( T value ) { return ::std::sqrt( value ); }
		template<> constexpr int Sqrt( int value ) { return static_cast<int>( Sqrt( static_cast<double>( value ) ) ); }
		template<> constexpr unsigned int Sqrt( unsigned int value ) { return static_cast<unsigned int>( Sqrt( static_cast<double>( value ) ) ); }
		template<> constexpr long long Sqrt( long long value ) { return static_cast<long long>( Sqrt( static_cast<double>( value ) ) ); }
		template<> constexpr unsigned long long Sqrt( unsigned long long value ) { return static_cast<unsigned long long>( Sqrt( static_cast<double>( value ) ) ); }

	}

#pragma endregion

	template<typename T>
	class Vectorable
	{
	public:
		using SizeType = typename ::std::vector<typename Details::Wrap<T>::type>::size_type;
		using ItrType = typename ::std::vector<typename Details::Wrap<T>::type>::iterator;

	public:

#pragma region Constructors

		constexpr Vectorable( SizeType size )
			: data_( size )
		{ }

		constexpr Vectorable( T element, SizeType size )
			: Vectorable( size )
		{
			::std::fill( ::std::begin( data_ ), ::std::end( data_ ), Details::MakeWrap( element ) );
		}

		template<typename FwdItr>
		constexpr Vectorable( FwdItr begin, FwdItr end )
			: Vectorable( ::std::distance( begin, end ) )
		{
			::std::transform(
				begin,
				end,
				::std::begin( data_ ),
				[]( T value ) { return Details::MakeWrap( value ); } );
		}

#pragma endregion

#pragma region Getter

		constexpr ItrType Begin() { return ::std::begin( data_ ); }
		constexpr ItrType End() { return ::std::end( data_ ); }

		constexpr T First() const { return *::std::begin( data_ ); }
		constexpr T Last() const { return *--::std::end( data_ ); }
		constexpr T At( SizeType index ) const
		{
			if( index > data_.size() )
			{
				OUTOFRANGEEX
			}

			auto itr = ::std::begin( data_ );
			::std::advance( itr, index );
			return *itr;
		}

		constexpr T First( ::std::function<bool( T )> predicate ) const { return Where( predicate ).First(); }
		constexpr T Last( ::std::function<bool( T )> predicate ) const { return Where( predicate ).Last(); }

#pragma endregion

#pragma region Conditional Judgement

		constexpr bool All( T element ) const
		{
			return ::std::all_of( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::bind( ::std::equal_to<T>(), ::std::placeholders::_1, element ) );
		}
		constexpr bool All( ::std::function<bool( T )> predicate ) const
		{
			return ::std::all_of( ::std::cbegin( data_ ), ::std::cend( data_ ), predicate );
		}

		constexpr bool Any( T element ) const
		{
			return ::std::any_of( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::bind( ::std::equal_to<T>(), ::std::placeholders::_1, element ) );
		}
		constexpr bool Any( ::std::function<bool( T )> predicate ) const
		{
			return ::std::any_of( ::std::cbegin( data_ ), ::std::cend( data_ ), predicate );
		}

		constexpr bool None( T element ) const
		{
			return ::std::none_of( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::bind( ::std::equal_to<T>(), ::std::placeholders::_1, element ) );
		}
		constexpr bool None( ::std::function<bool( T )> predicate ) const
		{
			return ::std::none_of( ::std::cbegin( data_ ), ::std::cend( data_ ), predicate );
		}

		constexpr bool Empty() const
		{
			return data_.empty();
		}

		constexpr bool SequenceEqual( Vectorable second ) const
		{
			return Count() == second.Count() && ::std::equal( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::cbegin( second.data_ ) );
		}

		constexpr bool Contain( T element ) const
		{
			return ::std::find( ::std::cbegin( data_ ), ::std::cend( data_ ), element ) != ::std::cend( data_ );
		}
		constexpr bool Contain( Vectorable second ) const
		{
			return second.All( [&]( T value )
			{
				return ::std::find( ::std::cbegin( data_ ), ::std::cend( data_ ), value ) != ::std::cend( data_ );
			} );
		}
		constexpr bool Include( T element ) const { return Contain( element ); }
		constexpr bool Include( Vectorable second ) const { return Contain( second ); }

#pragma endregion

#pragma region Basic Calc

		constexpr SizeType Count() const { return data_.size(); }
		constexpr SizeType Count( T element ) const
		{
			return ::std::count( ::std::cbegin( data_ ), ::std::cend( data_ ), element );
		}
		constexpr SizeType Count( ::std::function<bool( T )> predicate ) const
		{
			return ::std::count_if( ::std::cbegin( data_ ), ::std::cend( data_ ), predicate );
		}

		constexpr T Sum() const
		{
			ARITHMETICABLECHECK

			return ::std::accumulate( ::std::cbegin( data_ ), ::std::cend( data_ ), static_cast<T>( 0 ) );
		}

		constexpr T Average() const
		{
			ARITHMETICABLECHECK

			return Sum() / static_cast<T>( Count() );
		}
		template<typename S>
		constexpr S Average() const
		{
			ARITHMETICABLECHECK

			return Cast<S>().Average();
		}	
		constexpr T Mean() const { return Average(); }
		template<typename S> constexpr S Mean() const { return Average<S>(); }

		constexpr T Minimum() const
		{
			ARITHMETICABLECHECK

			return *::std::min_element( ::std::cbegin( data_ ), ::std::cend( data_ ) );
		}

		constexpr T Maximum() const
		{
			ARITHMETICABLECHECK

			return *::std::max_element( ::std::cbegin( data_ ), ::std::cend( data_ ) );
		}

		constexpr T Median() const
		{
			ARITHMETICABLECHECK

			const auto s = OrderBy();
			const auto count = s.Count();
			auto halfCount = count / static_cast<SizeType>( 2 );
			return count % 2 == 0 ? ( s.At( halfCount - 1 ) + s.At( halfCount ) ) / 2 : s.At( halfCount );
		}

		constexpr T Variance() const
		{
			ARITHMETICABLECHECK

			const auto average = Average();
			const auto count = static_cast<T>( Count() );
			return Aggregate<T>(
				0,
				[average]( T value, T next ) { return value + Details::Power2( next - average ); },
				[count]( T value ) { return value / count; } );
		}
		constexpr T StandardDeviation() const { return static_cast<T>( Details::Sqrt( Variance() ) ); }

		constexpr T Aggregate( T seed, ::std::function<T( T, T )> func ) const
		{
			ARITHMETICABLECHECK

			return ::std::accumulate( ::std::cbegin( data_ ), ::std::cend( data_ ), seed, func );
		}

		template<typename S>
		constexpr S Aggregate( T seed, ::std::function<T( T, T )> func, ::std::function<S( T )> resultSelector ) const
		{
			ARITHMETICABLECHECK

			return resultSelector( ::std::accumulate( ::std::cbegin( data_ ), ::std::cend( data_ ), seed, func ) );
		}

#pragma endregion

#pragma region Filtering

#ifdef __cplusplus_winrt
		template<typename T>
		Vectorable OfType() const
		{
			return OfType( T::typeid );
		}

		Vectorable OfType( Windows::UI::Xaml::Interop::TypeName typeName ) const
		{
			static_assert( __is_valid_winrt_type( typename Details::Wrap<T>::type ), "T is winrt type only." );

			return Where( [typeName]( typename Details::Wrap<T>::type value )
			{
				return ::Windows::UI::Xaml::Interop::TypeName( Details::Unwrap( value )->GetType() ).Name == typeName.Name;
			} );
		}
#endif	
		constexpr Vectorable EqualTo( T value ) const
		{
			return Where( ::std::bind( ::std::equal_to<>(), ::std::placeholders::_1, value ) );
		}

		constexpr Vectorable NotEqualTo( T value ) const
		{
			return Where( ::std::bind( ::std::not_equal_to<>(), ::std::placeholders::_1, value ) );
		}

		constexpr Vectorable LessThan( T value ) const
		{
			ARITHMETICABLECHECK

			return Where( ::std::bind( ::std::less<>(), ::std::placeholders::_1, value ) );
		}

		constexpr Vectorable LessThanOrEqualTo( T value ) const
		{
			ARITHMETICABLECHECK

			return Where( ::std::bind( ::std::less_equal<>(), ::std::placeholders::_1, value ) );
		}

		constexpr Vectorable GreaterThan( T value ) const
		{
			ARITHMETICABLECHECK

			return Where( ::std::bind( ::std::greater<>(), ::std::placeholders::_1, value ) );
		}

		constexpr Vectorable GreaterThanOrEqualTo( T value ) const
		{
			ARITHMETICABLECHECK

			return Where( ::std::bind( ::std::greater_equal<>(), ::std::placeholders::_1, value ) );
		}

		constexpr Vectorable Where( ::std::function<bool( T )> predicate ) const
		{
			Vectorable ret( data_.size() );
			auto itr = ::std::copy_if( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::begin( ret.data_ ), predicate );
			ret.data_.resize( ::std::distance( ::std::begin( ret.data_ ), itr ) );
			return ::std::move( ret );
		}

#pragma endregion

#pragma region Basic Operation

		constexpr Vectorable Skip( SizeType count ) const
		{
			const auto size = data_.size();

			if( count > size )
			{
				OUTOFRANGEEX
			}

			Vectorable ret( size - count );
			auto begin = ::std::cbegin( data_ );
			::std::advance( begin, count );
			::std::copy( begin, ::std::cend( data_ ), ::std::begin( ret.data_ ) );
			return ::std::move( ret );
		}

		constexpr Vectorable SkipWhile( ::std::function<bool( T )> predicate ) const
		{
			SizeType i = 0;
			for( ; i < data_.size(); ++i )
			{
				const auto data = data_[i];
				if( !predicate( data ) )
				{
					break;
				}
			}

			return Skip( i );
		}

		constexpr Vectorable Take( SizeType count ) const
		{
			return TakeWhile( [&count]( T ) { return count-- != 0; } );
		}

		constexpr Vectorable TakeWhile( ::std::function<bool( T )> predicate ) const
		{
			const auto size = data_.size();

			Vectorable ret( size );

			SizeType i = 0;
			for( ; i < size; ++i )
			{
				const auto data = data_[i];
				if( !predicate( data ) )
				{
					break;
				}

				ret.data_[i] = data;
			}

			ret.data_.resize( i );
			return ::std::move( ret );
		}

		constexpr Vectorable Reverse() const
		{
			Vectorable ret( data_.size() );
			::std::reverse_copy( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::begin( ret.data_ ) );
			return ::std::move( ret );
		}

		constexpr Vectorable Rotate( SizeType advance ) const
		{
			Vectorable ret( data_.size() );
			auto itr = ::std::cbegin( data_ );
			::std::advance( itr, advance );
			::std::rotate_copy( ::std::cbegin( data_ ), itr, ::std::cend( data_ ), ::std::begin( ret.data_ ) );
			return ::std::move( ret );
		}

		constexpr Vectorable OrderBy() const
		{
			Vectorable ret( data_.size() );
			::std::copy( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::rbegin( ret.data_ ) );
			::std::sort( ::std::begin( ret.data_ ), ::std::end( ret.data_ ) );
			return ::std::move( ret );
		}
		constexpr Vectorable OrderBy( ::std::function<bool( T, T )> predicate ) const
		{
			Vectorable ret( data_.size() );
			::std::copy( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::rbegin( ret.data_ ) );
			::std::sort( ::std::begin( ret.data_ ), ::std::end( ret.data_ ), predicate );
			return ::std::move( ret );
		}

		constexpr Vectorable OrderByDescending() const
		{
			Vectorable ret( data_.size() );
			::std::copy( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::rbegin( ret.data_ ) );
			::std::sort( ::std::begin( ret.data_ ), ::std::end( ret.data_ ), ::std::greater<>() );
			return ::std::move( ret );
		}

#pragma endregion

#pragma region Set Calc

		constexpr Vectorable Distinct() const
		{
			Vectorable ret( data_.size() );
			auto itr = ::std::unique_copy( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::begin( ret.data_ ) );
			ret.data_.resize( ::std::distance( ::std::begin( ret.data_ ), itr ) );
			return ::std::move( ret );
		}
		constexpr Vectorable Distinct( ::std::function<bool( T )> predicate ) const
		{
			Vectorable ret( data_.size() );
			auto itr = ::std::unique_copy( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::begin( ret.data_ ), predicate );
			ret.data_.resize( ::std::distance( ::std::begin( ret.data_ ), itr ) );
			return ::std::move( ret );
		}

		constexpr Vectorable Concat( Vectorable second ) const
		{
			Vectorable ret( data_.size() + second.Count() );
			auto itr = ::std::copy( ::std::cbegin( data_ ), ::std::cend( data_ ), ::std::rbegin( ret.data_ ) );
			::std::copy( ::std::cbegin( second.data_ ), ::std::cend( second.data_ ), itr );
			return ::std::move( ret );
		}

		constexpr Vectorable Except( Vectorable second ) const
		{
			Vectorable ret( data_.size() );
			auto sortedFirst = OrderBy();
			auto sortedSecond = second.OrderBy();
			::std::set_difference(
				::std::cbegin( sortedFirst.data_ ),
				::std::cend( sortedFirst.data_ ),
				::std::cbegin( sortedSecond.data_ ),
				::std::cend( sortedSecond.data_ ),
				::std::inserter( ret.data_, ::std::begin( ret.data_ ) ) );
			return ::std::move( ret );
		}
		constexpr Vectorable Except( Vectorable second, ::std::function<bool( T, T )> predicate ) const
		{
			Vectorable ret( data_.size() );
			auto sortedFirst = OrderBy();
			auto sortedSecond = second.OrderBy();
			::std::set_difference(
				::std::cbegin( sortedFirst.data_ ),
				::std::cend( sortedFirst.data_ ),
				::std::cbegin( sortedSecond.data_ ),
				::std::cend( sortedSecond.data_ ),
				::std::inserter( ret.data_, ::std::begin( ret.data_ ) ),
				predicate );
			return ::std::move( ret );
		}
		constexpr Vectorable Differ( Vectorable second ) const { return Except( second ); }
		constexpr Vectorable Differ( Vectorable second, ::std::function<bool( T, T )> predicate ) const { return Except( second, predicate ); }

		constexpr Vectorable Union( Vectorable second ) const
		{
			Vectorable ret( data_.size() );
			auto sortedFirst = OrderBy();
			auto sortedSecond = second.OrderBy();
			::std::set_union(
				::std::cbegin( sortedFirst.data_ ),
				::std::cend( sortedFirst.data_ ),
				::std::cbegin( sortedSecond.data_ ),
				::std::cend( sortedSecond.data_ ),
				::std::back_inserter( ret.data_ ) );
			return ::std::move( ret );
		}
		constexpr Vectorable Union( Vectorable second, ::std::function<bool( T, T )> predicate ) const
		{
			Vectorable ret( data_.size() );
			auto sortedFirst = OrderBy();
			auto sortedSecond = second.OrderBy();
			::std::set_union(
				::std::cbegin( sortedFirst.data_ ),
				::std::cend( sortedFirst.data_ ),
				::std::cbegin( sortedSecond.data_ ),
				::std::cend( sortedSecond.data_ ),
				::std::back_inserter( ret.data_ ),
				predicate );
			return ::std::move( ret );
		}

		constexpr Vectorable Intersect( Vectorable second ) const
		{
			Vectorable ret( data_.size() );
			auto sortedFirst = OrderBy();
			auto sortedSecond = second.OrderBy();
			::std::set_difference(
				::std::cbegin( sortedFirst.data_ ),
				::std::cend( sortedFirst.data_ ),
				::std::cbegin( sortedSecond.data_ ),
				::std::cend( sortedSecond.data_ ),
				::std::inserter( ret.data_, ::std::begin( ret.data_ ) ) );
			return ::std::move( ret );
		}
		constexpr Vectorable Intersect( Vectorable second, ::std::function<bool( T, T )> predicate ) const
		{
			Vectorable ret( data_.size() );
			auto sortedFirst = OrderBy();
			auto sortedSecond = second.OrderBy();
			::std::set_intersection(
				::std::cbegin( sortedFirst.data_ ),
				::std::cend( sortedFirst.data_ ),
				::std::cbegin( sortedSecond.data_ ),
				::std::cend( sortedSecond.data_ ),
				::std::back_inserter( ret.data_ ),
				predicate );
			return ::std::move( ret );
		}

#pragma endregion

#pragma region Conversion

#ifdef __cplusplus_winrt
		template<typename S>
		constexpr Vectorable<S> Cast() const
		{
			return Cast<S>( ::std::integral_constant<bool, __is_valid_winrt_type( T ) && !::std::is_arithmetic<T>::value>() );
		}

		template<typename S>
		constexpr Vectorable<S> Cast( ::std::false_type ) const
		{
			return Select<S>( []( T value ) { return static_cast<S>( value ); } );
		}

		template<typename S>
		constexpr Vectorable<S> Cast( ::std::true_type ) const
		{
			static_assert( __is_valid_winrt_type( typename Details::Wrap<T>::type ), "T is winrt type only." );

			return Select<S>( []( T value ) { return dynamic_cast<S>( value ); } );
		}
#else
		template<typename S>
		constexpr Vectorable<S> Cast() const
		{
			return Select<S>( []( T value ) { return static_cast<S>( value ); } );
		}
#endif

		constexpr Vectorable Square() const
		{
			ARITHMETICABLECHECK

			return Select( Details::Power2<T> );
		}

		constexpr Vectorable Select( ::std::function<T( T )> selector ) const
		{
			Vectorable ret( data_.size() );
			::std::transform(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				ret.Begin(),
				[selector]( typename Details::Wrap<T>::type value ) { return Details::MakeWrap( selector( Details::Unwrap( value ) ) ); } );
			return ::std::move( ret );
		}
		template<typename S>
		constexpr Vectorable<S> Select( ::std::function<S( T )> selector ) const
		{
			Vectorable<S> ret( data_.size() );
			::std::transform(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				ret.Begin(),
				[selector]( typename Details::Wrap<T>::type value ) { return Details::MakeWrap( selector( Details::Unwrap( value ) ) ); } );
			return ::std::move( ret );
		}

#pragma endregion

#pragma region Vectorlize/Maplize

#if defined( _VECTOR_ ) || defined( _LIBCPP_VECTOR ) || defined( _STLP_VECTOR ) || defined( _GLIBCXX_VECTOR )
		constexpr ::std::vector<T> to_vector() const
		{
			::std::vector<T> ret( data_.size() );
			::std::transform(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				::std::begin( ret ),
				[]( typename Details::Wrap<T>::type value ) { return Details::Unwrap( value ); } );
			return ::std::move( ret );
		}
		template<typename S> constexpr ::std::vector<T> to_vector( ::std::function<S( T )> selector ) const { return Select<S>( selector ).to_vector(); }
#endif

#if defined( _DEQUE_ ) || defined( _LIBCPP_DEQUE ) || defined( _STLP_DEQUE ) || defined( _GLIBCXX_DEQUE )
		constexpr ::std::deque<T> to_deque() const
		{
			::std::deque<T> ret( data_.size() );
			::std::transform(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				::std::begin( ret ),
				[]( typename Details::Wrap<T>::type value ) { return Details::Unwrap( value ); } );
			return ::std::move( ret );
		}
		template<typename S> constexpr ::std::deque<T> to_deque( ::std::function<S( T )> selector ) const { return Select<S>( selector ).to_deque(); }
#endif

#if defined( _LIST_ ) || defined( _LIBCPP_LIST ) || defined( _STLP_LIST ) || defined( _GLIBCXX_LIST )
		constexpr ::std::list<T> to_list() const
		{
			::std::list<T> ret( data_.size() );
			::std::transform(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				::std::begin( ret ),
				[]( typename Details::Wrap<T>::type value ) { return Details::Unwrap( value ); } );
			return ::std::move( ret );
		}
		template<typename S> constexpr ::std::list<T> to_list( ::std::function<S( T )> selector ) const { return Select<S>( selector ).to_list(); }
#endif

#if defined( _FORWARD_LIST_ ) || defined( _LIBCPP_FORWARD_LIST ) || defined( _STLP_FORWARD_LIST ) || defined( _GLIBCXX_FORWARD_LIST )
		constexpr ::std::forward_list<T> to_forward_list() const
		{
			::std::forward_list<T> ret( data_.size() );
			::std::transform(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				::std::begin( ret ),
				[]( typename Details::Wrap<T>::type value ) { return Details::Unwrap( value ); } );
			return ::std::move( ret );
		}
		template<typename S> constexpr ::std::forward_list<T> to_forward_list( ::std::function<S( T )> selector ) const { return Select<S>( selector ).to_forward_list(); }
#endif

#if defined( _MAP_ ) || defined( _LIBCPP_MAP ) || defined( _STLP_MAP ) || defined( _GLIBCXX_MAP )
		template<typename SKey>
		inline ::std::map<SKey, T> to_map( ::std::function<SKey( T )> selector ) const
		{
			::std::map<SKey, T> ret;
			::std::for_each(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				[&ret, selector]( typename Details::Wrap<T>::type value )
				{
					auto unwarppedValue = Details::Unwrap( value );
					ret.emplace( selector( unwarppedValue ), unwarppedValue );
				} );
			return ::std::move( ret );
		}
		template<typename SKey, typename SValue>
		inline ::std::map<SKey, SValue> to_map( ::std::function<SKey( T )> keySelector, ::std::function<SValue( T )> valueSelector ) const
		{
			::std::map<SKey, SValue> ret;
			::std::for_each(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				[&ret, keySelector, valueSelector]( typename Details::Wrap<T>::type value )
				{
					auto unwarppedValue = Details::Unwrap( value );
					ret.emplace( keySelector( unwarppedValue ), valueSelector( unwarppedValue ) );
				} );
			return ::std::move( ret );
		}

		template<typename SKey>
		inline ::std::multimap<SKey, T> to_multimap( ::std::function<SKey( T )> selector ) const
		{
			::std::multimap<SKey, T> ret;
			::std::for_each(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				[&ret, selector]( typename Details::Wrap<T>::type value )
				{
					auto unwarppedValue = Details::Unwrap( value );
					ret.emplace( selector( unwarppedValue ), unwarppedValue );
				} );
			return ::std::move( ret );
		}
		template<typename SKey, typename SValue>
		inline ::std::multimap<SKey, SValue> to_multimap( ::std::function<SKey( T )> keySelector, ::std::function<SValue( T )> valueSelector ) const
		{
			::std::multimap<SKey, SValue> ret;
			::std::for_each(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				[&ret, keySelector, valueSelector]( typename Details::Wrap<T>::type value )
				{
					auto unwarppedValue = Details::Unwrap( value );
					ret.emplace( keySelector( unwarppedValue ), valueSelector( unwarppedValue ) );
				} );
			return ::std::move( ret );
		}
#endif

#if defined( _UNORDERED_MAP_ ) || defined(  _LIBCPP_UNORDERED_MAP ) || defined( _STLP_UNORDERED_MAP ) || defined( _GLIBCXX_UNORDERED_MAP )
		template<typename SKey>
		constexpr ::std::unordered_map<SKey, T> to_unordered_map( ::std::function<SKey( T )> selector ) const
		{
			::std::unordered_map<SKey, T> ret;
			::std::for_each(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				[&ret, selector]( typename Details::Wrap<T>::type value )
				{
					auto unwarppedValue = Details::Unwrap( value );
					ret.emplace( selector( unwarppedValue ), unwarppedValue );
				} );
			return ::std::move( ret );
		}
		template<typename SKey, typename SValue>
		constexpr::std::unordered_map<SKey, SValue> to_unordered_map( ::std::function<SKey( T )> keySelector, ::std::function<SValue( T )> valueSelector ) const
		{
			::std::unordered_map<SKey, SValue> ret;
			::std::for_each(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				[&ret, keySelector, valueSelector]( typename Details::Wrap<T>::type value )
				{
					auto unwarppedValue = Details::Unwrap( value );
					ret.emplace( keySelector( unwarppedValue ), valueSelector( unwarppedValue ) );
				} );
			return ::std::move( ret );
		}

		template<typename SKey>
		constexpr ::std::unordered_multimap<SKey, T> to_unordered_multimap( ::std::function<SKey( T )> selector ) const
		{
			::std::unordered_multimap<SKey, T> ret;
			::std::for_each(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				[&ret, selector]( typename Details::Wrap<T>::type value )
				{
					auto unwarppedValue = Details::Unwrap( value );
					ret.emplace( selector( unwarppedValue ), unwarppedValue );
				} );
			return ::std::move( ret );
		}
		template<typename SKey, typename SValue>
		constexpr::std::unordered_multimap<SKey, SValue> to_unordered_multimap( ::std::function<SKey( T )> keySelector, ::std::function<SValue( T )> valueSelector ) const
		{
			::std::unordered_multimap<SKey, SValue> ret;
			::std::for_each(
				::std::cbegin( data_ ),
				::std::cend( data_ ),
				[&ret, keySelector, valueSelector]( typename Details::Wrap<T>::type value )
				{
					auto unwarppedValue = Details::Unwrap( value );
					ret.emplace( keySelector( unwarppedValue ), valueSelector( unwarppedValue ) );
				} );
			return ::std::move( ret );
		}
#endif

#ifdef _COLLECTION_H_
		constexpr ::Windows::Foundation::Collections::IVector<T>^ ToVector() const
		{
			return ref new ::Platform::Collections::Vector<T>( ::std::move( to_vector() ) );
		}
		template<typename S> constexpr ::Windows::Foundation::Collections::IVector<S>^ ToVector( ::std::function<S( T )> selector ) const { return Select<S>( selector ).ToVector(); }

		constexpr ::Windows::Foundation::Collections::IVectorView<T>^ ToVectorView() const
		{
			return ref new ::Platform::Collections::VectorView<T>( ::std::move( to_vector() ) );
		}
		template<typename S> constexpr ::Windows::Foundation::Collections::IVectorView<S>^ ToVectorView( ::std::function<S( T )> selector ) const { return Select<S>( selector ).ToVectorView(); }

#ifdef VECTOR_EXTENSION
		constexpr::Windows::Foundation::Collections::IVector<T>^ ToDeque() const
		{
			return ref new ::Platform::Collections::Vector<T>( ::std::move( to_deque() ) );
		}
		template<typename S> constexpr::Windows::Foundation::Collections::IVector<S>^ ToDeque( ::std::function<S( T )> selector ) const { return Select<S>( selector ).ToDeque(); }

		constexpr::Windows::Foundation::Collections::IVectorView<T>^ ToDequeView() const
		{
			return ref new ::Platform::Collections::VectorView<T>( ::std::move( to_deque() ) );
		}
		template<typename S> constexpr::Windows::Foundation::Collections::IVectorView<S>^ ToDequeView( ::std::function<S( T )> selector ) const { return Select<S>( selector ).ToDequeView(); }
#endif

		template<typename SKey>
		constexpr ::Windows::Foundation::Collections::IMap<SKey, T>^ ToMap( ::std::function<SKey( T )> selector ) const
		{
			return ref new ::Platform::Collections::Map<SKey, T>( ::std::move( to_map( selector ) ) );
		}

		template<typename SKey, typename SValue>
		constexpr ::Windows::Foundation::Collections::IMap<SKey, SValue>^ ToMap( ::std::function<SKey( T )> keySelector, ::std::function<SValue( T )> valueSelector ) const
		{
			return ref new ::Platform::Collections::Map<SKey, SValue>( ::std::move( to_map( keySelector, valueSelector ) ) );
		}

		template<typename SKey>
		constexpr ::Windows::Foundation::Collections::IMapView<SKey, T>^ ToMapView( ::std::function<SKey( T )> selector ) const
		{
			return ref new ::Platform::Collections::MapView<SKey, T>( ::std::move( to_map( selector ) ) );
		}

		template<typename SKey, typename SValue>
		constexpr ::Windows::Foundation::Collections::IMapView<SKey, SValue>^ ToMapView( ::std::function<SKey( T )> keySelector, ::std::function<SValue( T )> valueSelector ) const
		{
			return ref new ::Platform::Collections::MapView<SKey, SValue>( ::std::move( to_map( keySelector, valueSelector ) ) );
		}

		template<typename SKey>
		constexpr ::Windows::Foundation::Collections::IMap<SKey, T>^ ToUnorderedMap( ::std::function<SKey( T )> selector ) const
		{
			return ref new ::Platform::Collections::UnorderedMap<SKey, T>( ::std::move( to_unordered_map( selector ) ) );
		}

		template<typename SKey, typename SValue>
		constexpr ::Windows::Foundation::Collections::IMap<SKey, SValue>^ ToUnorderedMap( ::std::function<SKey( T )> keySelector, ::std::function<SValue( T )> valueSelector ) const
		{
			return ref new ::Platform::Collections::UnorderedMap<SKey, SValue>( ::std::move( to_unordered_map( keySelector, valueSelector ) ) );
		}

		template<typename SKey>
		constexpr ::Windows::Foundation::Collections::IMapView<SKey, T>^ ToUnorderedMapView( ::std::function<SKey( T )> selector ) const
		{
			return ref new ::Platform::Collections::UnorderedMapView<SKey, T>( ::std::move( to_unordered_map( selector ) ) );
		}

		template<typename SKey, typename SValue>
		constexpr ::Windows::Foundation::Collections::IMapView<SKey, SValue>^ ToUnorderedMapView( ::std::function<SKey( T )> keySelector, ::std::function<SValue( T )> valueSelector ) const
		{
			return ref new ::Platform::Collections::UnorderedMapView<SKey, SValue>( ::std::move( to_unordered_map( keySelector, valueSelector ) ) );
		}
#endif

#pragma endregion

	private:
		::std::vector<typename Details::Wrap<T>::type> data_;
	};

	template<typename T>
	using RemoveIteratorT = ::std::remove_const_t<::std::remove_reference_t<::std::remove_const_t<T>>>;

	template<class Container>
	constexpr auto From( const Container& container ) -> Vectorable<RemoveIteratorT<decltype( *::std::begin( container ) )>>
	{
		return Vectorable<RemoveIteratorT<decltype( *::std::begin( container ) )>>( ::std::cbegin( container ), ::std::cend( container ) );
	}

	template<class Integer>
	constexpr Vectorable<Integer> Range( Integer from, Integer to )
	{
		static_assert( ::std::is_integral<Integer>::value, "T is integer only." );

		Vectorable<Integer> ret( to - from + 1 );
		::std::generate( ret.Begin(), ret.End(), [&from] { return from++; } );
		return ret;
	}

	template<typename T>
	constexpr Vectorable<T> Repeat( T element, typename Vectorable<T>::SizeType count )
	{
		return Vectorable<T>( element, count );
	}

}

#ifdef _MSC_VER
#undef constexpr
#endif

#undef OUTOFRANGEEX
#undef ARITHMETICABLECHECK