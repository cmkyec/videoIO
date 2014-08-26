#ifndef _GENTECH_QUEUE_H_
#define _GENTECH_QUEUE_H_

#define __STDC_CONSTANT_MACROS
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <SDL.h>
#include <SDL_thread.h>
}


namespace gentech
{

struct AVFrameList
{
	AVFrame *frame;
	struct AVFrameList *next;
};

struct AVFrameQueue
{
	AVFrameList *first_frame, *last_frame;
	int nb_frames;
	SDL_mutex *mutex;
	SDL_cond *cond;
};

class CQueue
{
public:
	CQueue();

	~CQueue();

	int frame_queue_put(AVFrame *frame);

	void frame_queue_get(AVFrame *frame);

	void frame_queue_flush();

protected:
	void frame_queue_init();
	
	void frame_queue_destroy();

private:
	AVFrameQueue m_queue;
};


}

#endif /* _GENTECH_QUEUE_H_ */
