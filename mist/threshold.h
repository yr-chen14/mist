#ifndef __INCLUDE_MIST_THRESHOLD__
#define __INCLUDE_MIST_THRESHOLD__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// �ŋߖT�^���
template < class T, class Allocator >
typename array< T, Allocator >::value_type threshold( const array< T, Allocator > &in )
{
	typedef typename array< T, Allocator >::size_type  size_type;
	typedef typename array< T, Allocator >::value_type value_type;

	value_type min = in[ 0 ];
	value_type max = in[ 0 ];

	size_type i, k;

	for( i = 1 ; i < in.size( ) ; i++ )
	{
		if( min > in[ i ] )
		{
			min = in[ i ];
		}
		else if( max < in[ i ] )
		{
			max = in[ i ];
		}
	}

	size_type level = static_cast< size_type >( max - min ) + 1;
	size_type max_k;
	double *p   = new double[ level ];
	double myu, omg;
	double myuT, sig, max_sig = 0.0;

	size_type N = in.size( );
	for( k = 0 ; k < level ; k++ )
	{
		p[ k ] = 0.0;
	}
	for( k = 0 ; k < N ; k++ )
	{
		p[ static_cast< size_type >( in[ k ] - min ) ]++;
	}
	for( k = 0 ; k < level ; k++ )
	{
		p[ k ] /= static_cast< double >( N );
	}

	myuT = 0.0;
	for( k = 0 ; k < level ; k++ )
	{
		myuT += k * p[ k ];
	}

	myu = 0.0;
	omg = p[ 0 ];
	max_k = 0;
	max_sig = ( myuT * omg - myu ) * ( myuT * omg - myu ) / ( omg * ( 1.0 - omg ) );
	for( k = 1 ; k < level ; k++ )
	{
		omg = omg + p[ k ];
		myu = myu + k * p[ k ];
		sig = ( myuT * omg - myu ) * ( myuT * omg - myu ) / ( omg * ( 1.0 - omg ) );
		if( sig > max_sig )
		{
			max_sig = sig;
			max_k = k;
		}
	}

	delete [] p;

	return( static_cast< value_type >( max_k + min ) );
}



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_THRESHOLD__