CC  = /usr/bin/gcc

CFLAGS = -O2 -Wall -g -D_REENTRANT -I /usr/include/libxml2/ -I ../../include/ -I ./
LDFLAGS = -lacinfo -lxml2 -L  ../../bin/

OBJ = main.o tcpserver.o


acinfo: $(OBJ)
	$(CC) $(CFLAGS) -o ../../bin/acinfo $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ)
	rm -f ../../bin/acinfo
	
all:
	acinfo

	
