OBJS	= main.o
SOURCE	= src/main.cpp
HEADER	= 
OUT	= radio
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = -lSDL2 -lSDL2_ttf -lSDL2_mixer -fsanitize=address -lpulse -lpulse-simple -lfftw3

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o:
	$(CC) $(FLAGS) src/main.cpp -std=c++17


clean:
	rm -f $(OBJS) $(OUT)
