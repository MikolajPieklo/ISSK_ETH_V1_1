/*
 * WIZ5500.c
 *
 *  Created on: 28.12.2017
 *      Author: mikolajp
 */

#include "WIZ5500.h"
#include "gpio.h"
#include "stm32f1xx_hal.h"
#include "spi.h"
#include "string.h"
#include "ModbusTCP_IP.h"

void WIZ5500_Init()
{

	HAL_GPIO_WritePin(W5500_RST_GPIO_Port,W5500_RST_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port,W5500_CS_Pin,GPIO_PIN_SET);

	HAL_GPIO_WritePin(USART1_DE_GPIO_Port,USART1_DE_Pin,GPIO_PIN_RESET);   //Driver Modbustro
	HAL_GPIO_WritePin(USART1_REn_GPIO_Port,USART1_REn_Pin,GPIO_PIN_RESET); //Driver Modbustro
	HAL_Delay(50);

	ModbusTCP.Register[0]='A'<<8|'T';ModbusTCP.Register[1]='R'<<8|'E';ModbusTCP.Register[2]='M'<<8|' ';ModbusTCP.Register[3]='S'<<8|'A';
	ModbusTCP.Register[4]='I'<<8|'S';ModbusTCP.Register[5]='S'<<8|'K';ModbusTCP.Register[6]='_'<<8|'E';ModbusTCP.Register[7]='T'<<8|'H';

	ModbusTCP.Register[10]='1'<<8|'.';ModbusTCP.Register[11]='0'<<8|'1';
	ModbusTCP.Register[12]='1'<<8|'.';ModbusTCP.Register[13]='0'<<8|'1';



	Main_WIZ5500.GateDefault.Gate0=192;
	Main_WIZ5500.GateDefault.Gate1=168;
	Main_WIZ5500.GateDefault.Gate2=199;
	Main_WIZ5500.GateDefault.Gate3=56;

	Main_WIZ5500.MaskSubNetwork.Mask0=255;
	Main_WIZ5500.MaskSubNetwork.Mask1=255;
	Main_WIZ5500.MaskSubNetwork.Mask2=255;
	Main_WIZ5500.MaskSubNetwork.Mask3=0;

	Main_WIZ5500.MAC.MAC0=0x68;
	Main_WIZ5500.MAC.MAC1=0x69;
	Main_WIZ5500.MAC.MAC2=0x60;
	Main_WIZ5500.MAC.MAC3=0x61;
	Main_WIZ5500.MAC.MAC4=0x62;
	Main_WIZ5500.MAC.MAC5=0x63;

	Main_WIZ5500.IP.IP0=192;
	Main_WIZ5500.IP.IP1=168;
	Main_WIZ5500.IP.IP2=199;
	Main_WIZ5500.IP.IP3=56;

	WIZ_SPI_DATA_SEND[0]=0x00;
	WIZ_SPI_DATA_SEND[1]=0x00;
	WIZ_SPI_DATA_SEND[2]=0x04;
	WIZ_SPI_DATA_SEND[3]=0x3A;//Common Register
	memcpy(WIZ_SPI_DATA_SEND+4,&Main_WIZ5500.GateDefault.Gate0,4);
	memcpy(WIZ_SPI_DATA_SEND+8,&Main_WIZ5500.MaskSubNetwork.Mask0,4);
	memcpy(WIZ_SPI_DATA_SEND+12,&Main_WIZ5500.MAC.MAC0,6);
	memcpy(WIZ_SPI_DATA_SEND+18,&Main_WIZ5500.IP.IP0,4);


	 /*HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_RESET);
	 HAL_SPI_Transmit(&hspi1,WIZ_SPI_DATA_SEND,22,100);
	 HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_RESET);*/
}

void WIZ5000_TEST() {
	uint8_t SPI_DATA_Send[5];
	uint8_t SPI_DATA_Recesive[5];

	SPI_DATA_Send[0] = 0x00;//Offset_Hi
	SPI_DATA_Send[1] = 0x39;//Offset_Lo
	SPI_DATA_Send[2] = 0x00;//BSB[7:3] RWB[2] OM[1:0]] OM=0x00 RWB 0=read
	SPI_DATA_Send[3] = 0x00;//Data

	HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, SPI_DATA_Send, SPI_DATA_Recesive, 4, 100);
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
}
