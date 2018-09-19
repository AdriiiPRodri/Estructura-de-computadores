/*
				Orden de compilación = gcc -o bomba_Adrian_Jesus_Pena_Rodriguez bomba_Adrian_Jesus_Pena_Rodriguez.c -O0 -m32
*/

#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <string.h>	// para strncmp()/strlen()
#include <sys/time.h>	// para gettimeofday(), struct timeval

void codec(char *c){
	static int a = 1;
	int i;

	if(a == 1){
		i = 0;
		a = 0;
		while(c[i] != '\n'){
			c[i] -= 27;
			i++;
		}
	}
	else if(a == 0){
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

void boom(){
	printf("***************\n");
	printf("*** BOOM!!! ***\n");
	printf("***************\n");
	exit(-1);
}

void defused(){
	printf("*************************\n");
	printf("*** Bomba desactivada ***\n");
	printf("*************************\n");
	exit(0);
}

char password[8] = ">0;@0/0\n";
int  passcode = 487495;

int main(){
#define SIZE 100
	char pass[SIZE];
	int  pasv;
#define TLIM 60
	struct timeval tv1,tv2;	// gettimeofday() secs-usecs

	gettimeofday(&tv1,NULL);

	printf("Introduce la contraseña: ");
	fgets(pass,SIZE,stdin);

	codec(pass);

	decodec(password);
	int i = 0;

	while(i < 7){
		if(pass[i] != password[i])
			boom();
		i++;
	}
	i = 0;

	codec(password);
	codec(pass);
	while(i < 7){
		if(pass[i] != password[i])
			boom();
		i++;
	}

	gettimeofday(&tv2,NULL);
	if (tv2.tv_sec - tv1.tv_sec > TLIM)
    boom();

	printf("Introduce el código: ");
	scanf("%i",&pasv);

	if (pasv + 1 != passcode)
		boom();

	gettimeofday(&tv1,NULL);
	if (tv1.tv_sec - tv2.tv_sec > TLIM)
    boom();

	defused();
}
