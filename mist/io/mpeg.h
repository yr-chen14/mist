/// @file mist/io/mpeg.h
//!
//! @brief mpeg1, mpeg2�`���r�f�I�O���[�����o��
//!

#ifndef __INCLUDE_IO_MPEG_H__
#define __INCLUDE_IO_MPEG_H__



#include <avcodec.h>
#include <avformat.h>

#include <mist/mist.h>
#include <mist/config/color.h>
#include <iostream>



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



namespace __libavcodec__
{
	bool is_libavcodec_initialized = false;		///< @brief libavcodec�̏������t���O
}



//! @defgroup mpeg_group mpeg1, mpeg2�`���r�f�I�X�g���[�����o�͂̂��߂̃��C�u����
//!
//!
//! �{���C�u�����́Chttp://ffmpeg.sourceforge.net/index.php �ŊJ�����s���Ă��� ffmpeg-0.4.8 �� libavcodec, libavformat �𗘗p���Ă���
//! �G���R�[�h�ƃf�R�[�h�͂��̃��C�u������API��p���Ď�������Ă��邽�߁C�ʓr���C�u������p�ӂ���K�v������
//!
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/mpeg.h>
//! @endcode
//!
//!
//! @code �g�p��(mpeg�̃r�f�I�X�g���[������A��bmp�t�@�C�����쐬)
//! mist::array2< mist::rgb< unsigned char > > img;
//! mist::mpeg::input_video iv;
//! 
//! mist::mpeg::open( iv, "input.mpeg" );
//! mist::mpeg::dump_format( iv );
//! char f_name[ 256 ];
//! size_t i = 0;
//! while( mist::mpeg::read_frame( iv, img ) )
//! {
//!		sprintf( f_name, "tmp/frame%04d.bmp", i ++ );
//!		mist::write_bmp( img, f_name );
//! }
//! mist::mpeg::close( iv );
//! @endcode
//!
//!
//! @code �g�p��(�A��bmp����mpeg1video�X�g���[�����쐬)
//! mist::array2< mist::rgb< unsigned char > > img;
//! mist::mpeg::output_video ov;
//! 
//! // �p�����[�^�̕ύX�i�y���ӁI�z�X�g���[�����J���O�ɕύX���Ȃ���΂Ȃ�Ȃ��j
//! // ov.encode_buf_size( 524288 );	// �\���Ɋm�ۂ���
//! // ov.bit_rate( 1000000 );			// ���Ȃ��ƕi����������
//! ov.width( 320 );					// �摜�̃T�C�Y�ɍ��킹�ĕύX
//! ov.height( 240 );					// �摜�̃T�C�Y�ɍ��킹�ĕύX
//! // ov.frame_rate( 30 );				// �t���[�����[�g
//! // ov.frame_rate_base( 1 );			// 
//! // ov.gop_size( 12 );				// �����ƕi����������
//! // ov.max_b_frames( 2 );			// �����ƕi����������
//!
//! mist::mpeg::open( ov, "output.mpeg" );
//! mist::mpeg::dump_format( ov );
//! char f_name[ 256 ];
//! const size_t frame_num = 100;       // �A��bmp�t�@�C���̐�
//! for( size_t i = 0 ; i < frame_num ; i ++ )
//! {
//!		sprintf( f_name, "tmp/frame%04d.bmp", i );
//!		mist::read_bmp( img, f_name );
//!		mist::mpeg::write_frame( ov, img );
//! }
//! mist::mpeg::close( ov );
//! @endcode
//!
//!
//! @code �g�p��(�X�g���[�����̊e�t���[���̉摜�����t�B���^����)
//! mist::array2< mist::rgb< unsigned char > > in_img, out_img;
//! mist::mpeg::input_video iv;
//! mist::mpeg::output_video ov;
//! 
//! mist::mpeg::open( iv, "input.mpeg" );
//! mist::mpeg::copy_format( iv, ov );
//!
//! // �K�v�Ȃ�ov�̃p�����[�^�̕ύX�i�𑜓x�ϊ�����悤�ȏꍇ�ɂ̓t���[���̕��ƍ�����ύX�j
//!
//! mist::mpeg::open( ov, "output.mpeg" );
//! mist::mpeg::dump_format( iv );
//! mist::mpeg::dump_format( ov );
//! while( mist::mpeg::read_frame( iv, in_img ) )
//! {
//!		// in_img����͂���out_img���o�͂���悤�ȃt�B���^
//!		mist::gaussian( in_img, out_img );�@// ��
//!
//!		mist::mpeg::write_frame( ov, out_img );
//! }
//! mist::mpeg::close( iv );
//! mist::mpeg::close( ov );
//! @endcode
//!
//!
//! //! @code �g�p��(mpeg���̔C�ӂ̃t���[������mpeg1video�X�g���[�����쐬)
//! mist::array2< mist::rgb< unsigned char > > in_img, out_img;
//! mist::mpeg::input_video iv;
//! mist::mpeg::output_video ov;
//! 
//! mist::mpeg::open( iv, "input.mpeg" );
//! mist::mpeg::copy_format( iv, ov );
//!
//! // �K�v�Ȃ�ov�̃p�����[�^�̕ύX
//!
//! mist::mpeg::open( ov, "output.mpeg" );
//! mist::mpeg::dump_format( iv );
//! mist::mpeg::dump_format( ov );
//! mist::mpeg::skip_frame( iv, 60 );      // ����Ȃ��t���[���̓X�L�b�v
//! mist::mpeg::copy_frame( iv, ov, 240 ); // �K�v�ȃt���[���̂݃R�s�[
//! mist::mpeg::close( iv );
//! mist::mpeg::close( ov );
//! @endcode
//!
//!
//!  @{



/// @brief mpeg1,mpeg2�t�@�C������r�f�I�X�g���[���̓ǂݍ��݁Cmpeg1video�r�f�I�X�g���[����mpeg�t�@�C�������o��
namespace mpeg
{



	/// @brief mpeg�r�f�I���̓N���X
	//! 
	//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
	//!
	class input_video
	{

	private:

		AVFormatContext	*p_fctx_;				///< @brief mpeg�̃t�H�[�}�b�g���C�f�R�[�_��
		AVFrame			*p_frame_src_;			///< @brief �ǂݏo���ꂽ�t���[���摜�f�[�^
		AVFrame			*p_frame_rgb_;			///< @brief RGB�t�H�[�}�b�g�̃t���[���摜�f�[�^
		bool			is_open_;				///< @brief �r�f�I���J���Ă��邩�ǂ����̃t���O
		int				video_stream_index_;	///< @brief �r�f�I���w���X�g���[���ԍ�
		AVPacket		decoding_packet_;		///< @brief �f�R�[�h���̃p�P�b�g
		int				bytes_not_decoded_;		///< @brief �܂��f�R�[�h����Ă��Ȃ��f�[�^�̃o�C�g��
		uint8_t			*p_decoding_packet_;	///< @brief �f�R�[�h���̃p�P�b�g���̈ʒu���w���|�C���^

		/// @brief ������Z�q�̃I�[�o���[�h
		//! 
		//! ������֎~
		//! 
		input_video& operator =( const input_video &iv )
		{
		}

	public:

		/// @brief �R���X�g���N�^
		//! 
		//! �f�t�H���g�R���X�g���N�^
		//! 
		input_video( ) : p_fctx_( NULL ), p_frame_src_( NULL ), p_frame_rgb_( NULL ), is_open_( false ), video_stream_index_( -1 ), bytes_not_decoded_( 0 ), p_decoding_packet_( NULL ) 
		{
			if( !__libavcodec__::is_libavcodec_initialized )
			{
				av_register_all( );
				__libavcodec__::is_libavcodec_initialized = true;
			}
			decoding_packet_.data = NULL;
		}

		/// @brief �f�X�g���N�^
		//! 
		//!
		//!
		~input_video( )
		{ 
			close( );
		}

		/// @brief mpeg�t�@�C��������͗p�r�f�I�X�g���[�����J��
		//! 
		//! @param[in] filename �c ����mpeg�t�@�C��
		//! 
		bool open( const char *filename )
		{
			if( !is_open_ )
			{
				// Open video file
				if( av_open_input_file( &p_fctx_, filename, NULL, 0, NULL ) != 0 )
				{
					printf( "Couldn't open file %s\n", filename );
					return false;
				}
				// Retrieve stream information
				if( av_find_stream_info( p_fctx_ ) < 0 )
				{
					printf( "Couldn't find stream information\n" );
					return false;
				}
				// Find the first video stream
				video_stream_index_ = -1;
				for( int i = 0 ; i < p_fctx_->nb_streams ; i ++ )
				{
					if( p_fctx_->streams[ i ]->codec.codec_type == CODEC_TYPE_VIDEO )
					{
						video_stream_index_ = i;
						break;
					}
				}
				if( video_stream_index_ == -1 )
				{
					printf( "Didn't find a video stream\n" );
					return false;
				}
				// Find the decoder for the video stream
				AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
				AVCodec *p_codec = avcodec_find_decoder( p_cctx->codec_id );
				if( p_codec == NULL )
				{
					printf( "Codec not found\n" );
					return false;
				}
				// Inform the codec that we can handle truncated bitstreams -- i.e., bitstreams where frame boundaries can fall in the middle of packets
				if( p_codec->capabilities & CODEC_CAP_TRUNCATED )
				{
					p_cctx->flags |= CODEC_FLAG_TRUNCATED;
				}
				// Open codec
				if( avcodec_open( p_cctx, p_codec) < 0 )
				{
					printf( "Could not open codec\n" );
					return false;
				}
				// Hack to correct wrong frame rates that seem to be generated by some codecs
				if( p_cctx->frame_rate > 1000 && p_cctx->frame_rate_base == 1 )
				{
					p_cctx->frame_rate_base = 1000;
				}
				// Allocate video frame
				p_frame_src_ = avcodec_alloc_frame( );
				p_frame_rgb_ = avcodec_alloc_frame( );
				size_t size = avpicture_get_size( PIX_FMT_RGB24, p_cctx->width, p_cctx->height );
				uint8_t *rgb_buf = new uint8_t [ size ];
				// Assign appropriate parts of buffer to image planes in p_frame_rgb
				avpicture_fill( ( AVPicture * )p_frame_rgb_, rgb_buf, PIX_FMT_RGB24, p_cctx->width, p_cctx->height );

				is_open_ = true;
				return true;
			}
			return false;
		}

		/// @brief �r�f�I�X�g���[�������
		//! 
		//!
		//!
		bool close( )
		{
			if( is_open_ )
			{
				// Free the frames
				av_free( p_frame_src_->data[ 0 ] );
				av_free( p_frame_src_);
				delete [ ] p_frame_rgb_->data[ 0 ];
				av_free( p_frame_rgb_);
				// Close the codec
				avcodec_close( &p_fctx_->streams[ video_stream_index_ ]->codec );
				// Close the video file
				av_close_input_file( p_fctx_ );

				is_open_ = false;
				return true;
			}
			return false;
		}

		/// @brief �r�f�I�X�g���[�����J���Ă��邩�ǂ�����Ԃ�
		//! 
		//!
		//!
		bool is_open( ) const
		{
			return is_open_;
		}

		/// @brief �X�g���[���̃t�H�[�}�b�g��W���o�͂Ƀ_���v����
		//! 
		//!
		//!
		bool dump_format( ) const
		{
			if( is_open_ )
			{
				::dump_format( p_fctx_, 0, p_fctx_->filename, false );
				return true;
			}
			return false;
		}

		/// @brief ���݂̃t���[���ԍ��𓾂�
		//! 
		//!
		//!
		int frame_number( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_number;;
			}
			return -1;
		}

		/// @brief ���̃t���[���摜���f�R�[�h����array2�Ɋi�[(RGB)
		//! 
		//! @param[out] frame �c �t���[���摜���i�[���邽�߂̂Q�����z��
		//!
		bool read_frame( mist::array2< mist::rgb< unsigned char > > &frame )
		{
			if( is_open_ )
			{
				if( read( ) )
				{
					yuv_to_rgb( frame );
					return true;
				}
			}
			return false;
		}

		/// @brief ���̃t���[���摜���f�R�[�h����array2�Ɋi�[(�O���C�X�P�[��)
		//! 
		//! @param[out] frame �c �t���[���摜���i�[���邽�߂̂Q�����z��
		//!
		bool read_frame( mist::array2< unsigned char > &frame )
		{
			if( is_open_ )
			{
				if( read( ) )
				{
					yuv_to_gray( frame );
					return true;
				}
			}
			return false;
		}

		/// @brief �w�肵���񐔂����t���[���摜���X�L�b�v
		//! 
		//! @param[in] num �c �X�L�b�v����
		//!
		bool skip_frame( const size_t num = 1 )
		{
			if( is_open_ )
			{
				size_t i = 0;
				while( i < num )
				{
					if( !read( ) )
					{
						break;
					}
					i ++;
				}
				return true;
			}
			return false;
		}

		/// @brief �f�R�[�h���ꂽ�t���[���摜��array2�Ɋi�[(RGB)
		//! 
		//! @param[out] frame �c �t���[���摜���i�[���邽�߂̂Q�����z��
		//!
		void yuv_to_rgb( mist::array2< mist::rgb< unsigned char > > &frame )
		{
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			img_convert( ( AVPicture * )p_frame_rgb_, PIX_FMT_RGB24, ( AVPicture * )p_frame_src_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height );
			frame.resize( p_cctx->width, p_cctx->height );
			unsigned char *p = p_frame_rgb_->data[ 0 ];
			for( size_t i = 0 ; i < frame.size( ) ; i ++ )
			{
				frame[ i ].r = *( p ++ );
				frame[ i ].g = *( p ++ );
				frame[ i ].b = *( p ++ );
			}
		}

		/// @brief �f�R�[�h���ꂽ�t���[���摜��array2�Ɋi�[(�O���C�X�P�[��)
		//! 
		//! @param[out] frame �c �t���[���摜���i�[���邽�߂̂Q�����z��
		//!
		void yuv_to_gray( mist::array2< unsigned char > &frame )
		{
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			img_convert( ( AVPicture * )p_frame_rgb_, PIX_FMT_RGB24, ( AVPicture * )p_frame_src_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height );
			frame.resize( p_cctx->width, p_cctx->height );
			unsigned char *p = p_frame_rgb_->data[ 0 ];
			for( size_t i = 0 ; i < frame.size( ) ; i ++ )
			{
				frame[ i ] = ( unsigned char )( *p * 0.298912 + *( p + 1 ) * 0.586610 + *( p + 2 ) * 0.114478 );
				p += 3;
			}
		}

		/// @brief ���̃t���[���摜���f�R�[�h����
		//! 
		//! @param[out] p �c �f�R�[�h���ꂽ�t���[���摜�ւ̃|�C���^
		//! @param[out] pix_fmt �c ���̃t�H�[�}�b�g
		//!
		bool read( AVFrame *&p, PixelFormat &pix_fmt )
		{
			p = NULL;
			if( read( ) )
			{
				p = p_frame_src_;
				pix_fmt = p_fctx_->streams[ video_stream_index_ ]->codec.pix_fmt;
				return true;
			}
			return false;
		}

		/// @brief ���̃t���[���摜���f�R�[�h����
		//! 
		//!
		//!
		bool read( )
		{
			int bytes_decoded, frame_finished;
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			// Decode packets until we have decoded a complete frame
			while( true )
			{
				// Work on the current packet until we have decoded all of it
				while( bytes_not_decoded_ > 0 )
				{
					// Decode the next chunk of data
					bytes_decoded = avcodec_decode_video( p_cctx, p_frame_src_, &frame_finished, p_decoding_packet_, bytes_not_decoded_);						
					// Was there an error?
					if( bytes_decoded < 0 )
					{
						printf( "Error while decoding frame\n" );
						return false;
					}
					bytes_not_decoded_ -= bytes_decoded;
					p_decoding_packet_ += bytes_decoded;
					// Did we finish the current frame? Then we can return
					if( frame_finished != 0 )
					{
						return true;
					}
				}
				// Read the next packet, skipping all packets that aren't for video stream
				do
				{
					// Free old packet
					if( decoding_packet_.data != NULL )
					{
						av_free_packet( &decoding_packet_ );
					}
					// Read new packet
					if( av_read_packet( p_fctx_, &decoding_packet_ ) < 0 )
					{
						goto loop_exit;
					}				
				} while( decoding_packet_.stream_index != video_stream_index_ );
				bytes_not_decoded_ = decoding_packet_.size;
				p_decoding_packet_ = decoding_packet_.data;
			}
	loop_exit:
			// Decode the rest of the last frame
			bytes_decoded = avcodec_decode_video( p_cctx, p_frame_src_, &frame_finished, p_decoding_packet_, bytes_not_decoded_);	
			// Free last packet
			if( decoding_packet_.data != NULL )
			{
				av_free_packet( &decoding_packet_ );
			}
			return frame_finished != 0;
		}

		/// @brief mpeg�t�@�C�����𓾂�
		//! 
		//!
		//!
		char *filename( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->filename;
			}
			return NULL;
		}
		
		/// @brief codec_id�iavcodec.h�Q�Ɓj�𓾂�
		//! 
		//!
		//!
		CodecID codec_id( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.codec_id;
			}
			return CODEC_ID_NONE;
		}
	    
		/// @brief �r�b�g���[�g�𓾂�
		//! 
		//!
		//!
		int bit_rate( )  const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.bit_rate;
			}
			return 0;
		}
		
		/// @brief �t���[���̕��𓾂�
		//! 
		//!
		//!
		int width( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.width;
			}
			return 0;
		}
		
		/// @brief �t���[���̍����𓾂�
		//! 
		//!
		//!
		int height( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.height;
			}
			return 0;
		}
		
		/// @brief �t���[�����[�g�𓾂�
		//! 
		//!
		//!
		int frame_rate( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate;
			}
			return 0;
		}
		
		/// @brief �t���[�����[�g�x�[�X�𓾂�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		//! 
		//!
		//!
		int frame_rate_base( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate_base;
			}
			return 0;
		}
		
		/// @brief GOP�T�C�Y�𓾂�i���̖����̘A���t���[�����ɕK���P���ȏ�I�t���[�������݂���j
		//! 
		//!
		//!
		int gop_size( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.gop_size;
			}
			return 0;
		}
		
		/// @brief �ő�A��B�t���[�����𓾂�
		//! 
		//!
		//!
		int max_b_frames( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.max_b_frames;
			}
			return 0;
		}

	};



	/// @brief mpeg�r�f�I�o�̓N���X
	//! 
	//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
	//!
	class output_video
	{

	private:

		AVFormatContext	*p_fctx_;				///< @brief mpeg�̃t�H�[�}�b�g���C�f�R�[�_��
		AVFrame			*p_frame_dst_;			///< @brief �����o�����t���[���摜�f�[�^
		AVFrame			*p_frame_rgb_;			///< @brief RGB�t�H�[�}�b�g�̃t���[���摜�f�[�^
		bool			is_open_;				///< @brief �r�f�I���J���Ă��邩�ǂ����̃t���O
		const int		video_stream_index_;	///< @brief �r�f�I���w���X�g���[���ԍ�
		int				encode_buf_size_;		///< @brief �G���R�[�h�ŗp����o�b�t�@�̃T�C�Y
		uint8_t			*encode_buf_;			///< @brief �G���R�[�h�ŗp����o�b�t�@
		int				bit_rate_;				///< @brief �r�b�g���[�g
		int				width_;					///< @brief �t���[���摜�̕�
		int				height_;				///< @brief �t���[���摜�̍���
		int				frame_rate_;			///< @brief �t���[�����[�g
		int				frame_rate_base_;		///< @brief �t���[�����[�g�x�[�X�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		int				gop_size_;				///< @brief GOP�T�C�Y�i���̖����̘A���t���[�����ɕK���P���ȏ�I�t���[�������݂���j
		int				max_b_frames_;			///< @brief �ő�A��B�t���[����

		/// @brief ������Z�q�̃I�[�o���[�h
		//! 
		//! ������֎~
		//!
		output_video& operator =( const output_video &iv )
		{
		}

	public:

		/// @brief �R���X�g���N�^
		//! 
		//! @param[in] encode_buf_size �c 524288�i�f�t�H���g�l�j
		//! @param[in] bit_rate        �c 1000000�i�f�t�H���g�l�j
		//! @param[in] width           �c 160�i�f�t�H���g�l�j
		//! @param[in] height          �c 120�i�f�t�H���g�l�j
		//! @param[in] frame_rate      �c 30�i�f�t�H���g�l�j
		//! @param[in] frame_rate_base �c 1�i�f�t�H���g�l�j
		//! @param[in] gop_size        �c 12�i�f�t�H���g�l�j
		//! @param[in] max_b_frames    �c 2�i�f�t�H���g�l�j
		//!
		output_video( const size_t encode_buf_size = 524288, const size_t bit_rate = 1000000, const int width = 160, const int height = 120, const int frame_rate = 30, const int frame_rate_base = 1, const int gop_size = 12, const int max_b_frames = 2 )
			: p_fctx_( NULL ), p_frame_dst_( NULL ), p_frame_rgb_( NULL ), is_open_( false ), video_stream_index_( 0 ), encode_buf_size_( encode_buf_size ), bit_rate_( bit_rate ), width_( width ), height_( height ), frame_rate_( frame_rate ), frame_rate_base_( frame_rate_base ), gop_size_( gop_size ), max_b_frames_( max_b_frames )
		{
			if( !__libavcodec__::is_libavcodec_initialized )
			{
				av_register_all( );
				__libavcodec__::is_libavcodec_initialized = true;
			}
		}
		
		/// @brief �f�X�g���N�^
		//! 
		//!
		//!
		~output_video( )
		{
			close( );
		}

		/// @brief mpeg�t�@�C������o�͗p�r�f�I�X�g���[�����J��
		//! 
		//! @param[in] filename �c �o��mpeg�t�@�C��
		//! 
		bool open( const char *filename )
		{
			if( !is_open_ )
			{	
				// Allocate the output media context
				p_fctx_ = ( AVFormatContext * )av_mallocz( sizeof( AVFormatContext ) );
				if( !p_fctx_ ) {
					printf( "Memory error\n" );
					return false;
				}
				// Auto detect the output format from the name
				p_fctx_->oformat = guess_format( "mpeg", NULL, NULL );
				// Add the video stream  and initialize the codecs 
				AVStream *p_st= av_new_stream( p_fctx_, 0 );
				if( !p_st )
				{
					printf( "Could not alloc stream\n" );
					return false;
				}
				AVCodecContext *p_cctx = &p_st->codec;
				p_cctx->codec_id = ( CodecID )p_fctx_->oformat->video_codec;
				p_cctx->codec_type = CODEC_TYPE_VIDEO;
				p_cctx->bit_rate = bit_rate_;//ic->bit_rate;  
				p_cctx->width = width_;  
				p_cctx->height = height_;
				p_cctx->frame_rate = frame_rate_;  
				p_cctx->frame_rate_base = frame_rate_base_;
				p_cctx->gop_size = gop_size_;
				p_cctx->max_b_frames = max_b_frames_;
				p_fctx_->oformat->audio_codec = CODEC_ID_NONE;
				// Set the output parameters (must be done even if no parameters).
				if( av_set_parameters( p_fctx_, NULL ) < 0 ) {
					printf( "Invalid output format parameters\n" );
					return false;
				}
				// Find the video encoder
				AVCodec *p_codec = avcodec_find_encoder( p_cctx->codec_id );
				if(	!p_codec )
				{
					printf( "Codec not found\n" );
					return false;
				}
				// Open the codec
				if( avcodec_open( p_cctx, p_codec ) < 0) 
				{
					printf( "could not open codec\n" );
					return false;
				}
				// Allocate the encode buffer
				encode_buf_ = ( uint8_t * )malloc( encode_buf_size_ );
				// Allocate the frame buffers
				p_frame_dst_ = avcodec_alloc_frame( );
				size_t size = avpicture_get_size( PIX_FMT_YUV420P, p_cctx->width, p_cctx->height );
				uint8_t *buf_dst = new uint8_t [ size ];
				avpicture_fill( ( AVPicture * )p_frame_dst_, buf_dst, PIX_FMT_YUV420P, p_cctx->width, p_cctx->height );
				p_frame_rgb_ = avcodec_alloc_frame( );
				size = avpicture_get_size( PIX_FMT_RGB24, p_cctx->width, p_cctx->height );
				uint8_t *buf_rgb = new uint8_t [ size ];
				avpicture_fill( ( AVPicture * )p_frame_rgb_, buf_rgb, PIX_FMT_RGB24, p_cctx->width, p_cctx->height );
				// Open the output file, if needed
				sprintf( p_fctx_->filename, "%s", filename );
				if( !( p_fctx_->oformat->flags & AVFMT_NOFILE ) ) 
				{
					if( url_fopen( &p_fctx_->pb, filename, URL_WRONLY ) < 0 ) 
					{
						printf( "Could not open '%s'\n", filename );
						return false;
					}
				}
				// Write the stream header, if any
				av_write_header( p_fctx_ );

				is_open_ = true;
				return true;
			}
			return false;
		}

		/// @brief �r�f�I�X�g���[�������
		//! 
		//!
		//!
		bool close( )
		{
			if( is_open_ )
			{
				// Free the frame
				delete [ ] p_frame_dst_->data[ 0 ];
				av_free( p_frame_dst_ );
				p_frame_dst_ = NULL;
				// Write the frames remaining in encode buffer
				while( write( ) )
				{
				}
				// Write the trailer, if any
				av_write_trailer( p_fctx_ ); 
				// Free the frame
				delete [ ] p_frame_rgb_->data[ 0 ];
				av_free( p_frame_rgb_ );
				// Close each codec
				avcodec_close( &p_fctx_->streams[ video_stream_index_ ]->codec );
				// Free the buffer
				av_free( encode_buf_ );			
				// Free the streams
				av_freep( &p_fctx_->streams[ video_stream_index_ ] );
				// Free the stream
				av_free( p_fctx_ );

				is_open_ = false;
				return true;
			}
			return false;
		}

		/// @brief �r�f�I���J���Ă��邩�ǂ�����Ԃ�
		//! 
		//!
		//!
		bool is_open( ) const
		{
			return is_open_;
		}

		/// @brief �X�g���[���̃t�H�[�}�b�g��W���o�͂Ƀ_���v����
		//! 
		//!
		//!
		bool dump_format( ) const
		{
			if( is_open_ )
			{
				::dump_format( p_fctx_, 0, p_fctx_->filename, true );
				return true;
			}
			return false;
		}

		/// @brief ���݂̃t���[���ԍ��𓾂�
		//! 
		//!
		//!
		int frame_number( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_number;
			}
			return -1;
		}

		/// @brief array2�Ɋi�[���ꂽ�t���[���摜���G���R�[�h���ď�������(RGB)
		//! 
		//! @param[in] frame �c �t���[���摜
		//!
		bool write_frame( const mist::array2< mist::rgb< unsigned char > > &frame )
		{
			if( is_open_ )
			{
				rgb_to_yuv( frame );
				return( write( ) );
			}
			return false;
		}

		/// @brief array2�Ɋi�[���ꂽ�t���[���摜���G���R�[�h���ď�������(�O���C�X�P�[��)
		//! 
		//! @param[in] frame �c �t���[���摜
		//!
		bool write_frame( const mist::array2< unsigned char > &frame )
		{
			if( is_open_ )
			{
				gray_to_yuv( frame );
				return( write( ) );
			}
			return false;
		}

		/// @brief array2�Ɋi�[���ꂽ�t���[���摜���G���R�[�h�p�̃t���[���摜�ɕϊ�(RGB)
		//! 
		//! @param[in] frame �c �t���[���摜
		//!
		void rgb_to_yuv( const mist::array2< mist::rgb< unsigned char > > &frame )
		{
			unsigned char *p = p_frame_rgb_->data[ 0 ];
			for( size_t i = 0 ; i < frame.size( ) ; i ++ )
			{
				*( p ++ ) = frame[ i ].r;
				*( p ++ ) = frame[ i ].g;
				*( p ++ ) = frame[ i ].b;
			}
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			img_convert((AVPicture *)p_frame_dst_, p_cctx->pix_fmt/*PIX_FMT_YUV420P*/, (AVPicture*)p_frame_rgb_, PIX_FMT_RGB24, p_cctx->width, p_cctx->height);
		}

		/// @brief array2�Ɋi�[���ꂽ�t���[���摜���G���R�[�h�p�̃t���[���摜�ɕϊ�(�O���C�X�P�[��)
		//! 
		//! @param[in] frame �c �t���[���摜
		//!
		void gray_to_yuv( const mist::array2< unsigned char > &frame )
		{
			unsigned char *p = p_frame_rgb_->data[ 0 ];
			for( size_t i = 0 ; i < frame.size( ) ; i ++ )
			{
				*( p ++ ) = frame[ i ];
				*( p ++ ) = frame[ i ];
				*( p ++ ) = frame[ i ];
			}
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			img_convert((AVPicture *)p_frame_dst_, p_cctx->pix_fmt/*PIX_FMT_YUV420P*/, (AVPicture*)p_frame_rgb_, PIX_FMT_RGB24, p_cctx->width, p_cctx->height);
		}

		/// @brief �t���[���摜���G���R�[�h���ď�������
		//! 
		//! @param[in] p �c �t���[���摜�ւ̃|�C���^
		//! @param[in] pix_fmt �c ���̃t�H�[�}�b�g
		//!
		bool write( const AVFrame *p, const PixelFormat pix_fmt )
		{
			if( is_open_ )
			{
				AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
				img_convert((AVPicture *)p_frame_dst_, p_cctx->pix_fmt, (AVPicture*)p, pix_fmt, p_cctx->width, p_cctx->height);
				return write( );
			}
			return false;
		}

		/// @brief �t���[���摜���G���R�[�h���ď�������
		//! 
		//!
		//!
		bool write( )
		{
			AVStream *p_st = p_fctx_->streams[ video_stream_index_ ];
			// encode the image
			int bytes_encoded = avcodec_encode_video( &p_st->codec, encode_buf_, encode_buf_size_, p_frame_dst_ );
			// if zero size, it means the image was buffered
			if( bytes_encoded != 0 ) 
			{
				// write the compressed frame in the media file
				// XXX: in case of B frames, the pts is not yet valid
				if( av_write_frame( p_fctx_, p_st->index, encode_buf_, bytes_encoded ) != 0 )
				{
					printf( "Error while writing video frame\n" );
				}
			} 
			return bytes_encoded != 0;
		}

		/// @brief �G���R�[�h�o�b�t�@�T�C�Y��ݒ�
		//! 
		//! @param[in] �c �G���R�[�h�o�b�t�@�T�C�Y
		//!
		bool encode_buf_size( const int encode_buf_size )
		{
			if( !is_open_ )
			{
				encode_buf_size_ = encode_buf_size;
				return true;
			}
			return false;
		}

		/// @brief �r�b�g���[�g��ݒ�
		//! 
		//! @param[in] bit_rate �c �r�b�g���[�g
		//!
		bool bit_rate( const int bit_rate )
		{
			if( !is_open_ )
			{
				bit_rate_ = bit_rate;
				return true;
			}
			return false;
		}

		/// @brief �t���[���摜�̕���ݒ�(�Q�̔{��)
		//! 
		//! @param[in] width �c �t���[���摜�̕�
		//!
		bool width( int width )
		{
			if( !is_open_ )
			{
				if( width % 2 == 1 )
				{
					width ++;
				}
				width_ = width;
				return true;
			}
			return false;
		}

		/// @brief �t���[���摜�̍�����ݒ�(�Q�̔{��)
		//! 
		//! @param[in] height �c �t���[���摜�̍���
		//!
		bool height( int height )
		{
			if( !is_open_ )
			{
				if( height % 2 == 1 )
				{
					height ++;
				}
				height_ = height;
				return true;
			}
			return false;
		}

		/// @brief �t���[�����[�g��ݒ�
		//! 
		//! @param[in] frame_rate �c �t���[�����[t
		//!
		bool frame_rate( const int frame_rate )
		{
			if( !is_open_ )
			{
				frame_rate_ = frame_rate;
				return true;
			}
			return false;
		}

		/// @brief �t���[�����[�g�x�[�X��ݒ�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		//! 
		//! @param[in] frame_rate_base �c �t���[�����[�g�x�[�X
		//!
		bool frame_rate_base( const int frame_rate_base )
		{
			if( !is_open_ )
			{
				frame_rate_base_ = frame_rate_base;
				return true;
			}
			return false;
		}

		/// @brief GOP�T�C�Y��ݒ�i���̖����̘A���t���[�����ɕK���P���ȏ�I�t���[�������݂���j
		//! 
		//! @param[in] gop_size �c GOP�T�C�Y
		//!
		bool gop_size( const int gop_size )
		{
			if( !is_open_ )
			{
				gop_size_ = gop_size;
				return true;
			}
			return false;
		}

		/// @brief �ő�A��B�t���[������ݒ�
		//! 
		//! @param[in] max_b_frames �c �ő�A��B�t���[����
		//!
		bool max_b_frames( const int max_b_frames )
		{
			if( !is_open_ )
			{
				max_b_frames_ = max_b_frames;
				return true;
			}
			return false;
		}

		/// @brief mpeg�t�@�C�����𓾂�
		//! 
		//!
		//!
		char *filename( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->filename;
			}
			return NULL;
		}

		/// @brief codec_id�iavcodec.h�Q�Ɓj�𓾂�
		//! 
		//!
		//!
		CodecID codec_id( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.codec_id;
			}
			return CODEC_ID_NONE;
		}

		/// @brief �r�b�g���[�g�𓾂�
		//! 
		//!
		//!
		int bit_rate( )  const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.bit_rate;
			}
			return 0;
		}

		/// @brief �t���[���摜�̕��𓾂�
		//! 
		//!
		//!
		int width( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.width;
			}
			return 0;
		}

		/// @brief �t���[���摜�̍����𓾂�
		//! 
		//!
		//!
		int height( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.height;
			}
			return 0;
		}

		/// @brief �t���[�����[�g�𓾂�
		//! 
		//!
		//!
		int frame_rate( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate;
			}
			return 0;
		}

		/// @brief �t���[�����[�g�x�[�X�𓾂�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		//! 
		//!
		//!
		int frame_rate_base( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate_base;
			}
			return 0;
		}

		/// @brief GOP�T�C�Y�𓾂�i���̖����̘A���t���[�����ɕK���P���ȏ�I�t���[�������݂���j
		//! 
		//!
		//!
		int gop_size( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.gop_size;
			}
			return 0;
		}

		/// @brief �ő�A��B�t���[�����𓾂�
		//! 
		//!
		//!
		int max_b_frames( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.max_b_frames;
			}
			return 0;
		}

	};



	/// @brief �t�@�C������X�g���[�����J��
	//! 
	//! @param[in,out] av              �c �X�g���[��
	//! @param[in] filename             �c �t�@�C����
	//!
	template< typename Stream >
	inline bool open( Stream &av, const char *filename )
	{
		return av.open( filename );
	}

	/// @brief �X�g���[�������
	//! 
	//! @param[in,out]  av              �c �X�g���[��
	//!
	template< typename Stream >
	inline bool close( Stream &av )
	{
		return av.close( );
	}

	/// @brief �X�g���[�����J���Ă��邩�ǂ�����Ԃ�
	//! 
	//! @param[in]  av              �c �X�g���[��
	//!
	template< typename Stream >
	inline bool is_open( const Stream &av )
	{
		return av.is_open( );
	}

	/// @brief �X�g���[���̃t�H�[�}�b�g��W���o�͂Ƀ_���v����
	//! 
	//! @param[in]  av              �c �X�g���[��
	//!
	template< typename Stream >
	inline bool dump_format( const Stream &av )
	{
		return av.dump_format( );
	}

	/// @brief �r�f�I�X�g���[������t���[���摜��ǂݍ���
	//! 
	//! @param[in,out] video           �c �r�f�I�X�g���[��
	//! @param[out]    frame           �c �t���[���摜
	//!
	template< typename Stream, typename Frame >
	inline bool read_frame( Stream &video, Frame &frame )
	{
		return video.read_frame( frame );
	}

	/// @brief �r�f�I�X�g���[���Ƀt���[���摜����������
	//! 
	//! @param[in,out] video           �c �r�f�I�X�g���[��
	//! @param[in]     frame           �c �t���[���摜
	//!
	template< typename Stream, typename Image >
	inline bool write_frame( Stream &video, const Image &frame )
	{
		return video.write_frame( frame );
	}

	/// @brief �w�肵���񐔂����t���[���摜���X�L�b�v
	//! 
	//! @param[in,out] video         �c �r�f�I�X�g���[��
	//! @param[in]     num           �c �X�L�b�v��
	//!
	template< typename Stream >
	inline bool skip_frame( Stream &video, const size_t num = 1 )
	{
		return video.skip_frame( num );
	}

	/// @brief �w�肵���񐔂����r�f�I�X�g���[���ԂŃt���[���摜�̃R�s�[
	//! 
	//! @param[in,out] src         �c �R�s�[���r�f�I�X�g���[��
	//! @param[out]    dst         �c �R�s�[��r�f�I�X�g���[��
	//! @param[in]     num         �c �R�s�[��
	//!
	template< typename Stream1, typename Stream2 >
	inline bool copy_frame( Stream1 &src, Stream2 &dst, const size_t num = 1 )
	{
		AVFrame *p;
		PixelFormat pix_fmt;
		for( size_t i = 0 ; i < num ; i ++ )
		{
			if( src.read( p, pix_fmt ) )
			{
				if( !dst.write( p, pix_fmt ) )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	/// @brief �r�f�I�X�g���[���ԂŃt�H�[�}�b�g���̃R�s�[
	//! 
	//! @param[in,out] src         �c �R�s�[���r�f�I�X�g���[��
	//! @param[out]    dst         �c �R�s�[��r�f�I�X�g���[��
	//!
	template< typename Stream1, typename Stream2 >
	inline bool copy_format( const Stream1 &src, Stream2 &dst )
	{
		if( src.is_open( ) && !dst.is_open( ) )
		{
			dst.bit_rate(        src.bit_rate( ) );
			dst.width(           src.width( ) );
			dst.height(          src.height( ) );
			dst.frame_rate(      src.frame_rate( ) );
			dst.frame_rate_base( src.frame_rate_base( ) );
			dst.gop_size(        src.gop_size( ) );
			dst.max_b_frames(    src.max_b_frames( ) );
			return true;
		}
		return false;
	}

	/// @brief ���݂̃t���[���ԍ��𓾂�
	//! 
	//! @param[in] video         �c �r�f�I�X�g���[��
	//!
	template< typename Stream >
	inline int frame_number( const Stream &video )
	{
		return video.frame_number( );
	}



} // namespace mpeg



// mist���O��Ԃ̏I���
_MIST_END



#endif // __INCLUDE_IO_MPEG_H__