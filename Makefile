CC=gcc
BIN=./bin/
SRC=./src/

all: insecure-client insecure-server

insecure-client: $(BIN)acceso-rem.o $(BIN)options-client.o $(BIN)helper.o
	$(CC) -o $(BIN)acceso-rem $(BIN)acceso-rem.o $(BIN)options-client.o $(BIN)helper.o

insecure-server: $(BIN)serv-acceso.o $(BIN)options-server.o $(BIN)helper.o $(BIN)usuarios.o
	$(CC) -o $(BIN)serv-acceso $(BIN)serv-acceso.o $(BIN)options-server.o $(BIN)helper.o $(BIN)usuarios.o -lssl -lcrypto 
	
$(BIN)%.o: $(SRC)%.c
	$(CC) -c $< -o $@
	
clean:
	rm -f $(BIN)*
	clear 
