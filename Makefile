client: clean
	gcc -lcurses ./src/client/client.c -o ./client

run_client: client
	./client

clean:
	rm -f client server