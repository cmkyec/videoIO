SDL_INCLUDE = `pkg-config sdl --cflags`
SDL_LIB = `pkg-config sdl --libs`
FFMPEG_INCLUDE = `pkg-config libavutil libavformat libavcodec --cflags`
FFMPEG_LIB = `pkg-config libavutil libavformat libavcodec --libs`

OBJ = main.o gentech_queue.o

myffplay: $(OBJ)
	if [ ! -e ./bin ]; then mkdir ./bin; fi
	g++ -o ./bin/myffplay $(OBJ) $(SDL_LIB) $(FFMPEG_LIB)
	rm *.o
main.o: main.cpp 
	g++ -c main.cpp $(SDL_INCLUDE)
gentech_queue.o: gentech_queue.cpp gentech_queue.h
	g++ -c gentech_queue.cpp $(SDL_INCLUDE) $(FFMPEG_INCLUDE)
clean:
	rm -f *.o
	rm -f ./bin/myffplay 
