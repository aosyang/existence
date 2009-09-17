CC = gcc
XX = g++
AR = ar
LD = g++

CFLAGS = -Wall -O -g -D_LINUX
LDFLAGS = -shared -fPIC
INCLUDE = -I ../extra/include/ -I ../Engine/
LIBS = -L../bin/libs -lEngine # -ldl -lGL -lGLU -lXxf86vm -lfreetype -lOIS -lfreeimage 
LIBPATH = ../bin/plugins
TARGET = $(LIBPATH)/PluginCgGL.so

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
%.o: %.cpp
	$(XX) $(CFLAGS) -c $< -o $@ $(INCLUDE)
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES)))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	
clean:
	rm -rf *.o $(TARGET)

