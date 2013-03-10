CC=gcc
BIN=./bin/
SRC=./src/

all: insecure-client insecure-server secure-client nuevo-usuario

insecure-client: $(BIN)acceso-rem.o $(BIN)options-client.o $(BIN)helper.o
	$(CC) -o $(BIN)acceso-rem $(BIN)acceso-rem.o $(BIN)options-client.o $(BIN)helper.o

insecure-server: $(BIN)serv-acceso.o $(BIN)options-server.o $(BIN)helper.o $(BIN)usuarios.o
	$(CC) -o $(BIN)serv-acceso $(BIN)serv-acceso.o $(BIN)options-server.o $(BIN)helper.o $(BIN)usuarios.o -lssl -lcrypto

secure-client: $(BIN)acceso-rem-seg.o $(BIN)options-client.o $(BIN)secure-helper.o
	$(CC) -o $(BIN)acceso-rem-seg $(BIN)acceso-rem-seg.o $(BIN)options-client.o $(BIN)secure-helper.o $(BIN)tcp-helper.o -lssl -lcrypto

nuevo-usuario: $(BIN)usuarios.o $(BIN)nuevo-usuario.o
	$(CC) -o $(BIN)nuevo-usuario $(BIN)usuarios.o $(BIN)nuevo-usuario.o -lssl -lcrypto	
	
$(BIN)secure-helper.o: $(SRC)secure-helper.c $(BIN)tcp-helper.o
	$(CC) -c $(SRC)secure-helper.c -o $(BIN)secure-helper.o
	
$(BIN)%.o: $(SRC)%.c
	$(CC) -c $< -o $@
	
clean:
	rm -f $(BIN)*.o $(BIN)serv-acceso $(BIN)acceso-rem $(BING)acceso-rem-seg
	clear 
