#include "gentech_capture.h"

using namespace gentech;

CCapture::CCapture()
{
        m_pFormatCtx = NULL;
        m_videoStreamIndex = -1;
        m_pVideoCodecCtx = NULL;
	m_pVideoFrame = NULL;

        av_register_all();
	avformat_network_init();
}

CCapture::~CCapture()
{
        close();
}

bool CCapture::open(const char* pFilePath)
{
        int result = 0;
        result = avformat_open_input(&m_pFormatCtx, pFilePath, NULL, NULL);
        if (result != 0) {
                printf("open file: %s format contex failed.\n", pFilePath);
                close(); return false;
        }
        // Read packets of a media file to get stream information,
        // such as codec type, codec id, and so on..
        result = avformat_find_stream_info(m_pFormatCtx, NULL);
        if (result < 0) {
                printf("find file: %s stream info failed.\n", pFilePath);
		close(); return false;
        }
        for (unsigned i = 0; i < m_pFormatCtx->nb_streams && i < 2; ++i) {
                if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
                        m_videoStreamIndex = i;
			break;
                }
	}
        if (m_videoStreamIndex == -1) {
                printf("file: %s, can not find video stream or audio stream.\n", pFilePath);
		close(); return false;
        }
        if (m_videoStreamIndex != -1) {
                m_pVideoCodecCtx =
                        m_pFormatCtx->streams[m_videoStreamIndex]->codec;
                // check if there is a supported decoder
                AVCodec *pcodec = avcodec_find_decoder(m_pVideoCodecCtx->codec_id);
               if (!pcodec) {
                        printf("file: %s, can not find supported decoder.\n", pFilePath);
                        close(); return false;
                }
                // Initialize the AVCodecContext to use the given AVCodec
                int result = avcodec_open2(m_pVideoCodecCtx, pcodec, NULL);
                if (result != 0) {
                        printf("can not open the codec for file: %s.\n", pFilePath);
                        close(); return false;
                }
        }
	m_pVideoFrame = av_frame_alloc();
	if (!m_pVideoFrame) {
		printf("can not allocate video frame.\n");
		close(); return false;
	}
	return true;
}

bool CCapture::operator >> (CQueue *pVideoQueue)
{
        AVPacket packet;
	av_init_packet(&packet);
	int videoFrameFinished = 0;

	while (av_read_frame(m_pFormatCtx, &packet) >= 0) {
		if (packet.stream_index == m_videoStreamIndex && pVideoQueue) {
			int len = avcodec_decode_video2(m_pVideoCodecCtx,
                                                        m_pVideoFrame,
                                                        &videoFrameFinished,
                                                        &packet);
                        if (len < 0) {
                                printf("decode video packet failed.\n");
				return false;
                        }
			av_free_packet(&packet);
			if (videoFrameFinished) {
				pVideoQueue->frame_queue_put(m_pVideoFrame);
				break;
			}
		}
	}
	return true;
}

void CCapture::close()
{
	if (m_pVideoFrame) {
		av_frame_free(&m_pVideoFrame);
		m_pVideoFrame = NULL;
	}
	if (m_pVideoCodecCtx) {
                avcodec_close(m_pVideoCodecCtx);
		m_pVideoCodecCtx = NULL;
	}
	if (m_pFormatCtx) {
                // Close a media file (but not its codecs).
		avformat_close_input(&m_pFormatCtx);
		m_pFormatCtx = NULL;
	}
}
