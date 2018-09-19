.section .data

  lista:
    .int 10, 30, 20, 50		# Esta es la lista de valores con acarreo

  longlista:	.int (.-lista)/4
  # Ponemos a 0 todas las variables que usaremos
  cociente:	  .int 0
  resto:      .int 0
  dividendo:  .int 0
  divisor:    .int 0
  imprimir:	.ascii "Resultado de la division %d entre %d: \n\tCociente: %d dec\t%x hex\n\tResto: %d dec\t%x hex\n\n"

.section .text
	.extern printf																					# Con esto podremos usar el printf

.global main
main:
  mov $lista, %ebx							# ebx: movemos a este registro el inicio de nuestra lista
  mov longlista, %ecx						# ecx: guardamos nuestra longitud de lista en este registro
  call suma											# Llamada a nuestra función suma la cual esta declarada luego
	mov  %eax,cociente		        # Cociente: Nuestro cociente esta en eax y lo sacamos con idiv y lo guardamos en cociente
	movl %edx,resto			          # Resto: resto de la division con idiv

	#metemos en la pila los parametros de printf
	push resto                    # Hexadecimal
	push resto                    # Decimal
	push cociente                 # Hexadecimal
	push cociente                 # Decimal
	push divisor                  # Decimal
	push dividendo                # Decimal
	push $imprimir
	call printf			             	# Llamada a printf
	add $28, %esp			            # Devolvemos %esp alvalor original, restauramos la pila al valor original

	#SALIDA DEL PROGRAMA
	mov $1, %eax   #Llamada a exit()
	mov $0, %ebx   #codigo a retornar en la salida
	int $0x80      #interrupcion para llamada a exit

suma:
  pushl %ebp		# Guardamos el valor de ebp para restaurarlo al salir de la funcion
  # Ponemos a cero los registros de la siguiente manera
  mov $0, %esi		#esi: 32 bits menos significativos de la suma total
  mov $0, %edi		#edi: 32 bits mas significativos(extension del signo) de la suma total

bucle:
  movl (%ebx,%ebp,4), %eax	# eax: guardamos el valor de la suma de los terminos de la lista de la siguiente forma ebx + ebp*4
  cltd											# edx:eax, con esta funcion extendemos el signo %edx:%eax
  addl %eax, %esi						# esi: acumulamos los valores de eax sin acarreo, suma en 32 bits menos significativos
  adc %edx, %edi						# edi: acumulador del acarreo, en caso de que exista, 32 bits más significativos
  incl	%ebp								# Aumentamos el contador del bucle
	cmpl  %ebp,%ecx						# Comparamos para ver si hemos llegado al final del bucle
  jne bucle									# Seguimos iterando mientras no sean iguales con la instrucción anterior
	movl %esi, %eax           # Guardamos esi en eax
	movl %edi, %edx           # Guardamos edi en edx

	# Ahora dividimos nuestro resultado edx:eax entre ecx, se guardara el cociente en eax y el resto en edx, y lo mostramos por pantalla  (idiv ecx)
	movl %eax, dividendo      # Colocamos el dividendo, suma total despues del bucle
	movl %ecx, divisor        # El divisor, nuestra variable longlista
	idiv %ecx                 # Dividimos con signed divided, con esto se guarda en edx el resto

  popl %ebp									# Restauramos el valor de %ebp
	ret                       #retornamos eax
