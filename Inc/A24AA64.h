/*
 * A24AA64.h
 *
 *  Created on: 04.01.2018
 *      Author: MikolajP
 */
#include "stdint.h"
#include "stdbool.h"


#ifndef A24AA64_H_
#define A24AA64_H_

#define A24AA64_Address 160

#define A24AA64_Offset_IP		0
#define A24AA64_Offset_SubMask  4
#define A24AA64_Offset_Gateway  8
#define A24AA64_Offset_MAC      12
#define A24AA64_Offset_DHCP     18
#define A24AA64_Offset_DNS      19

void A24AA64_Init();
void A24AA64_Read();

struct sA24AA64
{
	uint8_t Offset[2]; //Address_Offset, bytes quantity
	uint8_t IP[4];
	uint8_t SubMask[4];
	uint8_t Gateway[4];
	uint8_t MAC[6];
	uint8_t DHCP;
	uint8_t DNS[4];
}A24AA64;


#endif /* A24AA64_H_ */
