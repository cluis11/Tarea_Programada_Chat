#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*Definición de constantes*/
#define BUFFSIZE 1
#define PUERTO		1100
#define ERROR		-1

/*Prototipos de función*/
void recibirArchivo(int SocketFD, FILE *file);

int main(){
	struct sockaddr_in stSockAddr;
    struct sockaddr_in clSockAddr;
	FILE *archivo;
	char *direccIP;
	int SocketServerFD;
	int SocketClientFD;
	int clientLen;
	int serverLen;

	/*Se crea el socket*/
	if((SocketServerFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == ERROR){
		perror("No se puede crear el socket");
		exit(EXIT_FAILURE);
	}
 
	/*Se configura la dirección del socket*/
	memset(&stSockAddr, 0, sizeof stSockAddr);
	 
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(PUERTO);
	stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
	if(bind(SocketServerFD,(struct sockaddr *)&stSockAddr, sizeof stSockAddr) == ERROR){
		perror("Error en bind");
		close(SocketServerFD);
		exit(EXIT_FAILURE);
	}
	inet_pton(AF_INET, "192.168.222.133", &stSockAddr.sin_addr);
	if(listen(SocketServerFD, 10) == ERROR){
		perror("Error listen");
		close(SocketServerFD);
		exit(EXIT_FAILURE);
	}

	while (1){
		clientLen = sizeof(clSockAddr);

		//Espera por la conexión de un cliente//
		if ((SocketClientFD = accept(SocketServerFD, (struct sockaddr *) &clSockAddr, &clientLen)) < 0)
			perror("Fallo para acpetar la conexión del cliente");

		/*Se configura la dirección del cliente*/
		clSockAddr.sin_family = AF_INET;
		clSockAddr.sin_port = htons(PUERTO);

		/*Se recibe el archivo del cliente*/
		recibirArchivo(SocketClientFD, archivo);
		

	}

 	close(SocketClientFD);
	close(SocketServerFD);
	return 0;
}

void recibirArchivo(int SocketFD, FILE *file){
	char buffer[BUFFSIZE];
	int recibido = -1;
	/*Se abre el archivo para escritura*/
	file = fopen("archivoRecibido","wb");
	while((recibido = recv(SocketFD, buffer, BUFFSIZE, 0)) > 0)
		fwrite(buffer,sizeof(char),1,file);
	fclose(file);
}
