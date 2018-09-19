.section .data

	lista:
		.int 0xFFFFFFFF, 0XFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF		# Esta es la lista de valores con acarreo

	longlista:	.int (.-lista)/4														# Aqui conseguimos saber la longitud exacta de la lista ya que al dividir por cuatro lo que hacemos es ir termino a termino ya que cada termino de la lista es in int que son 4 bytes (32bits)
	resultado:	.quad 0x0123456789abcdef										# Declaramos nuestra variable resultado de 64 bits (8 bytes), que es un quad
	imprimir:	.ascii "res= %lld = %llx hex\n\n"							# Con esta variable imprimiremos el resultado en ascii por lo que tiene que acabar en \n

.section .text
	.extern printf																					# Con esto podremos usar el printf

	.global main																						# Sección main
	main:
		mov $lista, %ebx							# ebx: movemos a este registro el inicio de nuestra lista
		mov longlista, %ecx						# ecx: guardamos nuestra longitud de lista en este registro
		call suma											# Llamada a nuestra función suma la cual esta declarada luego
		movl  %eax,resultado					# Guardamos el resultado de los 32 bits menos significativos(eax)en resultado
		movl %edx,resultado+4					# Aquí guardamos los acarreos de 32 bits mas significativos(edx) del resultado

		# Acontinuación metemos todos los registros que posteriormente imprimiremos con la función printf
		pushl resultado+4		# Para poder imprimir el resultado tenemos que guardar en primer lugar los bits más significativos que se mostraran primero
		pushl resultado			# Ahora guardamos los menos significativos los que imprimira después
		pushl resultado+4		# Los guardamos dos veces ya que los mostraremos como hexadecimal y como long int
		pushl resultado
		pushl $imprimir			# Por último, guardamos lo primero que aparecera y nos dará el formato
		call printf					# Llamamos a la función printf
		add $20, %esp				# Devolvemos %esp alvalor original, restauramos la pila al valor original

		#SALIDA DEL PROGRAMA CORRECTA
		mov $1, %eax
		mov $0, %ebx
		int $0x80

	suma:
		#ponemos a cero los registros utilizados
		mov $0, %eax        # Valor sin acarreo
		mov $0, %edx        # Acarreo
		mov $0, %edi        # Contador del bucle
		bucle:
      add (%ebx,%ebp,4), %eax	  # eax: guardamos el valor de la suma de los terminos de la lista de la siguiente forma ebx + ebp*4
			adc $0, %edx					    # En caso de haber acarreo lo sumamos de la siguiente manera
			incl	%edi						    # Aumentamos el contador
      cmpl  %edi,%ecx						# Comparamos para ver si hemos llegado al final del bucle
			jne bucle									# Seguimos iterando mientras no sean iguales con la instrucción anterior

		ret
