#ifndef __INCLUDE_MIST_MEDIAN_FILTER__
#define __INCLUDE_MIST_MEDIAN_FILTER__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "../config/type_trait.h"
#endif


#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif

#include <algorithm>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// ���f�B�A���t�B���^
namespace __median_filter_with_histogram__
{
	// in  : ���͉摜. ���͉摜�̉�f�l�� min �� max �̊ԂƂ���
	// out : �o�͉摜. �o�͉摜�̃������͂��炩���ߊ��蓖�Ă��Ă�����̂Ƃ���
	// fw, fh, fd : �t�B���^�T�C�Y
	// min, max : �Z�W�͈�
	template < class Array1, class Array2 >
	void median_filter( const Array1 &in, Array2 &out,
						typename Array1::size_type fw, typename Array1::size_type fh, typename Array1::size_type fd,
						typename Array1::value_type min, typename Array1::value_type max,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array2::value_type out_value_type;
		typedef unsigned short hist_value;

		size_type range = static_cast< size_type >( max - min + 1 );
		size_type a = 0, i, j, k, x, y, z, ri, leftnum;
		signed int pth, th, lt_med;
		value_type med;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		size_type bw = fw / 2;
		size_type bh = fh / 2;
		size_type bd = fd / 2;

		value_type *leftmost = new value_type[ fh * fd ];
		value_type *sort = new value_type[ fw * fh * fd + 1 ];
		hist_value *hist = new hist_value[range];

		for( k = thread_idz ; k < d ; k += thread_numz )
		{
			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				i = 0;

				memset( hist, 0, sizeof( hist_value ) * range );
				pth = 0;
				leftnum = 0;
				for( z = k < bd ? 0 : k - bd ; z <= k + bd && z < d ; z++ )
				{
					for(y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
					{
						leftmost[ leftnum++ ] = in( 0, y, z ) - min;
						for( x = 0 ; x <= bw ; x++ )
						{
							sort[ pth++ ] = in( x, y, z ) - min;
							hist[ in( x, y, z ) - min ]++;
						}
					}
				}

				// �����ɕ��בւ���
				std::sort( sort, sort + pth );
				pth--;

				th = pth / 2;
				med = sort[ th ];

				if( sort[ 0 ] == sort[ th ] )
				{
					lt_med = 0;
				}
				else
				{
					a = th;
					while( med <= sort[ --a ] );
					lt_med = static_cast< signed int >( a + 1 );
				}

				out( i, j, k ) = static_cast< out_value_type >( med + min );

				for( i = 1 ; i < w ; i++ )
				{
					ri = i + bw;

					if( bw < i )
					{
						for( a = 0 ; a < leftnum ; a++ )
						{
							hist[ leftmost[ a ] ]--;
							pth--;
							if( leftmost[ a ] < med )
							{
								lt_med--;
							}
						}
					}

					leftnum = 0;
					for( z = k < bd ? 0 : k - bd ; z <= k + bd && z < d ; z++ )
					{
						for( y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
						{
							if( ri < w )
							{
								hist[ in( ri, y, z ) - min ]++;
								pth++;
								if( in( ri, y, z ) - min < med )
								{
									lt_med++;
								}
							}

							if ( bw <= i )
							{
								leftmost[ leftnum++ ] = in( i - bw, y, z ) - min;
							}
						}
					}

					th = pth / 2;

					if( lt_med <= th )
					{
						while( lt_med + hist[ med ] <= th )
						{
							lt_med += hist[ med ];
							med++;
						}
					}
					else
					{
						while( th < lt_med )
						{
							med--;
							lt_med -= hist[ med ];
						}
					}

					out( i, j, k ) = static_cast< out_value_type >( med + min );
				}
			}
		}

		delete [] hist;
		delete [] sort;
		delete [] leftmost;
	}
}


namespace __median_filter_divide_conquer__
{
	// DATA�^�̔z��̒����l�������Ɍv�Z����A���S���Y��
	// �z��̗v�f ����num�ł���B
	// �z����ő�c�Ԗڂɏ������l����������
	// ���͂����DATA�^�̔z��͒��g�����ւ��Ȃ�����ɁC���[�N�z��𗘗p����
	template < class T >
	inline T nth_value( const T *a, size_t num, size_t c, T *work1, T *work2, T *work3 )
	{
		typedef T value_type;

		size_t i;
		size_t wi1, wi2, wi3;
		value_type s;
		value_type *w1 = work1;
		value_type *w2 = work2;
		value_type *src, *www;

		wi1 = wi2 = wi3 = 0;
		s = a[ c ];

		for( i = 0 ; i < num ; i++ )
		{
			if( a[ i ] < s )
			{
				w1[ wi1++ ] = a[ i ];
			}
			else if( a[ i ] > s )
			{
				w2[ wi2++ ] = a[ i ];
			}
			else
			{
				wi3++;
			}
		}

		if( wi1 > c )
		{
			src = w1;
			w1 = work3;
			num = wi1;
		}
		else if( wi1 + wi3 <= c )
		{
			src = w2;
			w2 = work3;
			c -= wi1 + wi3;
			num = wi2;
		}
		else
		{
			return ( s );
		}

		while( true )
		{
			wi1 = wi2 = wi3 = 0;
			s = src[ c ];

			for( i = 0 ; i < num ; i++ )
			{
				if( src[ i ] < s )
				{
					w1[ wi1++ ] = src[ i ];
				}
				else if( src[ i ] > s )
				{
					w2[ wi2++ ] = src[ i ];
				}
				else
				{
					wi3++;
				}
			}

			if( wi1 > c )
			{
				www = src;
				src = w1;
				w1 = www;
				num = wi1;
			}
			else if( wi1 + wi3 <= c )
			{
				www = src;
				src = w2;
				w2 = www;
				c -= wi1 + wi3;
				num = wi2;
			}
			else
			{
				break;
			}
		}

		return ( s );
	}


	// in  : ���͉摜. ���͉摜�̉�f�l�� min �� max �̊ԂƂ���
	// out : �o�͉摜. �o�͉摜�̃������͂��炩���ߊ��蓖�Ă��Ă�����̂Ƃ���
	// fw, fh, fd : �t�B���^�T�C�Y
	// min, max : �Z�W�͈�
	template < class Array1, class Array2 >
	void median_filter( const Array1 &in, Array2 &out,
						typename Array1::size_type fw, typename Array1::size_type fh, typename Array1::size_type fd,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array2::value_type out_value_type;

		size_type i, j, k, x, y, z, ri;
		size_type pth, c, th, windex;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		size_type bw = fw / 2;
		size_type bh = fh / 2;
		size_type bd = fd / 2;

		size_type bbh, bbd;

		size_type size = fw * fh * fd;

		value_type *work  = new value_type[ size + 1 ];
		value_type *work1 = new value_type[ size + 1 ];
		value_type *work2 = new value_type[ size + 1 ];
		value_type *work3 = new value_type[ size + 1 ];
		value_type **sort = new value_type*[ fw ];

		for( k = thread_idz ; k < d ; k += thread_numz )
		{
			if( k < bd )
			{
				bbd = k + bd + 1;
			}
			else if( k + bd >= d )
			{
				bbd = d - k + bd + 1;
			}
			else
			{
				bbd = fd;
			}

			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				if( j < bh )
				{
					bbh = j + bh + 1;
				}
				else if( k + bd >= d )
				{
					bbh = h - j + bh + 1;
				}
				else
				{
					bbh = fh;
				}

				for( i = 0 ; i < fw ; i++ )
				{
					sort[ i ] = work + i * bbh * bbd;
				}

				i = 0;

				windex = 0;

				for( x = 0 ; x <= bw ; x++ )
				{
					windex = windex % fw;
					c = 0;
					for( z = k < bd ? 0 : k - bd ; z <= k + bd && z < d ; z++ )
					{
						for(y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
						{
							sort[ windex ][ c++ ] = in( x, y, z );
						}
					}
					windex++;
				}

				pth = ( bw + 1 ) * bbh * bbd;
				th = ( pth % 2 ) == 0 ? pth / 2 - 1 : pth / 2;
				out( i, j, k ) = static_cast< out_value_type >( nth_value( work, pth, th, work1, work2, work3 ) );

				for( i = 1 ; i < bw ; i++ )
				{
					ri = i + bw;
					pth = ( i + bw + 1 ) * bbh * bbd;
					windex = windex % fw;

					c = 0;
					for( z = k < bd ? 0 : k - bd ; z <= k + bd && z < d ; z++ )
					{
						for( y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
						{
							sort[ windex ][ c++ ] = in( i, y, z );
						}
					}

					th = ( pth % 2 ) == 0 ? pth / 2 - 1 : pth / 2;

					out( i, j, k ) = static_cast< out_value_type >( nth_value( work, pth, th, work1, work2, work3 ) );

					windex++;
				}

				pth = fw * bbh * bbd;
				th = ( pth % 2 ) == 0 ? pth / 2 - 1 : pth / 2;
				for( ; i + bw < w ; i++ )
				{
					ri = i + bw;
					windex = windex % fw;

					c = 0;
					for( z = k < bd ? 0 : k - bd ; z <= k + bd && z < d ; z++ )
					{
						for( y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
						{
							sort[ windex ][ c++ ] = in( ri, y, z );
						}
					}

					out( i, j, k ) = static_cast< out_value_type >( nth_value( work, pth, th, work1, work2, work3 ) );

					windex++;
				}

				for( ; i < w ; i++ )
				{
					windex = windex % fw;

					c = 0;
					for( z = k < bd ? 0 : k - bd ; z <= k + bd && z < d ; z++ )
					{
						for( y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
						{
							sort[ windex ][ c++ ] = 0;
						}
					}

					out( i, j, k ) = static_cast< out_value_type >( nth_value( work, pth, th, work1, work2, work3 ) );

					windex++;
				}
			}
		}

		delete [] work;
		delete [] work1;
		delete [] work2;
		delete [] work3;
		delete [] sort;
	}
}

// ���f�B�A���t�B���^�̃X���b�h����
namespace __median_filter_controller__
{
	template < class T, class Allocator >
	void get_min_max( const array< T, Allocator > &in, typename array< T, Allocator >::value_type &min, typename array< T, Allocator >::value_type &max )
	{
		min = max = in[ 0 ];
		for( typename array< T, Allocator >::size_type i = 0 ; i < in.size( ) ; i++ )
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
	}

	template < bool b >
	struct __median_filter__
	{
		template < class T1, class Allocator1, class T2, class Allocator2 >
		static void median_filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
							typename array< T1, Allocator1 >::size_type fw, typename array< T1, Allocator1 >::size_type fh, typename array< T1, Allocator1 >::size_type fd,
							typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num )
		{
			typedef typename array< T1, Allocator1 >::value_type value_type;
			value_type min = in[ 0 ];
			value_type max = in[ 0 ];
			get_min_max( in, min, max );
			__median_filter_with_histogram__::median_filter( in, out, fw, fh, fd, min, max, 0, 1, thread_id, thread_num );
		}

		template < class T1, class Allocator1, class T2, class Allocator2 >
		static void median_filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
							typename array2< T1, Allocator1 >::size_type fw, typename array2< T1, Allocator1 >::size_type fh, typename array2< T1, Allocator1 >::size_type fd,
							typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num )
		{
			typedef typename array2< T1, Allocator1 >::value_type value_type;
			value_type min = in[ 0 ];
			value_type max = in[ 0 ];
			get_min_max( in, min, max );
			__median_filter_with_histogram__::median_filter( in, out, fw, fh, fd, min, max, thread_id, thread_num, 0, 1 );
		}

		template < class T1, class Allocator1, class T2, class Allocator2 >
		static void median_filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
							typename array3< T1, Allocator1 >::size_type fw, typename array3< T1, Allocator1 >::size_type fh, typename array3< T1, Allocator1 >::size_type fd,
							typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num )
		{
			typedef typename array3< T1, Allocator1 >::value_type value_type;
			value_type min = in[ 0 ];
			value_type max = in[ 0 ];
			get_min_max( in, min, max );
			__median_filter_with_histogram__::median_filter( in, out, fw, fh, fd, min, max, 0, 1, thread_id, thread_num );
		}
	};

	// �q�X�g�O�������쐬�ł��Ȃ��ꍇ�̃��f�B�A���t�B���^
	// ���������_�Ȃ�
	template < >
	struct __median_filter__< false >
	{
		template < class T1, class Allocator1, class T2, class Allocator2 >
		static void median_filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
							typename array< T1, Allocator1 >::size_type fw, typename array< T1, Allocator1 >::size_type fh, typename array< T1, Allocator1 >::size_type fd,
							typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num )
		{
			__median_filter_divide_conquer__::median_filter( in, out, fw, fh, fd, 0, 1, thread_id, thread_num );
		}

		template < class T1, class Allocator1, class T2, class Allocator2 >
		static void median_filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
							typename array2< T1, Allocator1 >::size_type fw, typename array2< T1, Allocator1 >::size_type fh, typename array2< T1, Allocator1 >::size_type fd,
							typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num )
		{
			__median_filter_divide_conquer__::median_filter( in, out, fw, fh, fd, thread_id, thread_num, 0, 1 );
		}

		template < class T1, class Allocator1, class T2, class Allocator2 >
		static void median_filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
							typename array3< T1, Allocator1 >::size_type fw, typename array3< T1, Allocator1 >::size_type fh, typename array3< T1, Allocator1 >::size_type fd,
							typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num )
		{
			__median_filter_divide_conquer__::median_filter( in, out, fw, fh, fd, 0, 1, thread_id, thread_num );
		}
	};


	template < class T1, class T2 >
	class median_thread : public mist::thread_object< median_thread< T1, T2 > >
	{
	public:
		typedef mist::thread_object< median_thread< T1, T2 > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T1::size_type size_type;
		typedef typename T1::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// ���o�͗p�̉摜�ւ̃|�C���^
		const T1 *in_;
		T2 *out_;
		size_type fw_;
		size_type fh_;
		size_type fd_;

	public:
		void setup_parameters( const T1 &in, T2 &out, size_t fw, size_type fh, size_type fd, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			out_ = &out;
			fw_ = fw;
			fh_ = fh;
			fd_ = fd;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		const median_thread& operator =( const median_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				in_ = p.in_;
				out_ = p.out_;
				fw_ = p.fw_;
				fh_ = p.fh_;
				fd_ = p.fd_;
			}
			return( *this );
		}

		median_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), fw_( 3 ), fh_( 3 ), fd_( 3 )
		{
		}
		median_thread( const median_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
													in_( p.in_ ), out_( p.out_ ), fw_( p.fw_ ), fh_( p.fh_ ), fd_( p.fd_ )
		{
		}

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( const median_thread &p )
		{
			__median_filter__< is_integer< T1 >::value >::median_filter( *in_, *out_, fw_, fh_, fd_, thread_id_, thread_num_ );
			return( true );
		}
	};
}


template < class T1, class Allocator1, class T2, class Allocator2 >
void median( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, typename array< T1, Allocator1 >::size_type fw, typename array< T1, Allocator1 >::size_type thread_num = 0 )
{
	out.resize( in.size( ) );

	__median_filter_controller__::__median_filter__< is_integer< T1 >::value >::median_filter( in, out, fw, 1, 1, 0, 1 );
}

template < class T1, class Allocator1, class T2, class Allocator2 >
void median( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out, typename array1< T1, Allocator1 >::size_type fw, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
{
	out.resize( in.size( ) );
	out.reso1( in.reso1( ) );

	__median_filter_controller__::__median_filter__< is_integer< T1 >::value >::median_filter( in, out, fw, 1, 1, 0, 1 );
}

template < class T1, class Allocator1, class T2, class Allocator2 >
void median( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
				   typename array2< T1, Allocator1 >::size_type fw, typename array2< T1, Allocator1 >::size_type fh,
				   typename array2< T1, Allocator1 >::size_type thread_num = 0 )
{
	typedef typename array2< T1, Allocator1 >::size_type  size_type;
	typedef __median_filter_controller__::median_thread< array2< T1, Allocator1 >, array2< T2, Allocator2 > > median_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	median_thread *thread = new median_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, out, fw, fh, 1, i, thread_num );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].create_thread( );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].wait_thread( );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].close_thread( );
	}

	delete [] thread;
}

template < class T1, class Allocator1, class T2, class Allocator2 >
void median( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
				   typename array3< T1, Allocator1 >::size_type fw, typename array3< T1, Allocator1 >::size_type fh, typename array3< T1, Allocator1 >::size_type fd,
				   typename array3< T1, Allocator1 >::size_type thread_num = 0 )
{
	typedef typename array3< T1, Allocator1 >::size_type  size_type;
	typedef __median_filter_controller__::median_thread< array3< T1, Allocator1 >, array3< T2, Allocator2 > > median_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	median_thread *thread = new median_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, out, fw, fh, fd, i, thread_num );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].create_thread( );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].wait_thread( );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].close_thread( );
	}

	delete [] thread;
}



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_MEDIAN_FILTER__