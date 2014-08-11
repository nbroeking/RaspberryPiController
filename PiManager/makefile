CC=g++
#CC=/usr/bin/arm-linux-gnueabi-gcc-4.7'
LDFLAGS= -Wall
CFLAGS= -Wall -g -fPIE --std=c++11 -lpthread -I/home/nbroeking/Documents/Framework/include -I/home/nbroeking/Documents/Librarys/x86/include -I/home/nbroeking/Documents/Librarys/x86 -I/home/nbroeking/Documents/Librarys/x86/5.3/gcc_64/include -I/home/nbroeking/Documents/Librarys/x86/5.3/gcc_64/include/QtCore -I/home/nbroeking/Documents/Librarys/x86/5.3/gcc_64/include/QtMultimedia
LIBS = --std=c++11 -lpthread -ldl -L /home/nbroeking/Documents/Librarys/x86/5.3/gcc_64/lib -lQt5Core -lQt5Multimedia -lQt5Gui
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
