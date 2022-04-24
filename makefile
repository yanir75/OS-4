all: client server

zip:
	zip archive.zip makefile *.c *.h *.md

server: server.o 
	gcc -o server server.o -pthread

test: test.o
	gcc -o test test.o

client: client.o
	gcc -o client client.o

%.o: %.c
	gcc -c -g $^

clean:
	rm -rf *.o *.zip
	rm server client test