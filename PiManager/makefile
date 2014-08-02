CC=g++
#CC=/usr/bin/arm-linux-gnueabi-gcc-4.7'
LDFLAGS= -Wall -Wl,--no-as-needed
CFLAGS= -Wall -g -Wl,--no-as-needed --std=c++11 -lpthread -I/home/nbroeking/Documents/Framework/include -I/home/nbroeking/Documents/Librarys/x86/include -I/home/nbroeking/Documents/Librarys/x86
LIBS = /home/nbroeking/Documents/Librarys/x86/lib/libQtCore.a --std=c++11 -lpthread -ldl 
export CC
export LDFLAGS
export CFLAGS
export LIBS

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
