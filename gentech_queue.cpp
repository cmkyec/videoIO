#include "gentech_queue.h"

using namespace gentech;

CQueue::CQueue() 
{
	frame_queue_init();
}

CQueue::~CQueue()
{
	frame_queue_destroy();
}

int CQueue::frame_queue_put(AVFrame *frame)
{
	SDL_LockMutex(m_queue.mutex);
	
	AVFrameList* list = (AVFrameList*)av_malloc(sizeof(AVFrameList));
	if (!list) goto _exit;
	list->frame = av_frame_alloc();
	if (!list->frame) {
		av_free(list);
		goto _exit;
	}
	av_frame_ref(list->frame, frame); // copy frame data
	list->next = NULL;
	if (!m_queue.last_frame) {
		m_queue.first_frame = list;
	} else {
		m_queue.last_frame->next = list;
	}
	m_queue.last_frame = list;
	m_queue.nb_frames++;
	SDL_CondSignal(m_queue.cond);
	SDL_UnlockMutex(m_queue.mutex);
	return 1;

_exit:
	SDL_UnlockMutex(m_queue.mutex);
	return -1;
}

void CQueue::frame_queue_init()
{
	memset(&m_queue, 0, sizeof(AVFrameQueue));
	m_queue.mutex = SDL_CreateMutex();
	m_queue.cond = SDL_CreateCond();
}

void CQueue::frame_queue_destroy()
{
	SDL_LockMutex(m_queue.mutex);
	AVFrameList *list_a, *list_b;
	for (list_a = m_queue.first_frame; list_a != NULL; list_a = list_b) {
		list_b = list_a->next;
		av_frame_free(&list_a->frame);
		av_free(list_a);
	}
	SDL_UnlockMutex(m_queue.mutex);

	SDL_DestroyMutex(m_queue.mutex);
	SDL_DestroyCond(m_queue.cond);
}

void CQueue::frame_queue_get(AVFrame *frame)
{
	AVFrameList *list;

	SDL_LockMutex(m_queue.mutex);
	for (;;) {
		list = m_queue.first_frame;
		if (list) {
			m_queue.first_frame = list->next;
			if (!m_queue.first_frame) m_queue.last_frame = NULL;
			m_queue.nb_frames--;
			av_frame_ref(frame, list->frame);
			av_frame_free(&list->frame);
			av_free(list);
			break;
		} else {
			SDL_CondWait(m_queue.cond, m_queue.mutex);
		}
	}
	SDL_UnlockMutex(m_queue.mutex);
	return;
}

