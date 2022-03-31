OBJS	= main.o pulse.o visualizer.o defs.o cube.o
SOURCE	= src/main.cpp src/input/pulse.cpp src/visualizer.cpp src/defs.cpp src/cube.cpp
HEADER	= 
OUT	= radio
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = -lSDL2 -lSDL2_ttf -lSDL2_mixer -fsanitize=address -lpulse -lpulse-simple -lfftw3

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o:
	$(CC) $(FLAGS) src/main.cpp -std=c++17
visualizer.o:
	$(CC) $(FLAGS) src/visualizer.cpp -std=c++17
pulse.o:
	$(CC) $(FLAGS) src/input/pulse.cpp -std=c++17
defs.o:
	$(CC) $(FLAGS) src/defs.cpp -std=c++17
cube.o:
	$(CC) $(FLAGS) src/cube.cpp -std=c++17


clean:
	rm -f $(OBJS) $(OUT)
