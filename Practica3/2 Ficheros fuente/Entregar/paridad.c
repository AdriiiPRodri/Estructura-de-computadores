//  según la versión de gcc y opciones de optimización usadas, tal vez haga falta
//  usar gcc –fno-omit-frame-pointer si gcc quitara el marco pila (%ebp)

#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>	// para gettimeofday(), struct timeval

#define TEST  0
#define COPY_PASTE_CALC 1

#if ! TEST
  #define NBITS 20
  #define SIZE (1<<NBITS)
  unsigned lista[SIZE];
  #define RESULT ("N*(N+1)/2 = %d\n", (SIZE-1)*(SIZE/2))
#else
  #define SIZE 4
  unsigned lista[SIZE] = {0x80000000, 0x00100000, 0x00000800, 0x00000001};
  #define RESULT 4

#endif

int paridad_for(int* lista, int longitud){
  int aux, bit, result = 0;

  for(int i = 0; i < longitud; i++){
    aux = lista[i];
    bit = 0;
    for(int j = 0; j < 32; j++){
      bit = bit^( aux & 0x1);
      aux >>= 1;
    }
    result += bit;
  }
  return result;
}

int paridad_while(int* lista, int longitud){
  int result = 0, bit;
  for(int i = 0; i < longitud; i++){
    unsigned int x = lista[i];
    bit = 0;
    while(x){
      bit = bit^(x & 0x1);
      x >>= 1;
    }
    result += bit;
  }
  return result;
}

int parity3(int* lista, int longitud){
  int val = 0, result = 0, x;

  for(int i = 0; i < longitud; i++){
    val = 0;
    x = lista[i];
    while(x){
      val ^= x;
      x >>= 1;
    }
    result += val & 0x1;
  }
  return result;
}

int parity4(int* lista, int longitud){
  int x, val, result = 0;
  for (int i = 0; i < longitud; i++) {
    x = lista[i];
    val = 0;
    asm("\n"
    "ini3:              \n\t"   // Seguir mientras que x != 0
        "xor %[x], %[v] \n\t"   // Realmente sólo nos interea LSB
        "shr $1,   %[x] \n\t"
        "cmp $0,   %[x] \n\t"
        "ja  ini3       \n\t"
        :[v]"+r" (val)        // e/s: entrada 0, salidad paridad elemento
        :[x]"r"  (x)          // entrada: valor elemento
    );
    result += val & 0x1;
  }
  return result;
}

int parity5(int* lista, int longitud){
  int result = 0;
  int x;

  for(int j = 0; j < longitud; j++){
    x = lista[j];
    for(int i = 16; i > 1; i /= 2){
      x ^= x >> i;
    }
    result += x & 0x1;
  }
  return result;
}

int parity6(int* lista, int longitud){
  int x, result = 0;
  for (int i = 0; i < longitud; i++) {
    x = lista[i];;
    asm(
        "mov  %[x], %%edx        \n\t"   // sacar copia para XOR. Controlar el registro...
        "shr  $16, %[x]          \n\t"
        "xor  %[x], %%edx        \n\t"
        "xor  %%dh, %%dl         \n\t"
        "setpo %%dl              \n\t"
        "movzx  %%dl, %[x]       \n\t"
        :[x]"+r" (x)            // e/s: entrada valor elemento, salidad paridad
        :
        :"edx"                  // Clobber
    );
    result += x;
  }
  return result;
}

void crono(int (*func)(), char* msg){
  struct timeval tv1,tv2;	// gettimeofday() secs-usecs
  long           tv_usecs;	// y sus cuentas
  int resultado;

  gettimeofday(&tv1,NULL);
  resultado = func(lista, SIZE);
  gettimeofday(&tv2,NULL);

  tv_usecs=(tv2.tv_sec -tv1.tv_sec )*1E6+
           (tv2.tv_usec-tv1.tv_usec);
  #if COPY_PASTE_CALC
    printf("%ld\n", tv_usecs);
  #else
    printf("resultado = %d\t", resultado);
    printf("%s:%9ld us\n", msg, tv_usecs);
  #endif
}

int main()
{
    #if ! TEST
      for(int i = 0; i < SIZE; i++)
        lista[i] = i;
    #endif
    crono(paridad_for, "paridad con for (lenguaje C)");
    crono(paridad_while, "paridad con while (lenguaje C)");
    crono(parity3, "paridad 3 (lenguaje C)");
    crono(parity4, "paridad 4 (lenguaje C - ASM)");
    crono(parity5, "paridad 5 (lenguaje C )");
    crono(parity6, "paridad 6 (lenguaje C - ASM)");

    #if ! COPY_PASTE_CALC
      printf("Calculado = %d\n", RESULT);
    #endif

    exit(0);
}
