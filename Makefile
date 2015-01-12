CC = g++
C_SOURCES = $(wildcard *.cpp)
OBJS = $(C_SOURCES:.cpp=.o)
DEBUG = -g
CINCLUDE = include/
CFLAGS = -Wall -std=c++11 -I$(CINCLUDE)
LFLAGS = -Wall $(DEBUG) -lGLEW -lGLU -lGL -lsfml-system -lsfml-window

all: main

run: all
	./main

main: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o main

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf main *.o *~ \#*\#

