#include "stdint.h"
#include "stdbool.h"
#

#define Modbus_Address_Device     0x64
#define Modbus_Function_Write     0x10
#define Modbus_Function_Read      0x03
#define Modbus_Data_OFFSET        0x00

#define Modbus_Offset_IP        20
#define Modbus_Offset_SubMask   30
#define Modbus_Offset_Gateway   40
#define Modbus_Offset_Mac       50
#define Modbus_Offset_DHCP      60
#define Modbus_Offset_DNS       70


void Modbus_Main(void);
uint8_t Modbus_Flaga_DMA_Rx;
uint8_t Modbus_Counter_DMA_Tx;
uint8_t Modbus_Counter_DMA_Tx_index;

uint8_t Modbus_Buffor_Temp;
uint8_t Modbus_Buffor_Tab[20];
uint8_t Modbus_index;

uint16_t CRC16 (const uint8_t *nData, uint16_t wLength);
void ModbusWrite(uint8_t* SendBuffor,uint8_t len);
