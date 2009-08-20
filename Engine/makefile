CC = gcc

XX = g++
CFLAGS = -Wall -O -g -D_LINUX
INCLUDE = -I ../extra/include/ -I ../extra/include/freetype2/freetype2/ -I ../extra/include/tinyxml/
LIB = -lXxf86vm
TARGET = ./Engine.a

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $(LIB) -c $< -o $@
%.o: %.cpp
	$(XX) $(CFLAGS) $(INCLUDE) $(LIB) -c $< -o $@
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES)))

$(TARGET): $(OBJS)
	$(XX) $(OBJS) -o $(TARGET)
	
	chmod a+x $(TARGET)
	
clean:
	rm -rf *.o Engine.a
