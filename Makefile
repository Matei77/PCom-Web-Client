CC = g++
CFLAGS = -Wall -Wextra -std=c++17

.PHONY: build clean

build: client


main.o: main.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

client.o: client.cpp client.hpp
	$(CC) $(CFLAGS) -o $@ -c $<

connection.o: connection.cpp connection.hpp
	$(CC) $(CFLAGS) -o $@ -c $<

requests.o: requests.cpp requests.hpp
	$(CC) $(CFLAGS) -o $@ -c $<

utils.o: utils.cpp utils.hpp
	$(CC) $(CFLAGS) -o $@ -c $<


client: main.o client.o connection.o requests.o utils.o
	$(CC) $(CFLAGS) -o $@ $^


clean:
	-rm -f *.o client