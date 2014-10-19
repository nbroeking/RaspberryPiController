CC=g++
LL=g++
#CC=/usr/bin/arm-linux-gnueabi-g++-4.7
#LL=/usr/bin/arm-linux-gnueabi-g++-4.7

INCLUDE=  -I/home/nbroeking/Documents/Framework/include \
-I/home/nbroeking/Documents/Librarys/x86/include \
-I/home/nbroeking/Documents/Librarys/x86 \
-I/home/nbroeking/Documents/Librarys/x86/5.3/gcc_64/include \
-I/home/nbroeking/Documents/PiManager/include

LDFLAGS= -Wl,--no-as-needed -lpthread
CFLAGS= -Wall -g  -Wl,--no-as-needed $(INCLUDE)
LIBS = -ldl -L /home/nbroeking/Documents/Librarys/x86/5.3/gcc_64/lib -L tmp -L usr/lib/ -lSDL -lSDL_mixer

export CC
export LL
export LDFLAGS
export CFLAGS
export LIBS

.PHONY: all clean gen clobber

all:
	$(MAKE) -f raspberrypi.mk

gen:
	-mkdir obs
	./gen.sh
clean:
	-rm -rf obs
	-rm Manager

clobber:
	-rm -rf obs
	-rm Manager
	-rm raspberrypi.mk
