#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

/** Comandos para compilar:
 *	arm-linux-gnueabi-gcc usr_mem_app.c -o usr_mem_app
 *	PARA EJECUTAR:
 *	./usr_app
 */
int read_data(int fd){
	int ret;
	int addr;
	int size;
	char ch[256];

	printf("Lectura sobre la memoria:\n");
	printf("Introduzca la dirreción:\n");
	scanf("%d", &addr);
	printf("Introduzca el número de caracteres a leer:\n");
	scanf("%d", &size);

	//Se establece la dirreción
	if(ioctl(fd, 0x01, addr) == 0){
		//Se realiza la lectura de los datos
		ret = read(fd, ch, size);
		if (ret < 0) {
			printf("Falló la lectura de la memoria.\n");
			close(fd);
			return -1;
		} else {
			printf("La trama de la lectura es: \n");
			printf(" %s \n", ch);
		}
	}
	return 0;
}

int write_data(int fd){
	int ret;
	int addr;
	char ch[256];

	printf("Escritura sobre la memoria:\n");
	printf("Introduzca la dirreción:\n");
	scanf("%d", &addr);
	printf("Introduzca la cadena de caracteres:\n");
	scanf("%s",ch);
	printf("Tamaño de la trama: %d\n", strlen(ch));

	//Se establece la dirreción
	if(ioctl(fd, 0x01, addr) == 0){
		//Se realiza la escritura de los datos
		ret = write(fd,ch,strlen(ch));
		if (ret < 0) {
			printf("Falló la escritura de la memoria.\n");
			close(fd);
			return -1;
		} else {
			printf("La escritura fué correcta.\n");
		}
	}
	return 0;
}

int main(int argc, char **argv) {

	printf("Se empieza el programa de prueba de la memoria 24LS512\n");

	int fd;
	int flag = 1;
	int op = 0;
	int ret;

	printf("Abriendo dispositivo i2c...\n");

	fd = open("/dev/memIMD", O_RDWR); // Dispositivo se abre con escritura y lectura
	if (fd < 0) {
		printf("Error al abrir el dispositivo\n");
		return -1;
	}

	while (flag) {

		printf("Introduzca la operación a realizar:\n");
		printf(" 1. Lectura de la memoria.\n");
		printf(" 2. Escritura sobre la memoria.\n");
		printf(" 3. Salir del programa.\n");
		scanf("%d", &op);

		if(op == 1){ //Se realiza la lectura
			ret = read_data(fd);
			if(ret != 0){
				flag = 0;
			}
		}

		if(op == 2){ //Se realiza la escritura
			ret = write_data(fd);
			if(ret != 0){
				flag = 0;
			}
		}

		if(op == 3 ){
			flag = 0;
		}
	}
	printf("Finalización del programa.\n");
	close(fd);
	return 0;
}
