CC = g++
DEBUG = -g

CINCLUDE = include
SRCDIR = src
OBJDIR = obj

HEADERS = $(wildcard $(CINCLUDE)/*.h)
C_SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(C_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

CFLAGS = -Wall $(DEBUG) -std=c++1y -I$(CINCLUDE)
LFLAGS = -Wall $(DEBUG) -lGLEW -lGLU -lGL -lsfml-system -lsfml-window

all: main
	@echo "Done!"
run: all
	./main

main: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@
	@echo "Linking complete!"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"
clean:
	rm -rf $(OBJS) main *~ \#*\#
	@echo "Cleanup complete!"
