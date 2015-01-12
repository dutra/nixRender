CC = g++
C_SOURCES = $(wildcard *.cpp)
OBJS = $(C_SOURCES:.cpp=.o)
DEBUG = -g
CINCLUDE = include/
CFLAGS = -Wall -I$(CINCLUDE)
LFLAGS = -Wall $(DEBUG)

all: main

run: all
	./main

main: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o main

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf main *.o *~
