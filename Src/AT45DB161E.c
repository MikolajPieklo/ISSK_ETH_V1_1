/*
 * AT45DB161E.c
 *
 *  Created on: 04.01.2018
 *      Author: MikolajP
 */
#include "AT45DB161E.h"
#include "stm32f1xx_hal.h"
#include "spi.h"

void AT45DB161E_Init()
{
	//HAL_GPIO_WritePin(W5500_CS_GPIO_Port,W5500_CS_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(EX_MEMORY_CS_GPIO_Port,EX_MEMORY_CS_Pin,GPIO_PIN_SET);
	HAL_SPI_Init(&hspi2);
	HAL_SPI_MspInit(&hspi2);

}

void AT45DB161E_Chip_Erase()
{
	uint8_t Buffor[4];
	Buffor[0]=0xC7;
	Buffor[1]=0x94;
	Buffor[2]=0x80;
	Buffor[3]=0x9A;
	HAL_GPIO_WritePin(EX_MEMORY_CS_GPIO_Port,EX_MEMORY_CS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2,Buffor,4,100);
	HAL_GPIO_WritePin(EX_MEMORY_CS_GPIO_Port,EX_MEMORY_CS_Pin,GPIO_PIN_SET);
}

void AT45DB161E_Enable_Sector_Protection()
{
	uint8_t Buffor[4];
	Buffor[0]=0x3D;
	Buffor[1]=0x2A;
	Buffor[2]=0x7F;
	Buffor[3]=0xA9;
	HAL_GPIO_WritePin(EX_MEMORY_CS_GPIO_Port,EX_MEMORY_CS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2,Buffor,4,100);
	HAL_GPIO_WritePin(EX_MEMORY_CS_GPIO_Port,EX_MEMORY_CS_Pin,GPIO_PIN_SET);
}
void AT45DB161E_Disable_Sector_Protection()
{
	uint8_t Buffor[4];
		Buffor[0]=0x3D;
		Buffor[1]=0x2A;
		Buffor[2]=0x7F;
		Buffor[3]=0x9A;
		HAL_GPIO_WritePin(EX_MEMORY_CS_GPIO_Port,EX_MEMORY_CS_Pin,GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi2,Buffor,4,100);
		HAL_GPIO_WritePin(EX_MEMORY_CS_GPIO_Port,EX_MEMORY_CS_Pin,GPIO_PIN_SET);
}

uint16_t AT45DB161E_Status_Register()
{
	uint8_t Buffor[4];
	Buffor[0]=0xD7;
	uint8_t ReadBuffor[4];

	HAL_GPIO_WritePin(EX_MEMORY_CS_GPIO_Port,EX_MEMORY_CS_Pin,GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2,Buffor,ReadBuffor,4,100);
	HAL_GPIO_WritePin(EX_MEMORY_CS_GPIO_Port,EX_MEMORY_CS_Pin,GPIO_PIN_SET);

	return 0;
}

void AT45DB161E_Device_ID()
{
	uint8_t Buffor[5];
	Buffor[0]=0x9F;
	uint8_t ReadBuffor[5];

	HAL_GPIO_WritePin(EX_MEMORY_CS_GPIO_Port,EX_MEMORY_CS_Pin,GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_SPI_TransmitReceive(&hspi2,Buffor,ReadBuffor,5,100);
	HAL_Delay(10);
	HAL_GPIO_WritePin(EX_MEMORY_CS_GPIO_Port,EX_MEMORY_CS_Pin,GPIO_PIN_SET);
}
