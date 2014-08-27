#ifndef GENTECH_CAPTURE_H_ 
#define GENTECH_CAPTURE_H_

#include "gentech_queue.h"

namespace gentech
{

class CCapture
{
public:
        CCapture();

        ~CCapture();

        bool open(const char* pFilePath);

	// save decoded frame to queue
        bool operator >> (CQueue *pVideoQueue = NULL);

        void close();
private:
        AVFormatContext* m_pFormatCtx;
        int m_videoStreamIndex;
        AVCodecContext *m_pVideoCodecCtx;
	AVFrame *m_pVideoFrame;
};

}

#endif // GENTECH_CAPTURE_H_
