#include <iostream>
#include "gentech_queue.h"
#include "gentech_capture.h"
#include "gentech_play.h"
#include <unistd.h>

int main()
{
	gentech::CQueue queue;
	gentech::CCapture capture;
	const char *pVideoPath = "./testResource/test.flv";
	if (!capture.open(pVideoPath)) {
		std::cerr<<"can not open the video: "<<pVideoPath<<std::endl;
		return 1;
	}
	for (int i = 0; i < 1000; ++i) {
		if (!(capture >> &queue)) {
			std::cerr<<"capture frame failed. i = "<<i<<std::endl;
			return 1;
		}
	}
	std::cout<<"queue size: "<<queue.size()<<std::endl;

	gentech::CPlay play;
	AVFrame *pFrame = av_frame_alloc();
	queue.frame_queue_get(pFrame);
	play.init(pFrame->width, pFrame->height);
	
	for (int i = 0; i < 999; ++i) {
		queue.frame_queue_get(pFrame);
		play.show(pFrame);
		sleep(30);
	}
	
	av_frame_free(&pFrame);
	return 0;
}
