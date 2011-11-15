CPPFLAGS = $(COMMONS_CPPFLAGS) -Iwin32/include
CFLAGS =  $(COMMONS_CFLAGS) -DGLEW_BUILD=GLEW_STATIC -march=i686
LDFLAGS = $(COMMONS_LDFLAGS) -Wl -Lwin32/libs
MINGW = i386-mingw32-
#MINGW = i586-mingw32msvc-
CPP = $(MINGW)g++
CC = $(MINGW)gcc
STRIP = $(MINGW)strip
UPX = upx --best

all: demo.exe

include Makefile.commons.mak

release: all
	$(STRIP) demo.exe
	$(COPYDATA)
	cp -vf demo.exe output
	cp -vf win32/libs/*.dll output

pack: release
	$(UPX) demo.exe
	$(UPX) output/demo.exe

demo.exe: $(OBJ) glew.o
	$(CPP) $(LDFLAGS) $(OBJ) glew.o -luser32 -lgdi32 -lwinmm -ldxguid -lopengl32 -lglu32 -lmingw32 -lSDLmain -lSDL -lSDL_mixer -lSDL_ttf -o demo.exe

glewinfo.exe :	glewinfo.o glew.o
	$(CC) $(LDFLAGS) glewinfo.o glew.o -luser32 -lgdi32 -lwinmm -ldxguid -lopengl32 -lglu32 -lmingw32 -lSDLmain -lSDL -lSDL_mixer -o glewinfo.exe

visualinfo.exe: visualinfo.o glew.o
	$(CC) $(LDFLAGS) visualinfo.o glew.o -luser32 -lgdi32 -lwinmm -ldxguid -lopengl32 -lglu32 -lmingw32 -lSDLmain -lSDL -lSDL_mixer -o visualinfo.exe

infos: visualinfo.exe glewinfo.exe
