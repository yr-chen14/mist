#ifndef __INCLUDE_MIST_PNM__
#define __INCLUDE_MIST_PNM__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// �J���[�摜�̐ݒ��ǂݍ���
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif


#include <iostream>
#include <string>
#include <vector>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __pnm_controller__
{
	enum PNM_TYPE
	{
		UNKNOWN = 0,
		P1,
		P2,
		P3,
		P4,
		P5,
		P6,
	};

	template < class T, class Allocator >
	struct pnm_controller
	{
		typedef typename array2< T, Allocator >::size_type  size_type;
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;
		typedef typename pixel_converter::value_type value_type;

		static PNM_TYPE pnm_format( const std::string &str )
		{
			if( str == "P1" )
			{
				return( P1 );
			}
			else if( str == "P2" )
			{
				return( P2 );
			}
			else if( str == "P3" )
			{
				return( P3 );
			}
			else if( str == "P4" )
			{
				return( P4 );
			}
			else if( str == "P5" )
			{
				return( P5 );
			}
			else if( str == "P6" )
			{
				return( P6 );
			}
			else
			{
				return( UNKNOWN );
			}
		}

		static const unsigned char *get_line( const unsigned char *s, const unsigned char *e, std::string &line )
		{
			line = "";
			while( s < e )
			{
				if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
						s = s + 2;
						break;
					}
					s = s + 1;
					break;
				}
				else if( s[ 0 ] == '\n' )
				{
					s = s + 1;
					break;
				}
				line += *s;
				s++;
			}
			return( s > e ? e : s );
		}

		static size_type split_string( const std::string &line, const char ch, std::vector< std::string > &elements )
		{
			std::string str = "";
			size_type i = 0;
			elements.clear( );
			while( i < line.size( ) )
			{
				for( ; i < line.size( ) && line[ i ] == ch ; i++ );

				str = "";
				for( ; i < line.size( ) && line[ i ] != ch ; i++ )
				{
					str += line[ i ];
				}

				if( str != "" )
				{
					elements.push_back( str );
				}
			}
			return( elements.size( ) );
		}

		static bool convert_from_pnm_data( array2< T, Allocator > &image, const unsigned char *buff, size_type len )
		{
			// PNM�`���̃w�b�_����
			const unsigned char *p = buff;
			const unsigned char *e = buff + len;
			std::string line = "";
			std::vector< std::string > elements;

			p = get_line( p, e, line );
			PNM_TYPE pnm_type = pnm_format( line );
			if( pnm_type == UNKNOWN )
			{
				// ���T�|�[�g�̃r�b�g�}�b�v
				std::cerr << "This format is not supported currently!" << std::endl;
				return( false );
			}

			// �R�����g�s���΂�
			p = get_line( p, e, line );
			while( line[ 0 ] == '#' )
			{
				p = get_line( p, e, line );
			}

			// �摜�T�C�Y���擾����
			split_string( line, ' ', elements );
			if( elements.size( ) != 2 )
			{
				// ���T�|�[�g�̃r�b�g�}�b�v
				std::cerr << "Image size is unknown!" << std::endl;
				return( false );
			}

			size_type w = std::atoi( elements[ 0 ].c_str( ) );
			size_type h = std::atoi( elements[ 1 ].c_str( ) );
			image.resize( w, h );

			// �Z�W�̃��x�������擾����
			switch( pnm_type )
			{
			case P2:
			case P3:
			case P5:
			case P6:
				p = get_line( p, e, line );
				split_string( line, ' ', elements );
				if( elements.size( ) != 1 )
				{
					// ���T�|�[�g�̃r�b�g�}�b�v
					std::cerr << "Image size is unknown!" << std::endl;
					return( false );
				}
				break;

			default:
				break;
			}

			// ��f�f�[�^���擾����
			size_type i = 0;
			switch( pnm_type )
			{
			case P1:
			case P2:
				while( p < e )
				{
					p = get_line( p, e, line );
					split_string( line, ' ', elements );
					for( size_type j = 0 ; j < elements.size( ) ; j++ )
					{
						image[ i++ ] = std::atoi( elements[ j ].c_str( ) );
					}
				}
				break;

			case P3:
				while( p < e )
				{
					p = get_line( p, e, line );
					split_string( line, ' ', elements );
					for( size_type j = 0 ; j < elements.size( ) ; j += 3 )
					{
						value_type r = static_cast< value_type >( std::atoi( elements[ j + 0 ].c_str( ) ) );
						value_type g = static_cast< value_type >( std::atoi( elements[ j + 1 ].c_str( ) ) );
						value_type b = static_cast< value_type >( std::atoi( elements[ j + 2 ].c_str( ) ) );
						image[ i++ ] = pixel_converter::convert_to( r, g, b );
					}
				}
				break;

			case P4:
			case P5:
				while( p < e )
				{
					image[ i++ ] = *p++;
				}
				break;

			case P6:
				while( p < e )
				{
					value_type r = static_cast< value_type >( p[ 0 ] );
					value_type g = static_cast< value_type >( p[ 1 ] );
					value_type b = static_cast< value_type >( p[ 2 ] );
					image[ i++ ] = pixel_converter::convert_to( r, g, b );
					p += 3;
				}
				break;

			default:
				return( false );
			}

			return( true );
		}

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			typedef typename array2< T, Allocator >::size_type size_type;

			size_type filesize;
			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "rb" ) ) == NULL ) return( false );
			// �t�@�C���T�C�Y���擾
			fseek( fp, 0, SEEK_END );
			filesize = ftell( fp );
			fseek( fp, 0, SEEK_SET );

			unsigned char *buff = new unsigned char[ filesize + 1 ];
			unsigned char *pointer = buff;
			size_type read_size = 0;
			while( feof( fp ) == 0 )
			{
				read_size = fread( pointer, sizeof( unsigned char ), 1024, fp );
				if( read_size < 1024 )
				{
					break;
				}
				pointer += read_size;
			}
			fclose( fp );

			bool ret = convert_from_pnm_data( image, buff, filesize );
			delete [] buff;
			return( ret );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, PNM_TYPE pnm_type )
		{
			typedef typename array2< T, Allocator >::size_type size_type;

			if( pnm_type == UNKNOWN )
			{
				std::cerr << "This format is not supported currently!" << std::endl;
				return( false );
			}

			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				return( false );
			}

			// �w�b�_������������
			fprintf( fp, "P%1d\n", pnm_type );
			fprintf( fp, "# Created by MIST\n", pnm_type );
			fprintf( fp, "%d %d\n", static_cast< int >( image.width( ) ), static_cast< int >( image.height( ) ) );

			color_type max = image[ 0 ];
			size_type i, j;
			for( i = 1 ; i < image.size( ) ; i++ )
			{
				max = max > image[ i ] ? max : image[ i ];
			}

			switch( pnm_type )
			{
			case P2:
			case P3:
			case P5:
			case P6:
				fprintf( fp, "%d\n", static_cast< int >( pixel_converter::convert_from( max ).get_value( ) ) );
				break;

			default:
				break;
			}


			// ��f�f�[�^�������o��
			switch( pnm_type )
			{
			case P1:
			case P2:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						fprintf( fp, "%d ", static_cast< int >( pixel_converter::convert_from( image( i, j ) ).get_value( ) ) );
					}
					fputc( '\n', fp );
				}
				break;

			case P3:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						color_type c = pixel_converter::convert_from( image( i, j ) );
						fprintf( fp, "%d ", static_cast< int >( c.r ) );
						fprintf( fp, "%d ", static_cast< int >( c.g ) );
						fprintf( fp, "%d ", static_cast< int >( c.b ) );
					}
					fputc( '\n', fp );
				}
				break;

			case P4:
			case P5:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						fprintf( fp, "%c", static_cast< unsigned char >( pixel_converter::convert_from( image( i, j ) ).get_value( ) ) );
					}
				}
				break;

			case P6:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						color_type c = pixel_converter::convert_from( image( i, j ) );
						fprintf( fp, "%c", static_cast< unsigned char >( c.r ) );
						fprintf( fp, "%c", static_cast< unsigned char >( c.g ) );
						fprintf( fp, "%c", static_cast< unsigned char >( c.b ) );
					}
				}
				break;

			default:
				fclose( fp );
				return( false );
			}

			fclose( fp );

			return( true );
		}
	};
}


template < class T, class Allocator >
bool read_pnm( array2< T, Allocator > &image, const std::string &filename )
{
	return( __pnm_controller__::pnm_controller< T, Allocator >::read( image, filename ) );
}

template < class T, class Allocator >
bool write_pnm( array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::size_type pnm_type = 6 )
{
	if( pnm_type > 6 ) 
	{
		std::cerr << "This format is not supported currently!" << std::endl;
		return( false );
	}
	return( __pnm_controller__::pnm_controller< T, Allocator >::write( image, filename, static_cast< __pnm_controller__::PNM_TYPE >( pnm_type ) ) );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_PNM__