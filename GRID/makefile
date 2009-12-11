CC = gcc
XX = g++
AR = ar
LD = g++

ENGINEPATH = ..

CFLAGS = -Wall -O -g -D_LINUX
LDFLAGS = -shared -fPIC
INCLUDE = -I $(ENGINEPATH)/Engine/include -I $(ENGINEPATH)/extra/include/freetype2/freetype2 -I $(ENGINEPATH)/extra/include
LIBS = -L$(ENGINEPATH)/bin/libs -lEngine -lfreeimage -lfreetype -lOIS
TARGETPATH = ../bin/grid/linux
TARGET = $(TARGETPATH)/grid

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
%.o: %.cpp
	$(XX) $(CFLAGS) -c $< -o $@ $(INCLUDE)
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES)))

$(TARGET): $(OBJS)
	$(XX) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)
	
clean:
	rm -rf *.o $(TARGET)
