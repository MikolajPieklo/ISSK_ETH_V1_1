/*
 * ModbusTCP_IP.h
 *
 *  Created on: 29.12.2017
 *      Author: mikolajp
 */

#ifndef MODBUSTCP_IP_H_
#define MODBUSTCP_IP_H_

#define ModbusTCP_IP_ID 0x17
#define ModbusTCP_IP_F_Write 0x10
#define ModbusTCP_IP_F_Read  0x03


struct sModbusTCP
{
	uint8_t BufforTemp;
	uint8_t Buffor[50];
	uint16_t Register[100];
	uint16_t Data_Offset;
	uint16_t Data_Len;
	uint8_t  Data_Nr_Of_Bytes;
	uint8_t index;
	uint16_t Transaction_ID;
}ModbusTCP;

#endif /* MODBUSTCP_IP_H_ */
