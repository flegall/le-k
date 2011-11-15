CPPFLAGS = $(COMMONS_CPPFLAGS) -I/Library/Frameworks/SDL.framework/Headers -I/System/Library/Frameworks/OpenGL.framework/Headers -I/Library/Frameworks/SDL_mixer.framework/Headers -I/Library/Frameworks/SDL_ttf.framework/Headers
SDK=/Developer/SDKs/MacOSX10.4u.sdk
ARCH = -arch ppc -arch i386 -march=pentium4
#ARCH = -arch ppc
CFLAGS = $(COMMONS_CFLAGS) -isysroot $(SDK) $(ARCH)
LDFLAGS =  $(COMMONS_LDFLAGS) -Wl,-syslibroot,$(SDK) $(ARCH) -framework SDL -framework SDL_ttf -framework SDL_mixer -framework Cocoa -framework OpenGL -framework AGL
CPP = g++
CC = gcc
STRIP = strip
PAUSE_ITUNES = osascript -e 'tell application "iTunes"' -e "pause" -e "end tell"
PLAY_ITUNES = osascript -e 'tell application "iTunes"' -e "play" -e "end tell"

all: demo-osx

include Makefile.commons.mak

run: demo-osx
	$(PAUSE_ITUNES)
	./demo-osx
	$(PLAY_ITUNES)

prof: run
	gprof demo-osx > gprof-output.txt

debug: demo-osx
	gdb demo-osx -x gdb-commands.txt

release: all
	$(STRIP) demo-osx
	cp -f demo-osx demo.app/Contents/MacOS
	cp -frv demo.app output
	cp -fvr /Library/Frameworks/SDL* output/demo.app/Contents/Frameworks
	$(COPYDATA)

SDLMain.o: osx/SDLMain.m
	$(CPP) $(CPPFLAGS) $(CFLAGS) -c osx/SDLMain.m

demo-osx: $(OBJ) SDLMain.o glew.o
	$(CPP) $(LDFLAGS) $(OBJ) SDLMain.o glew.o -o demo-osx

glewinfo-osx: glewinfo.o glew.o
	$(CC) $(LDFLAGS) glewinfo.o glew.o -o glewinfo-osx

visualinfo-osx: visualinfo.o glew.o
	$(CC) $(LDFLAGS) visualinfo.o glew.o -o visualinfo-osx

infos: visualinfo-osx glewinfo-osx

glewinforun: glewinfo-osx
	./glewinfo-osx

visualinforun: visualinfo-osx
	./visualinfo-osx
