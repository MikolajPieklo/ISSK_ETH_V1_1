#include "Modbus.h"
#include "stm32f1xx_hal.h"
#include "usart.h"



uint16_t CRC16 (const uint8_t *nData, uint16_t wLength)
{
static const uint16_t wCRCTable[] = {
0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };
uint8_t nTemp;
uint16_t wCRCWord = 0xFFFF;
while (wLength--)
{
nTemp = *nData++ ^ wCRCWord;
wCRCWord >>= 8;
wCRCWord ^= wCRCTable[nTemp];
}
return wCRCWord;
}


void ModbusWrite(uint8_t* SendBuffor,uint8_t len){
	volatile uint16_t tempCRC;
	HAL_GPIO_WritePin(USART1_DE_GPIO_Port,USART1_DE_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(USART1_REn_GPIO_Port,USART1_REn_Pin,GPIO_PIN_SET);
	tempCRC=CRC16(SendBuffor,len);
	SendBuffor[len]=(tempCRC&0x00FF);
	SendBuffor[len+1]=(tempCRC>>0x08);

	HAL_UART_Transmit_DMA(&huart2,SendBuffor,len+2);
	HAL_Delay(2);
	//SS_RS_485_LED_R_OFF();
}

void Modbus_Main() {
	if(Modbus_Flaga_DMA_Rx==true){
		Modbus_Buffor_Tab[Modbus_index] = Modbus_Buffor_Temp;
		Modbus_index++;
		if(Modbus_Buffor_Tab[0]!=Modbus_Address_Device) {Modbus_index=0;Modbus_Flaga_DMA_Rx=false;return ;} //Czyszczenie bufora
		//if(Modbus_Buffor_Tab[1]!=Modbus_Function_Read && Modbus_index >2 ) {Modbus_index=0;Modbus_Flaga_DMA_Rx=false;SS_RS_485_LED_G_OFF();return ;}
		//if(Modbus_Buffor_Tab[1]!=Modbus_Function_Write&& Modbus_index >2 ) {Modbus_index=0;Modbus_Flaga_DMA_Rx=false;SS_RS_485_LED_G_OFF();return ;}
		if (Modbus_Buffor_Tab[1] == Modbus_Function_Write && Modbus_index >= 11) { //Ramka Write
			uint16_t DataAdd = (Modbus_Buffor_Tab[2] << 8)	| Modbus_Buffor_Tab[3]; //The Data Address of the first register
			uint16_t NrReg2W = (Modbus_Buffor_Tab[4] << 8)	| Modbus_Buffor_Tab[5]; //The number of registers to write
			uint8_t n = 9 + 2*NrReg2W - 2;
			volatile uint16_t tCRC16 = CRC16(Modbus_Buffor_Tab, n);
			if (((tCRC16 & 0xFF) == Modbus_Buffor_Tab[n])	&& ((tCRC16 >> 8) == Modbus_Buffor_Tab[n + 1])) {
				uint8_t BuforACK[8];
				memcpy(BuforACK,Modbus_Buffor_Tab,6);

				ModbusWrite(BuforACK, 6);
				uint8_t i=0;
				for(i=0;i<NrReg2W;i++){
					//Modbus_Register1[Modbus_Data_OFFSET + DataAdd +i]=(Modbus_Buffor_Tab[7+2*i]<<8)|Modbus_Buffor_Tab[8+2*i];
				}
				/*if(Modbus_Register1[0]==1)NVIC_SystemReset();
				if(Modbus_Register1[2]!=0){
					SYS_COMMAND_Silnik_Lewo=true; SYS_COMMAND_Silnik_Prawo=false;}		//kierunek
				if(Modbus_Register1[2]==0){
					SYS_COMMAND_Silnik_Lewo=false;SYS_COMMAND_Silnik_Prawo=true;}		//kierunek
				g_time_softstart = Modbus_Register1[3];															//TSS
				basic_alfa = Modbus_Register1[10];																//Alfa
				if(Modbus_Register1[11]!=0){SYS_COMMAND_Start=true;	SYS_COMMAND_Stop=false;}					//Start
				if(Modbus_Register1[11]==0){SYS_COMMAND_Start=false;SYS_COMMAND_Stop=true;}						//Start
				L1.fuse_U_max=Modbus_Register1[12];L2.fuse_U_max=Modbus_Register1[12];L3.fuse_U_max=Modbus_Register1[12];	//fuse_U_max
				L1.fuse_U_min=Modbus_Register1[14];L2.fuse_U_min=Modbus_Register1[14];L3.fuse_U_min=Modbus_Register1[14];	//fuse_U_min
				uint16_t *p1,*p2,*p3;
				p1=&L1.fuse_I_nominalny;      p2=&L2.fuse_I_nominalny;       p3=&L3.fuse_I_nominalny;					//fuse_Iznam
				*p1=Modbus_Register1[50];     *p2=Modbus_Register1[50];      *p3=Modbus_Register1[50];
				*(p1+1)=Modbus_Register1[51]; *(p2+1)=Modbus_Register1[51];  *(p3+1)=Modbus_Register1[51];
				p1=&L1.fuse_I_zwarciowy;      p2=&L2.fuse_I_zwarciowy;       p3=&L3.fuse_I_zwarciowy;					//fuse_Imax
				*p1=Modbus_Register1[52];     *p2=Modbus_Register1[52];      *p3=Modbus_Register1[52];
				*(p1+1)=Modbus_Register1[53]; *(p2+1)=Modbus_Register1[53];  *(p3+1)=Modbus_Register1[53];
				p1=&L1.fuse_I_rozruchowy;     p2=&L2.fuse_I_rozruchowy;      p3=&L3.fuse_I_rozruchowy;				    //fuse_Iroz
				*p1=Modbus_Register1[54];     *p2=Modbus_Register1[54];      *p3=Modbus_Register1[54];
				*(p1+1)=Modbus_Register1[55]; *(p2+1)=Modbus_Register1[55];  *(p3+1)=Modbus_Register1[55];
				State_Output = Modbus_Register1[32];*/
				Modbus_index = 0;
			}
		}

		if (Modbus_index == 8 && Modbus_Buffor_Tab[1]==Modbus_Function_Read) //Ramka Read
		{
			volatile uint16_t tCRC16 = CRC16(Modbus_Buffor_Tab, 6);
			if (((tCRC16 & 0xFF) == Modbus_Buffor_Tab[6]) && ((tCRC16 >> 8) == Modbus_Buffor_Tab[7])) {
				uint8_t BuforACK[60];
				uint16_t DataAdd = (Modbus_Buffor_Tab[2] << 8)	| Modbus_Buffor_Tab[3]; //The Data Address of the first register
				uint16_t NrReg2R = (Modbus_Buffor_Tab[4] << 8)	| Modbus_Buffor_Tab[5]; //The total number of registers requested
				uint8_t n = 3+2*NrReg2R;

				/*uint16_t *p;
				p=&L3.fuse_I_nominalny;  Modbus_Register1[50]= *p; Modbus_Register1[51]= *(p+1);
				p=&L3.fuse_I_zwarciowy;  Modbus_Register1[52]= *p; Modbus_Register1[53]= *(p+1);
				p=&L3.fuse_I_rozruchowy; Modbus_Register1[54]= *p; Modbus_Register1[55]= *(p+1);
				Modbus_Register1[12]= L3.fuse_U_max;
				Modbus_Register1[14]= L3.fuse_U_min;
				if(SYS_COMMAND_Silnik_Lewo==true)  Modbus_Register1[2]= 1;
				if(SYS_COMMAND_Silnik_Prawo==true) Modbus_Register1[2]= 0;
				Modbus_Register1[3]= g_time_softstart;
				Modbus_Register1[10]= basic_alfa;
				Modbus_Register1[11]= SYS_COMMAND_Start;
				Modbus_Register1[30]= State_Input;
				Modbus_Register1[32]= State_Output;*/

				memcpy(BuforACK,Modbus_Buffor_Tab,2);
				BuforACK[2] = 2*NrReg2R;

				for(uint8_t i=0;i<NrReg2R;i++){
					if(Modbus_Buffor_Tab[3]>=10){
						//BuforACK[3+2*i] = *Modbus_Register1[Modbus_Data_OFFSET+DataAdd+i]>>8;
						//BuforACK[4+2*i] = *Modbus_Register1[Modbus_Data_OFFSET+DataAdd+i]&0xFF;
					}
					if(Modbus_Buffor_Tab[3]<10){
						//BuforACK[3+2*i] = 0;
						//BuforACK[4+2*i] = *Modbus_Register1[Modbus_Data_OFFSET+DataAdd+i]&0xFF;
					}
				}
				ModbusWrite(BuforACK, n);
			}
			Modbus_index = 0;
		}
		Modbus_Flaga_DMA_Rx=false;
	}
}
