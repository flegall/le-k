.PHONY: all clean stats infos release run glewinforun visualinforun pack prof debug
HEADERS = $(wildcard *.hh)
SRC = $(wildcard *.cc)
OBJ = $(SRC:.cc=.o)
#DEBUG = -g
#DEBUG = -pg
DEBUG = -O2
COMMONS_CFLAGS = $(DEBUG)
FREPO =
#FREPO = -frepo
COMMONS_LDFLAGS = $(DEBUG) $(FREPO)
COMMONS_CPPFLAGS = -Wall -I. -I../boost
MAKEFILES = Makefile Makefile.win32.mak Makefile.osx.mak Makefile.commons.mak Makefile.linux.mak
DEPDIR = .deps
df = $(DEPDIR)/$(*F)
MAKEDEPEND = $(CPP) $(CPPFLAGS) -MM -MP -o $(df).d $<
COPYDATA = cp -frv data output; rm -vrf `find output|grep CVS`

clean:
	rm -fv *.o *~ *.~*~ *.rpo $(DEPDIR)/*.d demo-linux demo-osx demo.exe glewinfo-linux glewinfo-osx glewinfo.exe visualinfo-linux visualinfo-osx visualinfo.exe gmon.out gprof-output.txt demo.app/Contents/MacOS/demo-osx
	rm -rvf output
	mkdir output

SELECTED_SOURCES_FOR_STAT = $(SRC) $(HEADERS)

stats:
	@echo "Chars:"
	@wc -c $(SELECTED_SOURCES_FOR_STAT)
	@echo "Words:"
	@wc -w $(SELECTED_SOURCES_FOR_STAT)
	@echo "Lines:"
	@wc -l $(SELECTED_SOURCES_FOR_STAT)

%.o : %.cc $(MAKEFILES)
	@$(MAKEDEPEND)	
	$(CPP) $(CPPFLAGS) $(CFLAGS) $(FREPO) -c -o $@ $<

-include $(SRC:%.cc=$(DEPDIR)/%.d)

glew.o: GL/glew.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c GL/glew.c

glewinfo.o: GL/glewinfo.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c GL/glewinfo.c

visualinfo.o: GL/visualinfo.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c GL/visualinfo.c 
