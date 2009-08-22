CC = gcc
XX = g++
AR = ar
LD = ld

CFLAGS = -Wall -O -g -D_LINUX
LDFLAGS = -shared -fPIC
INCLUDE = -I ../extra/include/ -I ../Engine/
LIBS = -L../lib -lEngine -ldl -lGL -lGLU -lXxf86vm -lfreetype -lOIS -lfreeimage 
LIBPATH = ../bin/plugins
TARGET = $(LIBPATH)/PluginGLRenderSystem.so

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
%.o: %.cpp
	$(XX) $(CFLAGS) -c $< -o $@ $(INCLUDE)
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES)))

$(TARGET): $(OBJS)
	$(XX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	
clean:
	rm -rf *.o $(TARGET)

