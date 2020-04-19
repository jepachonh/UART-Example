#include "stm32l476xx.h"

/*************************************************
 * function declarations
 *************************************************/

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




