# UART-Example
Este repositorio contiene el ejemplo paso a paso de como configurar la comunicacion serial UART de la tarjeta de desarrollo STM32 NUCLEO-L476RG

# Ejercicio : Trasmitir datos por UART.
En este ejercicio se mostrara como habilitar un puerto UART para transmitir datos por puertos COM. Existen tres formas para hacer esto, Polling, interrupcion o  accediendo a la memoria DMA, para este ejemplo utilizaremos el caso mas basico que es a traves del Polling. 

En este caso se va a utilizar el puerto PA los pines 2 y3, debido a que estos pines se puede configurar USART2 (para enviar datos a traves de ST-LINK sin necesidad de cables/conexiones extras.)

## Pasos.
 *   Enable USART clock en RCC register.
 *   Enable GPIO clock en RCC register.
 *   Set PA2 y PA3 como alternate pin en MODER register.
 *   Set PA2 y PA3 para high speed mode en OSPEEDR register
 *   Set PA2 and PA3 as AF7 (AF7 define el modo USART).
 *   Set UART word length and parity en CR1 register.
 *   Enable transmit and receive (TE/RE bits) en CR1 register.
 *   Enable stop bits (STOP bits) en CR2 register.
 *   Calculate baud rate and set BRR register.
 *   Enable UART.

## Desarollo paso a paso.
### Paso 1. Enable USART clock en RCC register.
Section 6.4 del manual de referencia.

Para habilitar el clock que se encarga del USART es necesario conocer cual es el BUS de datos que conecta este modulo para setear el registro correspondiente. Para ver esto, se debe mirar en el manual o datasheet del micro (STM32L476RGT6U) Tabla 19.

![Imagen tabla 19](https://github.com/jepachonh/UART-Example/blob/master/pictures/clock%20usart.png)

Conociendo que registro se debe modificar, vamos a la seccion 6.4 RCC registers del manual de referencia de la tarjeta para ver como se debe modificar el registro APB1 y habilitarlo.

![Registro RCC-APB1](https://github.com/jepachonh/UART-Example/blob/master/pictures/enclkusart.png)

Para habilitar USART2 es necesario establecer un 1 en el bit 17 del registro.

### Paso 2. Enable GPIO clock en RCC register.
Section 6.4 del manual de referencia.

Para habilitar el clock que se encarga de los perifericos GPIO es necesario conocer cual es el BUS de datos que conecta este modulo para setear el registro correspondiente. Para ver esto, se debe mirar en el manual o datasheet del micro (STM32L476RGT6U) Tabla 19.

![Imagen tabla 19](https://github.com/jepachonh/UART-Example/blob/master/pictures/clock%20GPIOA.png)

Conociendo que registro se debe modificar, vamos a la seccion 6.4 RCC registers del manual de referencia de la tarjeta para ver como se debe modificar el registro AHB2 y habilitarlo.

![Registro RCC-AHB2](https://github.com/jepachonh/UART-Example/blob/master/pictures/enclkgpio.png)

Para habilitar GPIOA es necesario establecer un 1 en el bit 0 del registro.

### Paso 3. Set PA2 y PA3 como alternate pin en MODER register.
Section 8.4 del manual de referencia.

El registro MODER es el encargado de definir como va a trabajar el pin elegido, para nuestro caso de estudio, es necesario definir los pines 2 y 3 del puerto A en la funcion alternate.

![Registro GPIOx-MODER](https://github.com/jepachonh/UART-Example/blob/master/pictures/moderA.png)

Para settear estos pines con la funcion alternate es necesario poner 10 en los bits 5-4 y 7-6.

### Paso 4. Set PA2 y PA3 como high speed mode pin en OSPEEDR register.
Section 6.4 del manual de referencia.

El registro OSPEEDR es el encargado de definir la velocidad de como va a trabajar el pin elegido, para nuestro caso de estudio, es necesario definir esta velocidad en los pines 2 y 3 del puerto A como high speed.

![Registro GPIOx-OSPEEDR](https://github.com/jepachonh/UART-Example/blob/master/pictures/OSPEED.png)

Para settear estos pines con la funcion high speed es necesario poner 10 en los bits 5-4 y 7-6.

### Paso 5. Set PA2 and PA3 as AF7 (AF7 define el modo USART).
Section 8.4 del manual de referencia.

Hay algunos I/O pines que pueden ser configurados de diferentes maneras debido a que pueden desarrollar diferentes tareas. Por ejemplo, en nuestro caso, al querer utilizar el USART2 que corresponde a los pines PA2 y PA3, podemos ver que estos pines ademas de poder ser configurados como GPIO, se pueden configurar tambien como Timmers y USART.

![PA2,PA3- Alternate Functions](https://github.com/jepachonh/UART-Example/blob/master/pictures/AF7.png)

De esta forma, es necesario configurar el registro AFR (Alternate function register), este registro esta dividido en dos partes -> AFRL y AFRH. El primero funciona para los primeros 8 puertos (PA0-PA7) y el segundo para los ultimos 8 puertos (PA8-PA15).

Como se puede evidenciar en la imagen anterior, para definir el modo USART en los PA2 y PA3, es necesario configurar AF7 en el registro.

![PA2,PA3- Alternate Functions](https://github.com/jepachonh/UART-Example/blob/master/pictures/AF7registro.png)

Para settear estos pines con AF7 es necesario poner 0111 (7hex) en el registro, en la parte correspondiente a los puertos 2 y 3.(AFSEL3,AFSEL2).

## Configuracion del modulo USART 
Section 40.8 del manual de referencia.

Para configurar el modulo UART es necesario configurar ciertos parametros que el protocolo de comunicacion exige:
inital bits, stop bits, message bits, parity (even,odd,none),HWcontrol, Baudrate.

Para configurar estos parametros es necesario modificar los siguientes registros: CR1,CR2 y BRR que son los registros encargados de manejar la comunicacion USART.

![USART configuration](https://github.com/jepachonh/UART-Example/blob/master/pictures/CR1.png)
![USART configuration](https://github.com/jepachonh/UART-Example/blob/master/pictures/CR2.png)
![USART configuration](https://github.com/jepachonh/UART-Example/blob/master/pictures/BRR.png)

### Paso 6.Set UART word length and parity en CR1 register.
Para modificar estos parametros se deben modificar los bits 28 o 12 para el word length y el bit 10 para el parity.

Para nuestro caso de estudio vamos a definir un mesaje de 8 bits y el protocolo de comunicacion no va a tener bit de paridad.

Para definir 8Bits, de debe poner 00 en el subregistro M1,M0 del CR1. 

### Paso 7.Enable transmit and receive (TE/RE bits) en CR1 register.
Para modificar estos parametros se deben modificar el bit 2  para habilitar el receptor y el bit 3 para habilitar el trasmisor.

Esto se hace poniendo el bit 2 y 3 en 1 en el subregistro RE/TE del CR1 register.

### Paso 8.Enable stop bits (STOP bits) en CR2 register.
Para modificar estos parametros se deben modificar los bits 13,12 para definir el numero de bits de parada. En nuestro caso de estudio se va a definir 1 bit de parada.

Para definir 1bit, se debe poner 00 en el subregistro STOP del CR2 register.

### Paso 9.Calculate baud rate and set BRR register.
Debido a que se va a utilizar UART es necesario configurar el baud rate de comunicacion debido a que la comunicacion va a ser asincrona.

Para hacer esta configuracion se debe hacer unos calculos teniendo en cuenta la frecuencia del reloj y el BAUD RATE al que se quiera trasmitir/recibir.

![BaudRate configuration](https://github.com/jepachonh/UART-Example/blob/master/pictures/Baudrate.png)

En nuestro caso de estudio, se desea obtener un baud rate de 9600bps.

### Paso 10.Enable UART.
Para habilitar el uso del USART se debe modificar el bit 0, subregistro UE del registro CR1. Para habilitar, se debe establecer un 1 en este bit. 

## Codigo ejemplo.
Este codigo transmite por consola el mensaje Hello world. los pasos estan descritos y explicados de acuerdo a la explicacion anterior.
~~~
include "stm32l476xx.h"
int main(void)
{
	// Enable GPIOA Peripheral Clock (bit 0 in AHB2ENR register)
	RCC->AHB2ENR |= (1 << 0);
	//Enable peripheral clock for USART2 (bit 17 in 1 in APB1ENR1 register)
	RCC->APB1ENR1 |= (1 << 17);

	// Make GPIOA Pin2,3 as alternate pin (bits 1:0 in MODER register)
	GPIOA->MODER &= 0xABFFFF0F;		//clear bits 4,5,6,7 for P2 and P3
	GPIOA->MODER |= 0x000000A0;		//Write 10 to bits 4,5,6,7 for P2 and P3

	// Set GPIOA Pin2,3 in high speed mode
	GPIOA->OSPEEDR |= 0x000000A0;		//Write 10 to bits 4,5,6,7 for P2 and P3

	// Choose AF7 for USART2 in alternate function registers
	GPIOA->AFR[0] |= 0x7700;

	//UART configuration

	// Set USART2 word length
	USART2->CR1 |= 0x00001000;		//word length define with 8bits
	// Set USART2 Parity control bit
	USART2->CR1 |= (0 << 10);
	// Set USART2 stop bits
	USART2->CR2 |= (0b00 << 12);
    // USART2 tx enable, TE bit 3
    USART2->CR1 |= (1 << 3);
    // USART2 rx enable, RE bit 2
    USART2->CR1 |= (1 << 2);
    // Set Baud Rate at 9600Bps, internal clock 8Mhz by default
    /*
     * formula para calcular el Baudrate
     * TX/RX BR = Fclk/(USARTDIV)
     * 8MHz internal clock by default
     * TX/RX BR = 8000000/9600=833.33
     * 833.33d = 0341hex
     */
    USART2->BRR |= 0x0341;
    // Enable USART2  UE, bit 0
    USART2->CR1 |= (1 << 0);

   const uint8_t mensaje[] = "hello world\n\r";

    while(1)
    {
    	for (uint32_t i=0; i<sizeof(mensaje); i++){
    		// send character
    		USART2->TDR = mensaje[i];
    		// wait for transmit complete
    		while(!(USART2->ISR & (1 << 6)));
    		// slow down
    		for(int i=0; i<1000000; i++);
    	}
    }

    __asm__("NOP"); // Assembly inline can be used if needed
    return 0;
}
~~~

## Ejemplo codigo usando libreria HAL
Para este ejemplo se utilizo la libreria Hal  que provee el STM32CubeIDE para configurar el modulo USART2 como UART y trasmitir un mensaje por consola.

Para esto se deben seguir los siguientes pasos.

## Pasos
*   Abrir el Cube ioc para configurar los pines.
*   General el codigo.
*   Hacer uso de las librerias para transmitir un mensaje.

# Paso 1.Abrir el Cube ioc para configurar los pines.
En esta parte, se deben configurar el USART y sus parametros, y habilitar los pines del GPIOA.

![CubeIDE IOC configuration](https://github.com/jepachonh/UART-Example/blob/master/pictures/IOC.png)

# Paso 1. General el codigo.
Para generar el codigo luego de haber configurado los parametros, se debe guardar el archivo y dar clic en la opcion _Device Configuration Tool Code Generation_.

Una vez generado el codigo, se procede a abrir el archivo main.c que contiene el las configuraciones dispuesta en las siguientes funciones.
~~~
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
~~~
Al ver la funcion que contiene las configuraciones de UART podemos ver que todos los parametros estan adecuadamente configurados como se habian definido en la CubeIDE IOC tool.

~~~
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
~~~

# Paso 3. Hacer uso de las librerias para transmitir un mensaje.
En el codigo principal, dentro del while(1) del int main(void) function, se puede definir la trasmision de datos usando las funciones de la libreria HAL.
~~~
HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)

huart --> es el canal de trasmision/recepcion. Para este caso seria huart2
pData --> es el mensaje que se va a transmitir.
Size --> es el tamaño/longitud del mensaje que se va a transmitir.
Timeout --> Tiempo que se espera para que se termine la trasmision.
~~~
Ejemplo.

HAL_UART_Transmit(&huart2, "Hello world\n\r",13, HAL_MAX_DELAY);


## Tarea. 
Configurar otro de los modulos UART/USART para trasmitir datos.

## Descargar ejemplo
En el enlace [UART-Example](https://github.com/jepachonh/UART-Example) puede descargar la carpeta con los ejercicios.



