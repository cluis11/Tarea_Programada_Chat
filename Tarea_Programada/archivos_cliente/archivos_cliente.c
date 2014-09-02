/*
* Integrantes del equipo:
*			Pablo Camarillo Ramírez.
*			Diego Filloy.
*			Marco Antonio Garcia Bautista.
*			Demian Gaxiola Valles.
* Fecha: 28 de Septiembre de 2010.
* Redes de Computadora.
* Descripción:	Práctica No. 6. Aplicación cliente que envía un archivo
* 		hacia un proceso servidor.
*/
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 1
#define	ERROR	-1
#define	PUERTO	1100

int main(){
	struct sockaddr_in stSockAddr;
	int sockLen;
	int Res;
	int SocketFD;
	int recibido;
	char buffer[BUFFSIZE];
	char nombre_archivo[30];
	FILE *archivo;

	/*Se crea el socket*/
	SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
	/*Se verifica la integridad del socket*/
	if (SocketFD == ERROR){
		perror("cannot create socket");
		exit(EXIT_FAILURE);
	}

	/*Se configura la dirección del socket del cliente*/
	memset(&stSockAddr, 0, sizeof stSockAddr);
 
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(1100);
	Res = inet_pton(AF_INET, "192.168.222.133", &stSockAddr.sin_addr);
 
	sockLen = sizeof(stSockAddr);

	if (0 > Res){
		perror("error: El primer paránmetro no es una familia de direcciónes");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}else if (Res == 0){
		perror("char string (El segundo parmetro no contiene una dirección IP válida");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}

	if (connect(SocketFD, (struct sockaddr *)&stSockAddr, sizeof stSockAddr) == ERROR){
		perror("Error a la hora de conectarse con el cliente");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}

	printf("Ingrese el nombre del archivo a enviar (recuerde que debe estar en la misma carpeta que el programa): ");
	scanf("%s",nombre_archivo);

	/*Se abre el archivo a enviar*/
	archivo = fopen(nombre_archivo,"rb");
	/*Se verifica la integridad del archivo*/
	if(!archivo){
		perror("Error al abrir el archivo:");
		exit(EXIT_FAILURE);
	}

	/*Se envia el archivo*/
	while(!feof(archivo)){
		fread(buffer,sizeof(char),BUFFSIZE,archivo);
		if(send(SocketFD,buffer,BUFFSIZE,0) == ERROR)
			perror("Error al enviar el arvhivo:");
	}
	
	fclose(archivo);
	close(SocketFD);

	return 0;
}

