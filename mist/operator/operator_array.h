#ifndef __INCLUDE_MIST_OPERATOR_ARRAY__
#define __INCLUDE_MIST_OPERATOR_ARRAY__

//#define _CHECK_ARRAY_OPERATION_


template < class T, class Allocator >
inline const array< T, Allocator >& operator +=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// 足し算できません例外
		::std::cout << "can't add arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator -=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// 引き算できません例外
		::std::cout << "can't subtract arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator *=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// 掛け算できません例外
		::std::cout << "can't multiply arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator /=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// 割り算できません例外
		::std::cout << "can't divide arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator +=( array< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += val;
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator -=( array< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator *=( array< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator /=( array< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( val == 0 )
	{
		// ゼロ除算発生
		::std::cout << "zero division occured." << ::std::endl;
		return;
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= val;
	return( a1 );
}


// 符号反転
template < class T, class Allocator >
inline array< T, Allocator > operator -( const array< T, Allocator > &a )
{
	typedef typename array< T, Allocator >::size_type size_type;
	array< T, Allocator > o( a.size( ) );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -a[i];
	return( o );
}


// 足し算
template < class T, class Allocator >
inline array< T, Allocator > operator +( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) += a2 );
}


// 引き算
template < class T, class Allocator >
inline array< T, Allocator > operator -( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) -= a2 );
}


// 掛け算
template < class T, class Allocator >
inline array< T, Allocator > operator *( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) *= a2 );
}

// 割り算
template < class T, class Allocator >
inline array< T, Allocator > operator /( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) /= a2 );
}

// 定数との足し算
template < class T, class Allocator >
inline array< T, Allocator > operator +( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) += val );
}

template < class T, class Allocator >
inline array< T, Allocator > operator +( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array< T, Allocator >( a ) += val );
}



// 定数との引き
template < class T, class Allocator >
inline array< T, Allocator > operator -( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) -= val );
}

template < class T, class Allocator >
inline array< T, Allocator > operator -( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array< T, Allocator >( a ) -= val );
}


// 定数との掛け算
template < class T, class Allocator >
inline array< T, Allocator > operator *( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) *= val );
}

template < class T, class Allocator >
inline array< T, Allocator > operator *( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array< T, Allocator >( a ) *= val );
}


// 定数との割り
template < class T, class Allocator >
inline array< T, Allocator > operator /( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) /= val );
}

#endif // __INCLUDE_MIST_OPERATOR_ARRAY__