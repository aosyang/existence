CC = gcc
XX = g++
AR = ar

CFLAGS = -Wall -O -g -D_LINUX
INCLUDE = -I ../extra/include/ -I ../extra/include/freetype2/freetype2/
LIBS = 
LIBPATH = ../bin/libs
TARGET = $(LIBPATH)/libEngine.a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
%.o: %.cpp
	$(XX) $(CFLAGS) -c $< -o $@ $(INCLUDE)
SOURCES = $(wildcard *.c *.cpp tinyxml/*.cpp)
OBJS = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES)))

#include $(OBJ:.o=.d)

$(TARGET): $(OBJS)
#	$(XX) $(OBJS) -o $(TARGET)
	
#	chmod a+x $(TARGET)
	$(AR) -rcs $@ $^
	
clean:
	rm -rf *.o $(TARGET)
	
#depend:
#	$(CC) $(CFLAGS) -c $< $(INCLUDE) -E -MM *.c *.cpp > depend
