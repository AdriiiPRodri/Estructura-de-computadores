#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <string.h>	// para strncmp()/strlen()
#include <sys/time.h>	// para gettimeofday(), struct timeval

char password[]="abracadabra\n";
int  passcode  = 7777;

void codec(char *c){
	static int a = 0;
	int i;
	if(a == 0){
		i = 0;
		a = 1;
		while(c[i] != '\n'){
			c[i] -= 27;
			i++;
		}
	}
	else{
		i = 0;
		while(c[i] != '\n'){
			c[i] += 230;
			i++;
		}
	}
}

void decodec(char *c){
	static int a = 0;
	int i;
	if(a == 1){
		i = 0;
		while(c[i] != '\n'){
			c[i] += 27;
			i++;
		}
	}
	else{
		i = 0;
		a = 1;
		while(c[i] != '\n'){
			c[i] -= 230;
			i++;
		}
	}
}

int main(){
	#define SIZE 20
		char pass[SIZE];
		int  pasv;
		int i = 0;

	printf("Introduce la contraseña: ");
	fgets(pass,SIZE,stdin);												// Termina con \n
	codec(pass);
	while(pass[i] != '\n'){
		printf("%c",pass[i]);
		i++;
	}
	printf("\nPrimera codificación\n");
	i = 0;
	codec(pass);
	while(pass[i] != '\n'){
		printf("%c",pass[i]);
		i++;
	}
	printf("\nSegunda codificación\n");

	decodec(pass);
	i = 0;
	while(pass[i] != '\n'){
		printf("%c",pass[i]);
		i++;
	}
	printf("\nPrimera decodificación\n");

	decodec(pass);
	i = 0;
	while(pass[i] != '\n'){
		printf("%c",pass[i]);
		i++;
	}
	printf("\nSegunda decodificación\n");
}
