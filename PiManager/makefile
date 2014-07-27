CC=g++
#CC=/usr/bin/arm-linux-gnueabi-gcc-4.7'
LDFLAGS=
CFLAGS= -Wall -g -I/home/nbroeking/Documents/Framework/include -std=c++11
export CC
export LDFLAGS
export CFLAGS

.PHONY: all clean gen clobber

all:
	-mkdir obs
	$(MAKE) -f raspberrypi.mk

gen:
	./gen.sh
clean:
	-rm -rf obs
	-rm Manager

clobber:
	-rm -rf obs
	-rm Manager
	-rm raspberrypi.mk
