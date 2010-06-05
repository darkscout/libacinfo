all:
	make -C src libacinfo
	make -C examples/acinfo acinfo

clean:
	make -C src clean
	make -C examples/acinfo clean

install:
	cp bin/libacinfo.so /usr/local/lib/
	cp include/libacinfo.h /usr/local/include/
	ldconfig
