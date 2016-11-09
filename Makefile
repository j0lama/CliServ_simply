CC = gcc -g
EXE =

all :
	$(CC) -o server socket_server.c
	$(CC) -o client socket_client.c
	@echo "------------DONE------------"

.PHONY : clean
clean :
	rm -f *.o core $(EXE)