.section .data
lista:		.int 1,2,10		# Definimos un array de int en el que metemos los valores 1, 2, 10
longlista:	.int (.-lista)/4	# El punto es la dirección actual donde se quedo desde la última linea donde hemos cogido memoria, le restamos el 						# inicio de la lista(devuelve una direccion) y lo dividimos por 4 ya que va cada 4 bytes que son 32 bits y esto 					# nos va a dar el tamaño de la lista. Hay que dividir por 4 porque es memoria lo que nos devuelve la operación 						# (.-lista)
resultado:	.int -1			# Declaramos la variable resultado donde almacenaremos el mismo, está esta inicializada a -1 (0xFFFFFFFF)
formato:
		.ascii "%d"

.section .text				# Empezamos con el código del programa, digamos el algorítmo en si mismo
.global main				# LLamamos a main
main:					# Comenzamos con nuestro main (cuerpo)

	mov    $lista, %ebx		# Movemos nuestra lista al registro %ebx
	mov longlista, %ecx		# Movemos el tamaño de nuestra lista al registro %ecx para usarlo en un bucle posteriormente
	call suma			# Llamamos a nuestra función suma la cual tenemos definida más abajo
	mov %eax, resultado		# Movemos el registro %eax a nuestra variable resultado, de esta manera lo almacenamos en ella para 						# posteriormente mostrarlo, en %eax tenemos el resultado de la suma
	push resultado			# Colocamos el resultado en la pila
	push $formato
	call printf			# Extrae de la pila nuestro valor resultado y lo imprime por pantalla
	add $8, %esp

/*************SALIR**************/
	mov $1, %eax			# Ya no necesitamos el resultado en %eax ya que lo tenemos en resultado, por tanto empezamos la salida del 						# programa, usando llamadas al kernel de linux de esta manera
	mov $0, %ebx
	int $0x80
/********************************/

suma:
	push %edx			# Pusheamos a la pila el valor de %edx para guardarlo despues de llamar a la función suma, posteriormente lo 						# recuperaremos con la llamada pop la cual recoge el valor de la pila en orden LIFO
	mov $0, %eax			# Ponemos %eax a 0, en este guardaremos la suma
	mov $0, %edx			# Ponemos %edx a 0, este es nuestro contador, el cual en un principio a de estar a 0
bucle:					# Definición del bucle
	add (%ebx,%edx,4), %eax		# (%ebx,%edx,4) calcula %ebx + 4*%edx, esto vale para ver los distintos valores del array y los vamos acumulando 						# en %eax para saber cual es la suma de todos ellos
	inc       %edx			# En cada iteración del bucle debemos incrementar en 1 nuestro contador, el cual es %edx
	cmp  %edx,%ecx			# Compara el tamaño de %edx (contador) y %ecx (tamaño de la lista), esto nos vale para salir del bucle
	jne bucle			# JNE -> Jump if not equal, entonces volvera a iteracionar si no son iguales, en caso de ser iguales continua

	pop %edx			# Recuperamos el último valor introducido en la pila, el cual era el valor inicial de %edx
	ret				# return
