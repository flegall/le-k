CPPFLAGS = $(COMMONS_CPPFLAGS) $(shell sdl-config --cflags)
CFLAGS = $(COMMONS_CFLAGS) -march=i686
LDFLAGS = $(COMMONS_LDFLAGS) $(shell sdl-config --libs) -lGL -lGLU -lSDL_mixer -lSDL_ttf
CPP = g++
CC = gcc
STRIP = strip
UPX = upx --best

all: demo-linux

include Makefile.commons.mak

run: demo-linux
	./demo-linux

debug: demo-linux
	gdb demo-linux -x gdb-commands.txt

release: all
	$(STRIP) demo-linux
	$(UPX) demo-linux
	$(COPYDATA)
	cp -fv demo-linux output

demo-linux: $(OBJ) glew.o
	$(CPP) $(LDFLAGS) $(OBJ) glew.o -o demo-linux

glewinfo-linux: glewinfo.o glew.o
	$(CC) $(LDFLAGS) glewinfo.o glew.o -o glewinfo-linux

visualinfo-linux: visualinfo.o glew.o
	$(CC) $(LDFLAGS) visualinfo.o glew.o -o visualinfo-linux

infos: visualinfo-linux glewinfo-linux

glewinforun: glewinfo-linux
	./glewinfo-linux

visualinforun: visualinfo-linux
	./visualinfo-linux
