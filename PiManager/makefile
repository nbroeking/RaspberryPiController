CC=/usr/bin/arm-linux-gnueabi-gcc-4.7
#CC=gcc
LDFLAGS= -lobjc
HEADERS= -I/home/nbroeking/Documents/Framework/include
DEPENDS= main.o Application.o
PROJECT=Manager

.PHONY: all clean

all: $(DEPENDS)
	$(CC) $(DEPENDS) $(LDFLAGS) -o $(PROJECT)

Application.o: Framework/classes/Application.m
	$(CC) $< $(HEADERS) $(CCFLAGS) -c -o $@

main.o: PiManager/main.m
	$(CC) $< $(HEADERS) $(CCFLAGS) -c -o $@

clean:
	rm *.o
	rm Manager
