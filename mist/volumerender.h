/// @file mist/volumerender.h
//!
//! @brief �o�ߎ��Ԃ��v�Z����N���X
//!
#ifndef __INCLUDE_MIST_VOLUMERENDER__
#define __INCLUDE_MIST_VOLUMERENDER__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "limits.h"
#endif


#ifndef __INCLUDE_MIST_THREAD__
#include "thread.h"
#endif


#include <vector>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN




//! @defgroup visualization_group ����
//!
//!  @{


//! @addtogroup volumerendering_group �{�����[�������_�����O
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/volumerender.h>
//! @endcode
//!
//! @{


namespace volumerender
{
	template < class T >
	struct attribute
	{
		typedef T value_type;

		value_type pixel;
		double alpha;
		bool   has_alpha;

		attribute( ) : pixel( ), alpha( 0 ), has_alpha( false ){ }

		//template < class TT >
		//attribute( const attribute &a ) : pixel( a.pixel ), alpha( a.alpha ), has_alpha( a.has_alpha ){ }

		attribute( const attribute &a ) : pixel( a.pixel ), alpha( a.alpha ), has_alpha( a.has_alpha ){ }
	
	};

	template < class T >
	class attribute_table : protected std::vector< attribute< T > >
	{
	public:
		typedef std::vector< attribute< T > > base;
		typedef attribute< T > attribute_type;
		typedef typename base::allocator_type allocator_type;	///< @brief MIST�R���e�i�����p����A���P�[�^�^
		typedef typename base::reference reference;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDmist::array< data > �̏ꍇ�Cdata & �ƂȂ�
		typedef typename base::const_reference const_reference;	///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::array< data > �̏ꍇ�Cconst data & �ƂȂ�
		typedef typename base::value_type value_type;			///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
		typedef typename base::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef typename base::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
		typedef typename base::pointer pointer;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cdata * �ƂȂ�
		typedef typename base::const_pointer const_pointer;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cconst data * �ƂȂ�

		typedef T pixel_type;

	protected:
		difference_type sindex_;
		difference_type eindex_;
		difference_type	zero_index_;

	public:
		void append( const pixel_type &pixel, difference_type vs, difference_type ve, double as, double ae )
		{
			if( base::empty( ) )
			{
				return;
			}
			else if( vs > ve )
			{
				difference_type tmp = vs;
				vs = ve;
				ve = vs;
			}
			vs = vs < sindex_ ? sindex_: vs;
			vs = vs > eindex_ ? eindex_: vs;
			ve = ve < sindex_ ? sindex_: ve;
			ve = ve > eindex_ ? eindex_: ve;

			double step = ( ae - as ) / static_cast< double >( ve - vs + 1 );
			for( difference_type i = vs ; i <= ve ; i++ )
			{
				value_type &p = at( i );
				p.pixel = pixel;
				p.alpha = as + static_cast< double >( i - vs ) * step;
				p.has_alpha = p.alpha > 0.0;
			}
		}

	public:
		value_type &operator []( difference_type index )
		{
			return( base::operator []( zero_index_ + index ) );
		}

		const value_type &operator []( difference_type index ) const
		{
			return( base::operator []( zero_index_ + index ) );
		}

		value_type &at( difference_type index )
		{
			return( base::at( zero_index_ + index ) );
		}

		const value_type &at( difference_type index ) const
		{
			return( base::at( zero_index_ + index ) );
		}

		bool has_alpha( difference_type index ) const
		{
			return( operator []( index ).has_alpha );
		}

		const attribute_table &operator =( const attribute_table &a )
		{
			if( &a != this )
			{
				base::operator =( a );
				sindex_ = a.sindex_;
				eindex_ = a.eindex_;
				zero_index_ = a.zero_index_;
			}
			return( *this );
		}


	public:
		void create( difference_type si, difference_type ei )
		{
			sindex_ = si;
			eindex_ = ei;

			base::resize( ei - si + 1 );
			zero_index_ = - si;
		}

		void clear( )
		{
			base::clear( );
			sindex_ = 0;
			eindex_ = -1;
			zero_index_ = 0;
		}

		attribute_table( ) : sindex_( 0 ), eindex_( -1 ), zero_index_( NULL ){ }

		attribute_table( difference_type si, difference_type ei ) : base( ei - si + 1 ), sindex_( si ), eindex_( ei ), zero_index_( NULL )
		{
			zero_pointer_ = &at( 0 ) - si;
		}

		attribute_table( const attribute_table &a ) : base( a ), sindex_( a.sindex_ ), eindex_( a.eindex_ ), zero_index_( a.zero_index_ )
		{
		}
	};

	struct boundingbox
	{
	public:
		double a;
		double b;
		double c;
		double d;

		void clear( )
		{
			a = 0;
			b = 0;
			c = 0;
			d = 0;
		}

		boundingbox( ) : a( 0 ), b( 0 ), c( 0 ), d( 0 ){ }
		boundingbox( double aa, double bb, double cc, double dd ) : a( aa ), b( bb ), c( cc ), d( dd ){ }
	};


	template < class T >
	inline bool check_intersection( vector3< T > &p1, vector3< T > &p2, const boundingbox &box )
	{
		typedef vector3< T > vector_type;
		typedef typename vector3< T >::value_type value_type;

		vector_type n( box.a, box.b, box.c );

		value_type d1 = p1.inner( n );
		value_type d2 = p2.inner( n );
		bool c1 = box.d + d1 >= 0;
		bool c2 = box.d + d2 >= 0;

		if( c1 && c2 )
		{
			return( true );
		}
		else if( !c1 && !c2 )
		{
			return( false );
		}
		vector_type v1 = p1;
		vector_type v2 = p2;
		if( !c1 )
		{
			p1 = v2 + ( v1 - v2 ) * ( ( box.d + d2 ) / ( d2 - d1 ) );
		}
		if( !c2 )
		{
			p2 = v1 + ( v2 - v1 ) * ( ( box.d + d1 ) / ( d1 - d2 ) );
		}
		return( true );
	}

	template < class T >
	struct parameter
	{
		typedef vector3< T > vector_type;
		typedef typename vector3< T >::value_type value_type;

		vector_type	pos;
		vector_type	dir;
		vector_type	up;
		vector_type	offset;
		value_type	fovy;
		value_type	ambient_ratio;
		value_type	diffuse_ratio;
		value_type	light_attenuation;
		value_type	sampling_step;
		value_type	termination;
		bool		specular;

		boundingbox box[ 6 ];
	};
}


/// @}
// �{�����[�������_�����O�O���[�v�̏I���

/// @}
//  �����O���[�v�̏I���



// �l��ԃ^�C�v�̃{�����[�������_�����O
namespace value_interpolation
{
	template < class Array1, class Array2, class T1, class T2 >
		bool volumerendering( const Array1 &in, Array2 &out, const volumerender::parameter< T1 > &p, const volumerender::attribute_table< T2 > &table,
		typename Array1::size_type thread_id, typename Array1::size_type thread_num )
	{
		typedef volumerender::parameter< T1 >::vector_type vector_type;
		typedef volumerender::attribute_table< T2 >::attribute_type attribute_type;
		typedef volumerender::attribute_table< T2 >::pixel_type pixel_type;
		typedef Array1::size_type size_type;
		typedef Array1::difference_type difference_type;
		typedef Array1::value_type value_type;
		typedef Array1::const_pointer const_pointer;
		typedef Array2::value_type out_value_type;

		vector_type pos = p.pos;
		vector_type dir = p.dir;
		vector_type up = p.up;
		vector_type offset = p.offset;
		double fovy = p.fovy;
		double ambient_ratio = p.ambient_ratio;
		double diffuse_ratio = p.diffuse_ratio;
		bool specular = p.specular;
		const volumerender::boundingbox *box = p.box;
		double lightAtten = p.light_attenuation;
		double sampling_step = p.sampling_step;
		double termination = p.termination;

		const size_type w = in.width( );
		const size_type h = in.height( );
		const size_type d = in.depth( );

		const size_type image_width  = out.width( );
		const size_type image_height = out.height( );

		difference_type d0, d1, d2, d3, d4, d5, d6, d7, _1, _2, _3;
		{
			difference_type cx = in.width( ) / 2;
			difference_type cy = in.height( ) / 2;
			difference_type cz = in.depth( ) / 2;
			const_pointer ppp = &in( cx, cy, cz );
			d0 = 0;
			d1 = &in( cx    , cy + 1, cz     ) - ppp;
			d2 = &in( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &in( cx + 1, cy    , cz     ) - ppp;
			d4 = &in( cx    , cy    , cz + 1 ) - ppp;
			d5 = &in( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &in( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &in( cx + 1, cy    , cz + 1 ) - ppp;
			_1 = &in( cx + 1, cy    , cz     ) - ppp;
			_2 = &in( cx    , cy + 1, cz     ) - ppp;
			_3 = &in( cx    , cy    , cz + 1 ) - ppp;
		}

		// �X���C�X���W�n�̎��������[���h�ƍl����
		vector_type ray, yoko, light, slight, slightstep, slightstep2, spos;
		vector_type normal, n1, n2, n3, n4, n5, n6, n7, n8;
		vector_type casting_start, casting_end;

		double alpha, lAtten = 1.0, spec;

		const double pai = 3.1415926535897932384626433832795;
		double focal = ( static_cast< double >( image_height ) / 2.0 ) / std::tan( fovy * pai / 180.0 / 2.0 );

		double cx = static_cast< double >( image_width ) / 2.0;
		double cy = static_cast< double >( image_height ) / 2.0;
		double ax = in.reso1( );
		double ay = in.reso2( );
		double az = in.reso3( );

		yoko = ( dir * up ).unit( );

		if( out.reso1( ) < out.reso2( ) )
		{
			yoko *= out.reso1( ) / out.reso2( );
		}
		else
		{
			up *= out.reso2( ) / out.reso1( );
		}

		double max_distance = pos.length( ) + std::sqrt( static_cast< double >( w * w + h * h + d * d ) );

		for( size_type j = thread_id ; j < image_height ; j += thread_num )
		{
			for( size_type i = 0 ; i < image_width ; i++ )
			{
				// ���e�ʏ�̓_���J�������W�n�ɕϊ�
				vector_type Pos( static_cast< double >( i ) - cx, cy - static_cast< double >( j ), focal );

				// ���C�������J�������W�n���烏�[���h���W�n�ɕϊ�
				ray = ( yoko * Pos.x + up * Pos.y + dir * Pos.z ).unit( );

				pixel_type add_intensity;
				double add_opacity = 1;
				double old_opacity = 0;

				casting_start = pos;
				casting_end = pos + ray * max_distance;

				// ���̂Ƃ̏Փ˔���
				if( volumerender::check_intersection( casting_start, casting_end, box[ 0 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 1 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 2 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 3 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 4 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 5 ] ) )
				{
					// ���[���h���W�n�i����j����X���C�X���W�n�i�E��j�ɕϊ�
					// �ȍ~�́C�S�ăX���C�X���W�n�Ōv�Z����
					casting_start.x = ( casting_start.x + offset.x ) / ax;
					casting_start.y = ( -casting_start.y + offset.y ) / ay;
					casting_start.z = ( casting_start.z + offset.z ) / az;
					casting_end.x   = ( casting_end.x   + offset.x ) / ax;
					casting_end.y   = ( -casting_end.y   + offset.y ) / ay;
					casting_end.z   = ( casting_end.z   + offset.z ) / az;

					spos = casting_start;
					ray = ( casting_end - casting_start ).unit( );
					vector_type ray_step = ray * sampling_step;

					difference_type n = static_cast< difference_type  >( ( casting_end - casting_start ).length( ) / sampling_step );
					difference_type l = 0;

					while( l < n )
					{
						difference_type si = static_cast< difference_type >( spos.x );
						difference_type sj = static_cast< difference_type >( spos.y );
						difference_type sk = static_cast< difference_type >( spos.z );

						const_pointer p = &in( si, sj, sk );

						// ��f�l�ɑΉ�����F�ƕs�����x���擾
						if( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
							table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
							table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) )
						{
							if( l > 0 )
							{
								spos -= ray_step;
								l--;
							}
							break;
						}

						l++;
						spos += ray_step;
					}

					while( l < n )
					{
						difference_type si = static_cast< difference_type >( spos.x );
						difference_type sj = static_cast< difference_type >( spos.y );
						difference_type sk = static_cast< difference_type >( spos.z );

						double xx = spos.x - si;
						double yy = spos.y - sj;
						double zz = spos.z - sk;

						const_pointer p = &in( si, sj, sk );

						// CT�l�ɑΉ�����F�ƕs�����x���擾
						double ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
						ct = ct + ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;

						const attribute_type &oc = table[ static_cast< difference_type >( ct ) ];

						// ���̈ʒu�ɂ����镨�̂������̏ꍇ�͎��̃X�e�b�v�ֈڍs����
						if( !oc.has_alpha )
						{
							spos += ray_step;
							l++;
							continue;
						}

						const_pointer p1 = p;
						const_pointer p2 = p1 + d1;
						const_pointer p3 = p1 + d2;
						const_pointer p4 = p1 + d3;
						const_pointer p5 = p1 + d4;
						const_pointer p6 = p1 + d5;
						const_pointer p7 = p1 + d6;
						const_pointer p8 = p1 + d7;

						n1.x = p4[  0  ] - p1[ -_1 ];
						n1.y = p1[ -_2 ] - p2[  0  ];
						n1.z = p5[  0  ] - p1[ -_3 ];
						n2.x = p3[  0  ] - p2[ -_1 ];
						n2.y = p1[  0  ] - p2[  _2 ];
						n2.z = p6[  0  ] - p2[ -_3 ];
						n3.x = p3[  _1 ] - p2[  0  ];
						n3.y = p4[  0  ] - p3[  _2 ];
						n3.z = p7[  0  ] - p3[ -_3 ];
						n4.x = p4[  _1 ] - p1[  0  ];
						n4.y = p4[ -_2 ] - p3[  0  ];
						n4.z = p8[  0  ] - p4[ -_3 ];
						n5.x = p8[  0  ] - p5[ -_1 ];
						n5.y = p5[ -_2 ] - p6[  0  ];
						n5.z = p5[  _3 ] - p1[  0  ];
						n6.x = p7[  0  ] - p6[ -_1 ];
						n6.y = p5[  0  ] - p6[  _2 ];
						n6.z = p6[  _3 ] - p2[  0  ];
						n7.x = p7[  _1 ] - p6[  0  ];
						n7.y = p8[  0  ] - p7[  _2 ];
						n7.z = p7[  _3 ] - p3[  0  ];
						n8.x = p8[  _1 ] - p5[  0  ];
						n8.y = p8[ -_2 ] - p7[  0  ];
						n8.z = p8[  _3 ] - p4[  0  ];

						normal = ( n1 + ( n4 - n1 ) * xx ) + ( n2 - n1 + ( n1 - n2 + n3 - n4 ) * xx ) * yy;
						normal = normal + ( ( n5 + ( n8 - n5 ) * xx ) + ( n6 - n5 + ( n5 - n6 + n7 - n8 ) * xx ) * yy - normal ) * zz;
						normal.x /=  ax;
						normal.y /=  -ay;
						normal.z /=  az;
						double len = std::sqrt( normal.x * normal.x + normal.y * normal.y + normal.z * normal.z ) + 1.0e-10;
						normal.x /=  len;
						normal.y /=  len;
						normal.z /=  len;

						lAtten = lightAtten > 0.0 ? 1.0 / ( 1.0 + lightAtten * ( ( ray - pos ).inner( ray - pos ) ) ) : 1.0;

						double c = ray.inner( normal );
						c = c < 0.0 ? -c : c;

						if( !specular )
						{
							spec = 0.0;
						}
						else
						{
							spec = 2.0 * c * c - 1.0;

							if( spec < 0.0 )
							{
								spec = 0;
							}
							else
							{
								spec *= spec; //^2
								spec *= spec; //^4
								spec *= spec; //^8
								spec *= spec; //^16
								spec *= spec; //^32
								spec *= spec; //^64
								//spec *= spec; //^128
							}
						}

						c = c * diffuse_ratio + ambient_ratio;

						alpha = oc.alpha * sampling_step;
						add_intensity += alpha * add_opacity * ( oc.pixel * c + spec ) * lAtten;
						add_opacity *= ( 1 - alpha );

						if( add_opacity < termination )
						{
							break;
						}
						spos += ray_step;
						l++;
					}
					out( i, j ) = mist::limits_0_255( add_intensity );
				}
				else
				{
					out( i, j ) = 0;
				}
			}
		}
		return( true );
	}
}


// �{�����[�������_�����O�̃X���b�h����
namespace __volumerendering_controller__
{
	template < class Array1, class Array2, class T1, class T2 >
	class volumerendering_thread : public mist::thread< volumerendering_thread< Array1, Array2, T1, T2 > >
	{
	public:
		typedef mist::thread< volumerendering_thread< Array1, Array2, T1, T2 > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// ���o�͗p�̉摜�ւ̃|�C���^
		const Array1 *in_;
		Array2 *out_;
		const volumerender::parameter< T1 > *param_;
		const volumerender::attribute_table< T2 > *table_;

	public:
		void setup_parameters( const Array1 &in, Array2 &out, const volumerender::parameter< T1 > &p, const volumerender::attribute_table< T2 > &t, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			out_ = &out;
			param_ = &p;
			table_ = &t;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		volumerendering_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), param_( NULL ), table_( NULL )
		{
		}
		volumerendering_thread( const volumerendering_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
																		in_( p.in_ ), out_( p.out_ ), param_( p.param_ ), table_( p.table_ )
		{
		}

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( )
		{
			value_interpolation::volumerendering( *in_, *out_, *param_, *table_, thread_id_, thread_num_ );
			return( true );
		}
	};
}



//! @defgroup visualization_group
//!
//!  @{


//! @addtogroup volumerendering_group
//!
//! @{



/// @brief �{�����[�������_�����O
//! 
//! @attention ���͂Əo�͂́C�ʂ�MIST�R���e�i�I�u�W�F�N�g�łȂ��Ă͂Ȃ�Ȃ�
//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//!
//! @param[in]  in         �c ���͉摜
//! @param[out] out        �c �o�͉摜
//! @param[in]  param      �c �{�����[�������_�����O�̃p�����[�^
//! @param[in]  table      �c �{�����[�������_�����O�̐F�|�l�e�[�u��
//! @param[in]  thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �{�����[�������_�����O�ɐ���
//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ
//! 
template < class Array1, class Array2, class T1, class T2 >
bool volumerendering( const Array1 &in, Array2 &out, const volumerender::parameter< T1 > &param, const volumerender::attribute_table< T2 > &table, typename Array1::size_type thread_num = 0 )
{
	if( is_same_object( in, out ) || in.empty( ) )
	{
		return( false );
	}

	typedef typename Array1::size_type size_type;
	typedef __volumerendering_controller__::volumerendering_thread< Array1, Array2, T1, T2 > volumerendering_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	volumerendering_thread *thread = new volumerendering_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, out, param, table, i, thread_num );
	}

	// �X���b�h�����s���āC�I���܂őҋ@����
	do_threads_( thread, thread_num );

	delete [] thread;
	
	return( true );
}


/// @}
// �{�����[�������_�����O�O���[�v�̏I���

/// @}
//  �����O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_VOLUMERENDER__