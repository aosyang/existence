CC = gcc
XX = g++
AR = ar

CFLAGS = -Wall -O0 -g -D_LINUX -DTIXML_USE_STL
INCLUDE = -I include/ -I ../extra/include/ -I ../extra/include/freetype2/freetype2/
LIBS = 
LIBPATH = ../bin/libs
SRC_DIR = src/
TARGET = $(LIBPATH)/libEngine.a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
%.o: %.cpp
	$(XX) $(CFLAGS) -c $< -o $@ $(INCLUDE)
#SOURCES = $(wildcard *.c *.cpp tinyxml/*.cpp)
SOURCES = $(shell find $(SRC_DIR) -name '*.c' -o -name '*.cpp')
OBJS = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES)))

#include $(OBJ:.o=.d)

$(TARGET): $(OBJS)
#	$(XX) $(OBJS) -o $(TARGET)
	
#	chmod a+x $(TARGET)
	$(AR) -rcs $@ $^
	
clean:
	rm -rf $(OBJS) $(TARGET)
	
#depend:
#	$(CC) $(CFLAGS) -c $< $(INCLUDE) -E -MM *.c *.cpp > depend
