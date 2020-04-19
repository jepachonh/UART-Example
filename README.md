## UART-Example
Este repositorio contiene el ejemplo paso a paso de como configurar la comunicacion serial UART de la tarjeta de desarrollo STM32 NUCLEO-L476RG

# Ejercicio : Trasmitir datos por UART.
En este ejercicio se mostrara como habilitar un puerto UART para transmitir datos por puertos COM. Existen tres formas para hacer esto, Polling, interrupcion o  accediendo a la memoria DMA, para este ejemplo utilizaremos el caso mas basico que es a traves del Polling. 

En este caso se va a utilizar el puerto PA los pines 2 y3, debido a que estos pines se puede configurar USART2 (para enviar datos a traves de ST-LINK sin necesidad de cables/conexiones extras.)

## Pasos
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
Paso 1. Enable USART clock en RCC register.
Para habilitar el clock que se encarga del USART es necesario conocer cual es el BUS de datos que conecta este modulo para setear el registro correspondiente. Para ver esto, se debe mirar en el manual o datasheet del micro (STM32L476RGT6U) Tabla 19.

If you want to embed images, this is how you do it:

![Image of Yaktocat](https://octodex.github.com/images/yaktocat.png)

