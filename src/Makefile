CC  = /usr/bin/gcc

CFLAGS_LIB = -fPIC -O2 -Wall -g -D_REENTRANT -I /usr/include/libxml2/ -I ../include/ -I ./
LDFLAGS_LIB = 

OBJ_LIB = libacinfo.o debug.o hiddev.o basic_convert.o \
	aquastreamxt.o aquastreamxt_interface.o 


CFLAGS = -O2 -Wall -g -D_REENTRANT -I /usr/include/libxml2/ -I ../include/ -I ./
LDFLAGS = -lacinfo -lxml2 -L  ./

OBJ = examples/acinfo/main.o examples/acinfo/tcpserver.o


# http://www.linux.org/docs/ldp/howto/Program-Library-HOWTO/shared-libraries.html  section 34.

libacinfo: $(OBJ_LIB)
	$(CC) -shared -Wl,-soname,libacinfo.so \
    	-o ../bin/libacinfo.so $(OBJ_LIB) -lc

%.o: %.c
	$(CC) $(CFLAGS_LIB) -c $<

clean:
	rm -f $(OBJ_LIB)
	rm -f ../bin/libacinfo.so
	
all:
	libacinfo

	
