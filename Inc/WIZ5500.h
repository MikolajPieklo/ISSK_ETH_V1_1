/*
 * WIZ5500.h
 *
 *  Created on: 28.12.2017
 *      Author: mikolajp
 */

#include "stdint.h"

#ifndef WIZ5500_H_
#define WIZ5500_H_

#define SOCK_0 0 //TCP
#define SOCK_1 1
#define SOCK_2 2
#define SOCK_3 3
#define SOCK_4 4
#define SOCK_5 5
#define SOCK_6 6
#define SOCK_7 7 //DHCP
#define DATA_BUF_SIZE   2048
uint8_t gDATABUFDHCP[DATA_BUF_SIZE];

uint8_t WIZ_SPI_DATA_SEND[50];
uint8_t WIZ_SPI_DATA_RECESIVE[10];
typedef struct sIP{
	uint8_t IP0;
	uint8_t IP1;
	uint8_t IP2;
	uint8_t IP3;
}tIP;

typedef struct sMaskSubNetwork{
	uint8_t Mask0;
	uint8_t Mask1;
	uint8_t Mask2;
	uint8_t Mask3;
}tMaskSubNetwork;

typedef struct sMAC{
	uint8_t MAC0;
	uint8_t MAC1;
	uint8_t MAC2;
	uint8_t MAC3;
	uint8_t MAC4;
	uint8_t MAC5;
}tMAC;

typedef struct sGateDefault{
	uint8_t Gate0;
	uint8_t Gate1;
	uint8_t Gate2;
	uint8_t Gate3;
}tGateDefault;

struct WIZ5500{
	tIP IP;
	tMaskSubNetwork MaskSubNetwork;
	tGateDefault GateDefault;
	tMAC MAC;
}Main_WIZ5500;


void WIZ5500_INIT();
void WIZ5000_TEST();

#endif /* WIZ5500_H_ */
