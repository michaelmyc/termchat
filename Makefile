client: clean
	gcc -Werror -Wall -lcurses ./src/client/client.c ./src/client/helper.c ./src/client/connection.c -o ./termchat

test_client: client
	./termchat testserver.com testroom tester

clean:
	rm -f termchat server
