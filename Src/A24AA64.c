/*
 * A24AA64.c
 *
 *  Created on: 04.01.2018
 *      Author: MikolajP
 */
#include "A24AA64.h"
#include "stm32f1xx_hal.h"
#include "i2c.h"


void A24AA64_Init()
{
	uint8_t Buffor[20];

	Buffor[0]=10;	Buffor[1]=125;	Buffor[2]=0;	Buffor[3]=150;
	HAL_I2C_Mem_Write(&hi2c1,A24AA64_Address,20,2,Buffor,4,100);
	HAL_Delay(100);

	Buffor[0]=255;	Buffor[1]=255;	Buffor[2]=255;	Buffor[3]=0;
	HAL_I2C_Mem_Write(&hi2c1,A24AA64_Address,35,2,Buffor,4,100);
	HAL_Delay(100);

	Buffor[0]=10;	Buffor[1]=125;	Buffor[2]=0;	Buffor[3]=1;
	HAL_I2C_Mem_Write(&hi2c1,A24AA64_Address,40,2,Buffor,4,100);
	HAL_Delay(100);

	Buffor[0]=0x00;	Buffor[1]=0x08;	Buffor[2]=0xdc;	Buffor[3]=0xab;	Buffor[4]=0xcd;	Buffor[5]=0xcd;
	HAL_I2C_Mem_Write(&hi2c1,A24AA64_Address,50,2,Buffor,6,100);
	HAL_Delay(100);

	Buffor[0]=8;	Buffor[1]=8;	Buffor[2]=8;	Buffor[3]=8;
	HAL_I2C_Mem_Write(&hi2c1,A24AA64_Address,60,2,Buffor,4,100);
	HAL_Delay(100);

	Buffor[0]=1;
	HAL_I2C_Mem_Write(&hi2c1,A24AA64_Address,70,2,Buffor,1,100);
	HAL_Delay(100);
}

void A24AA64_Read()
{
	HAL_I2C_Mem_Read(&hi2c1,A24AA64_Address,20,2,A24AA64.IP,4,100);
	HAL_I2C_Mem_Read(&hi2c1,A24AA64_Address,35,2,A24AA64.SubMask,4,100);
	HAL_I2C_Mem_Read(&hi2c1,A24AA64_Address,40,2,A24AA64.Gateway,4,100);
	HAL_I2C_Mem_Read(&hi2c1,A24AA64_Address,50,2,A24AA64.MAC,6,100);
	HAL_I2C_Mem_Read(&hi2c1,A24AA64_Address,60,2,A24AA64.DNS,4,100);
	HAL_I2C_Mem_Read(&hi2c1,A24AA64_Address,70,2,&A24AA64.DHCP,1,100);
}
