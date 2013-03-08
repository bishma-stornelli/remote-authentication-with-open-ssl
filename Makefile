CC=gcc

all: insecure-client secure-client insecure-server secure-server

insecure-client: acceso-rem.o options-client.o
	$(CC) -O acceso-rem.o options-client.o -o acceso-rem

secure-client: acceso-rem-seg.o options-client.o
	$(CC) -O acceso-rem-seg.o options-client.o -o acceso-rem-seg

insecure-server: serv-acceso.o options-server.o
	$(CC) -O serv-acceso.o options-server.o -o serv-acceso

secure-server: serv-acceso-seg.o options-server.o
	$(CC) -O serv-acceso-seg.o options-server.o -o serv-acceso-seg
	
insecure-client.o: acceso-rem.c options-client.o
	$(CC) -O -c acceso-rem.c -o acceso-rem.o
  
secure-client.o: acceso-rem-seg.c options-client.o
	$(CC) -O -c acceso-rem-seg.c -o acceso-rem-seg.o 
  
insecure-server.o: server-acceso.c options-server.o
	$(CC) -O -c server-acceso.c -o server-acceso.o 
  
secure-server.o: server-acceso-seg.c options-server.o
	$(CC) -O -c server-acceso-seg.c -o server-acceso-seg.o 
	
%.o: %.c %.h
	$(CC) -c $<
	
clean:
	rm *.o acceso-rem acceso-rem-seg serv-acceso serv-acceso-seg
