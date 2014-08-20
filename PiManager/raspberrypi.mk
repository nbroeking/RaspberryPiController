CC=g++
LL=g++
CFLAGS=-Wall -g  -Wl,--no-as-needed -I/home/nbroeking/Documents/Framework/include -I/home/nbroeking/Documents/Librarys/x86/include -I/home/nbroeking/Documents/Librarys/x86 -I/home/nbroeking/Documents/Librarys/x86/5.3/gcc_64/include -I/home/nbroeking/Documents/PiManager/include
LDFLAGS=-Wl,--no-as-needed -lpthread
LIBS=-ldl -L /home/nbroeking/Documents/Librarys/x86/5.3/gcc_64/lib
OBJECTS=obs/EventQueue.o obs/Application.o obs/MainApplication.o obs/Event.o obs/PlayerSystem.o obs/PiException.o obs/CommSystem.o obs/Log.o obs/ScopedLock.o obs/ServerSocket.o obs/Socket.o obs/SDLHelper.o
BINARY=Manager
.PHONY: all clean
all: obs/main.o $(OBJECTS)
	$(LL) $(OBJECTS) $< $(LDFLAGS) $(LIBS) -pthread -std=c++11 -o $(BINARY) 

obs/main.o: main.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/EventQueue.o: /home/nbroeking/Documents/Framework/classes/EventQueue.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/Application.o: /home/nbroeking/Documents/Framework/classes/Application.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/MainApplication.o: /home/nbroeking/Documents/Framework/classes/MainApplication.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/Event.o: /home/nbroeking/Documents/Framework/classes/Event.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/PlayerSystem.o: /home/nbroeking/Documents/Framework/classes/PlayerSystem.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/PiException.o: /home/nbroeking/Documents/Framework/classes/PiException.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/CommSystem.o: /home/nbroeking/Documents/Framework/classes/CommSystem.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/Log.o: /home/nbroeking/Documents/Framework/classes/Log.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/ScopedLock.o: /home/nbroeking/Documents/Framework/classes/ScopedLock.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/ServerSocket.o: /home/nbroeking/Documents/PiManager/classes/ServerSocket.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/Socket.o: /home/nbroeking/Documents/PiManager/classes/Socket.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

obs/SDLHelper.o: /home/nbroeking/Documents/PiManager/classes/SDLHelper.cpp
	$(CC) -c $(CFLAGS) $< -pthread -std=c++11 -o $@

