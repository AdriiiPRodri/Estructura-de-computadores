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

int pcount_for(int* array, int len)
{
  int result = 0;
  for(int j = 0; j < len; j++){
    unsigned x = array[j];
    for (int i = 0; i < 8*sizeof(int); i++) {
      unsigned mask = 1 << i;
      result += (x & mask) != 0;
    }
  }
  return result;
}

int pcount_while(int* array, int len) {
  int result = 0;
  for(int j = 0; j < len; j++){
    unsigned x = array[j];
    while (x) {
      result += x & 0x1;
      x >>= 1;
    }
  }
  return result;
}

int pcount_asm(int* array, int len) {
  int result = 0;
  for(int i = 0; i < len; i++){
    unsigned x = array[i];
    asm("\n"
    "ini3:                \n\t"
          "shr $1, %[x]   \n\t"         // Desplaza 1 hacia la derecha, esta dentro de un bucle ini3
          "adc $0, %[r]   \n\t"         // Sumamos si hay acarreo en este caso por la derecha
          "cmp $0, %[x]   \n\t"         // Activamos o no el ZF para saltar o no
          "ja ini3        \n\t"
          :[r]"+r" (result)             // Indica que r es de lectura-escritura desde result, r referencia a result en c
          :[x]"r" (x)     );            // x referencia a x en c y es de lectura solo
  }
  return result;
}

int pcount_4(int* array, int len) {
  int result = 0;
  int val;
  int x;

  for(int j = 0; j < len; j++){
    val = 0;
    x = array[j];
    for(int i = 0; i < 8; i++){
      val += x & 0x01010101L;
      x >>= 1;
    }
    val += (val >> 16);
    val += (val >> 8);
    val = val & 0xFF;
    result += val;
  }
  return result;
}

// Version SSSE3 (pshufb) web http://wm.ite.pl/articles/sse-popcount.html
int popcount5(unsigned* array, int len){
  int i,
      val,
      result = 0;
  int SSE_mask[] = {0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f};
  int SSE_LUTb[] = {0x02010100, 0x03020201, 0x03020201, 0x04030302};

  if(len & 0x3)
    printf("Leyendo 128b pero len no múltiplo de 4?\n");
  for(i = 0; i < len; i+=4){
    asm("movdqu   %[x],   %%xmm0  \n\t"
        "movdqa   %%xmm0, %%xmm1  \n\t"
        "movdqu   %[m],   %%xmm6  \n\t"
        "psrlw    $4,     %%xmm1  \n\t"
        "pand     %%xmm6, %%xmm0  \n\t"
        "pand     %%xmm6, %%xmm1  \n\t"

        "movdqu   %[l],   %%xmm2  \n\t"
        "movdqa   %%xmm2, %%xmm3  \n\t"
        "pshufb   %%xmm0, %%xmm2  \n\t"
        "pshufb   %%xmm1, %%xmm3  \n\t"

        "paddb    %%xmm2, %%xmm3  \n\t"
        "pxor     %%xmm0, %%xmm0  \n\t"
        "psadbw   %%xmm0, %%xmm3  \n\t"
        "movhlps  %%xmm3, %%xmm0  \n\t"
        "paddd    %%xmm3, %%xmm0  \n\t"
        "movd     %%xmm0, %[val]  \n\t"
        : [val]"=r" (val)
        : [x]  "m"  (array[i]),
          [m]  "m"  (SSE_mask[0]),
          [l]  "m"  (SSE_LUTb[0])
        );
        result += val;
  }
  return result;
}

int popcount6(int* lista, int longitud){
  unsigned x;
  int val, result = 0;
  for(int i = 0; i < longitud; i++){
    x = lista[i];
    asm("popcnt %[x], %[x]  \n\t"
      :[val] "=r" (val)
      :[x] "r" (x)
    );
    result += val;
  }
  return result;
}

int popcount7(int* lista, int longitud){
  unsigned x1, x2;
  int val, result = 0;
  if(longitud & 0x1)
    printf("Leer 6b y longitud impar?\n");
  for(int i = 0; i < longitud; i+=2){
    x1 = lista[i];
    x2 = lista[i+1];
    asm("popcnt %[x1], %[val] \n\t"
        "popcnt %[x2], %%edi  \n\t"
        "add %%edi, %[val]    \n\t"
        :[val]"=&r"(val)
        :[x1] "r" (x1),
        [x2] "r" (x2)
        :"edi"
    );
    result += val;
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

  crono(pcount_for, "popcount1 (lenguaje C -)");
  crono(pcount_while, "popcount2 (lenguaje C -)");
  crono(pcount_asm, "popcount3 (lenguaje C - ASM)");
  crono(pcount_4, "popcount4 (lenguaje C -)");
  crono(popcount5, "popcount5 (lenguaje C - ASM)");
  crono(popcount6, "popcount6 (lenguaje C - ASM)");
  crono(popcount7, "popcount7 (lenguaje C - ASM)");

  #if ! COPY_PASTE_CALC
    printf("Calculado = %d\n", RESULT);
  #endif
    exit(0);
}
