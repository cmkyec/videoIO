#ifndef _GENTECH_PLAY_H_
#define _GENTECH_PLAY_H_

#define __STDC_CONSTANT_MACROS
extern "C"
{
#include <libavformat/avformat.h>
#include <SDL.h>
#include <SDL_thread.h>
}

namespace gentech
{

class CPlay
{
public:
	CPlay();

	~CPlay();

	bool init(int frameWidth, int frameHeight);

	void show(AVFrame *pFrame);

	void close();
private:
	SDL_Rect m_rect;
	SDL_Surface *m_pSurface;
	SDL_Overlay *m_pOverlay;
};


}

#endif /* _GENTECH_PLAY_H_ */
