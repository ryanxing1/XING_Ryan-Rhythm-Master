#include <stdbool.h>  

#include "ece198.h"

void InitializePin(GPIO_TypeDef *port, uint16_t pins, uint32_t mode, uint32_t pullups, uint8_t alternate)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = pins;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = pullups;
    GPIO_InitStruct.Alternate = alternate;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

UART_HandleTypeDef UART_Handle;  

HAL_StatusTypeDef SerialSetup(uint32_t baudrate)
{
    __USART2_CLK_ENABLE();     
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    // pin 2 is serial RX
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // pin 3 is serial TX (most settings same as for RX)
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // configure the serial port
    UART_Handle.Instance = USART2;
    UART_Handle.Init.BaudRate = baudrate;
    UART_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    UART_Handle.Init.StopBits = UART_STOPBITS_1;
    UART_Handle.Init.Parity = UART_PARITY_NONE;
    UART_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UART_Handle.Init.Mode = UART_MODE_TX_RX;

    return HAL_UART_Init(&UART_Handle);
}

// wait for a character to arrive from the serial port, then return it

char SerialGetc()
{
    while ((UART_Handle.Instance->SR & USART_SR_RXNE) == 0);                             
    return UART_Handle.Instance->DR;
}

// send a single character out the serial port

void SerialPutc(char c)
{
    while ((UART_Handle.Instance->SR & USART_SR_TXE) == 0); 
    // wait for transmitter buffer to be empty
    UART_Handle.Instance->DR = c;  // send the character
}

// write a string of characters to the serial port

void SerialPuts(char *ptr)
{
    while (*ptr)
        SerialPutc(*ptr++);
}




