#include "gentech_play.h"
#include <iostream>

using namespace gentech;

CPlay::CPlay()
{
	m_pSurface = NULL;
	m_pOverlay = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
}

CPlay::~CPlay()
{
	close();
	SDL_Quit();
}

bool CPlay::init(int frameWidth, int frameHeight)
{
	m_pSurface = SDL_SetVideoMode(frameWidth, frameHeight, 0, 0);
	if (!m_pSurface) {
		std::cerr<<"SDL set video mode failed."<<std::endl;
		return false;
	}
	m_pOverlay = SDL_CreateYUVOverlay(frameWidth, frameHeight, SDL_IYUV_OVERLAY, m_pSurface);
	if (!m_pOverlay) {
		std::cerr<<"SDL create YUV overlay failed."<<std::endl;
		return false;
	}
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = frameWidth;
	m_rect.h = frameHeight;
	return true;
}

void CPlay::show(AVFrame *pFrame)
{
	SDL_LockYUVOverlay(m_pOverlay);
	for (int i = 0; i < pFrame->height; ++i) {
		memcpy(m_pOverlay->pixels[0] + i * m_pOverlay->pitches[0], 
		       pFrame->data[0] + i * pFrame->linesize[0], 
		       sizeof(uint8_t) * m_pOverlay->pitches[0]); 
	}
	for (int i = 0; i < pFrame->height / 2; ++i) {
		memcpy(m_pOverlay->pixels[1] + i * m_pOverlay->pitches[1],
		       pFrame->data[1] + i * pFrame->linesize[1],
		       sizeof(uint8_t) * m_pOverlay->pitches[1]);
		memcpy(m_pOverlay->pixels[2] + i * m_pOverlay->pitches[2], 
		       pFrame->data[2] + i * pFrame->linesize[2], 
		       sizeof(uint8_t) * m_pOverlay->pitches[2]);
	}
	SDL_UnlockYUVOverlay(m_pOverlay);
	SDL_DisplayYUVOverlay(m_pOverlay, &m_rect);
}

void CPlay::close()
{
	if (m_pOverlay) {
		SDL_FreeYUVOverlay(m_pOverlay);
		m_pOverlay = NULL;
	}
	if (m_pSurface) {
		SDL_FreeSurface(m_pSurface);
		m_pSurface = NULL;
	}
	m_rect.x = m_rect.y = m_rect.w = m_rect.h = 0;
}
