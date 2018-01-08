/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "Modbus.h"
#include "WIZ5500.h"
#include "wizchip_conf.h"
#include "w5500.h"
#include "socket.h"
#include "ModbusTCP_IP.h"
#include "dhcp.h"
//#include "AT45DB161E.h"
#include "A24AA64.h"

volatile uint32_t SysTickTimer=0;
// DHCP //
//uint8_t netconfig = NETINFO_DHCP;
uint8_t netconfig = NETINFO_STATIC;

void cs_sel() {	HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);} //CS LOW
void cs_desel() {HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);} //CS HIGH
uint8_t spi_rb(void) {
	uint8_t rbuf;
	HAL_SPI_Receive(&hspi2, &rbuf, 1, 0xFFFFFFFF);
	return rbuf;
}
void spi_wb(uint8_t b) {HAL_SPI_Transmit(&hspi2, &b, 1, 0xFFFFFFFF);}

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/*if(GPIO_Pin == WIZ_INT_Pin)
	{

	}*/
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		HAL_GPIO_WritePin(USART1_DE_GPIO_Port, USART1_DE_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(USART1_REn_GPIO_Port, USART1_REn_Pin, GPIO_PIN_RESET);
		HAL_NVIC_DisableIRQ(USART2_IRQn);
	}
}

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	uint8_t retVal, sockStatus;
	uint8_t dhcp_ret;
	int16_t rcvLen;
	uint8_t rcvBuf[20], bufSize[] = {2, 2, 2, 2};
	reg_wizchip_cs_cbfunc(cs_sel, cs_desel);
	reg_wizchip_spi_cbfunc(spi_rb, spi_wb);

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  //MX_I2C2_Init();
  //MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  //MX_USART2_UART_Init();
  //MX_USART3_UART_Init();

  /* USER CODE BEGIN 2 */
  HAL_I2C_MspInit(&hi2c1);
  HAL_UART_MspInit(&huart1);
  HAL_I2C_ClearBusyFlagErrata_2_14_7(&hi2c1);
  //HAL_UART_MspInit(&huart2);
  //HAL_UART_MspInit(&huart3);

  HAL_UART_Receive_DMA(&huart1,&Modbus_Buffor_Temp,1);
  //A24AA64_Init();

  A24AA64_Read();

  WIZ5500_Init();
  wizchip_init(bufSize, bufSize);//  ctlwizchip(CW_INIT_WIZCHIP,)
  wiz_NetInfo netInfo = { .mac 	= {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xcd},	// Mac address
                          .ip 	= {10, 125, 0, 150},					// IP address
                          .sn 	= {255, 255, 255, 0},					// Subnet mask
                          .gw 	= {10, 125, 0, 1},					// Gate address
    						.dhcp   = NETINFO_STATIC};//NETINFO_STATIC
  netInfo.ip[0]=A24AA64.IP[0];	netInfo.ip[1]=A24AA64.IP[1];  netInfo.ip[2]=A24AA64.IP[2];  netInfo.ip[3]=A24AA64.IP[3];
  netInfo.sn[0]=A24AA64.SubMask[0]; netInfo.sn[1]=A24AA64.SubMask[1];  netInfo.sn[2]=A24AA64.SubMask[2];  netInfo.sn[3]=A24AA64.SubMask[3];
  netInfo.gw[0]=A24AA64.Gateway[0]; netInfo.gw[1]=A24AA64.Gateway[1];  netInfo.gw[2]=A24AA64.Gateway[2];  netInfo.gw[3]=A24AA64.Gateway[3];
  netInfo.mac[0]=A24AA64.MAC[0]; netInfo.mac[1]=A24AA64.MAC[1]; netInfo.mac[2]=A24AA64.MAC[2]; netInfo.mac[3]=A24AA64.MAC[3];
  netInfo.mac[4]=A24AA64.MAC[4]; netInfo.mac[5]=A24AA64.MAC[5];
  netInfo.dns[0]=A24AA64.DNS[0]; netInfo.dns[1]=A24AA64.DNS[1];  netInfo.dns[2]=A24AA64.DNS[2]; netInfo.dns[3]=A24AA64.DNS[3];
  netInfo.dhcp=A24AA64.DHCP;

  ModbusTCP.Register[20]=A24AA64.IP[0]<<8|A24AA64.IP[1];
  ModbusTCP.Register[21]=A24AA64.IP[2]<<8|A24AA64.IP[3];
  ModbusTCP.Register[30]=A24AA64.SubMask[0]<<8|A24AA64.SubMask[1];
  ModbusTCP.Register[31]=A24AA64.SubMask[2]<<8|A24AA64.SubMask[3];
  ModbusTCP.Register[40]=A24AA64.Gateway[0]<<8 |A24AA64.Gateway[1];
  ModbusTCP.Register[41]=A24AA64.Gateway[2]<<8|A24AA64.Gateway[3];
  ModbusTCP.Register[50]=A24AA64.MAC[0]<<8|A24AA64.MAC[1];
  ModbusTCP.Register[51]=A24AA64.MAC[2]<<8|A24AA64.MAC[3];
  ModbusTCP.Register[52]=A24AA64.MAC[4]<<8|A24AA64.MAC[5];
  ModbusTCP.Register[60]=A24AA64.DNS[0]<<8|A24AA64.DNS[1];
  ModbusTCP.Register[61]=A24AA64.DNS[2]<<8|A24AA64.DNS[3];
  ModbusTCP.Register[70]=A24AA64.DHCP;


   ctlnetwork(CN_SET_NETINFO,&netInfo);
   ctlnetwork(CN_GET_NETINFO,&netInfo);

   if(netconfig == NETINFO_DHCP)
  	  DHCP_init(SOCK_7, gDATABUFDHCP);

 	uint8_t remoteIP[4];
 	uint16_t remotePort;
 	uint8_t write[5];
 	write[0]=50;
	uint8_t Read;
 	//CLEAR_BIT(hi2c1.Instance->CR1,I2C_CR1_SWRST);
	//A24AA64_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

 	while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	  /* DHCP IP allocation and check the DHCP lease time (for IP renewal) */

	  Modbus_Main();
	 	  if(netconfig == NETINFO_DHCP) {
	 		  dhcp_ret = DHCP_run();

	 		  if((dhcp_ret == DHCP_RUNNING) || (dhcp_ret == DHCP_IP_CHANGED)) {
	 			  getIPfromDHCP(netInfo.ip);
	 			  getGWfromDHCP(netInfo.gw);
	 			  getSNfromDHCP(netInfo.sn);
	 			  getDNSfromDHCP(netInfo.dns);
	 			  netInfo.dhcp = NETINFO_DHCP;
	 			  ctlnetwork(CN_SET_NETINFO, (void*)&netInfo);
	 		  }

	 		  if(dhcp_ret == DHCP_FAILED)	{
	 			  // netconfig = NETINFO_STATIC;
	 			  // set_netinfo_default();
	 		  }
	   }
	 	    retVal = getSn_SR(0);

	 	    switch (retVal) {

	 		case SOCK_CLOSED:
	 			socket(SOCK_0, Sn_MR_TCP, 5000, 0); //Open socket 0 as TCP_SOCKET with port 5000
	 			break;
	 		case SOCK_INIT:
	 			listen(0);
	 			break;
	 		case SOCK_LISTEN:
	 			break;
	 		case SOCK_ESTABLISHED:
	 			getsockopt(0, SO_DESTIP, remoteIP);	// Retrieving remote peer IP and port number
	 			getsockopt(0, SO_DESTPORT, (uint8_t*) &remotePort);

	 			retVal = recv(0, &ModbusTCP.BufforTemp, 1);
	 			if (retVal == 1) {
	 				ModbusTCP.Buffor[ModbusTCP.index] = ModbusTCP.BufforTemp;
	 				ModbusTCP.index++;

	 				if (ModbusTCP.Buffor[5] == (ModbusTCP.index - 6)&& ModbusTCP.Buffor[5] != 0)		//Jezeli cala ramka
	 				{
	 					ModbusTCP.index = 0;
	 					ModbusTCP.Transaction_ID = ModbusTCP.Buffor[0] << 8	| ModbusTCP.Buffor[1];

	 					if (ModbusTCP.Buffor[6] == ModbusTCP_IP_ID) {
	 						if (ModbusTCP.Buffor[7] == ModbusTCP_IP_F_Write) {
	 							ModbusTCP.Data_Offset = ModbusTCP.Buffor[8] << 8| ModbusTCP.Buffor[9];
	 							ModbusTCP.Data_Len = ModbusTCP.Buffor[10] << 8  | ModbusTCP.Buffor[11];
	 							ModbusTCP.Data_Nr_Of_Bytes = ModbusTCP.Buffor[12];

	 							for (uint8_t i = 0; i < ModbusTCP.Data_Len; i++) {
	 								ModbusTCP.Register[ModbusTCP.Data_Offset + i] =	ModbusTCP.Buffor[13 + 2 * i] << 8| ModbusTCP.Buffor[14 + 2 * i];
	 							}
	 							ModbusTCP.Buffor[0] = ModbusTCP.Transaction_ID >> 8;
	 							ModbusTCP.Buffor[1] = ModbusTCP.Transaction_ID	& 0xFF;
	 							ModbusTCP.Buffor[2] = 0x00;		//Protocol number
	 							ModbusTCP.Buffor[3] = 0x00;		//Protocol number
	 							ModbusTCP.Buffor[4] = 0x00;		//REsponse Len_Hi
	 							ModbusTCP.Buffor[5] = 0x06;		//REsponse Len_Lo
	 							ModbusTCP.Buffor[6] = ModbusTCP_IP_ID;
	 							ModbusTCP.Buffor[7] = ModbusTCP_IP_F_Write;
	 							ModbusTCP.Buffor[8] = ModbusTCP.Data_Offset >> 8;
	 							ModbusTCP.Buffor[9] = ModbusTCP.Data_Offset & 0xFF;
	 							ModbusTCP.Buffor[10] = ModbusTCP.Data_Len >> 8;
	 							ModbusTCP.Buffor[11] = ModbusTCP.Data_Len & 0xFF;

	 							send(0, ModbusTCP.Buffor, 12);
	 						}
	 						if (ModbusTCP.Buffor[7] == ModbusTCP_IP_F_Read) {
	 							uint8_t i = 0;
	 							ModbusTCP.Data_Offset = ModbusTCP.Buffor[8] << 8 | ModbusTCP.Buffor[9];
	 							ModbusTCP.Data_Len = ModbusTCP.Buffor[10] << 8  | ModbusTCP.Buffor[11];

	 							ModbusTCP.Buffor[0] = ModbusTCP.Transaction_ID >> 8;
	 							ModbusTCP.Buffor[1] = ModbusTCP.Transaction_ID	& 0xFF;
	 							ModbusTCP.Buffor[2] = 0x00;		//Protocol number
	 							ModbusTCP.Buffor[3] = 0x00;		//Protocol number
	 							ModbusTCP.Buffor[4] = 0x00;		//REsponse Len_Hi
	 							ModbusTCP.Buffor[5] = 3 + 2 * ModbusTCP.Data_Len;//REsponse Len_Lo
	 							ModbusTCP.Buffor[6] = ModbusTCP_IP_ID;
	 							ModbusTCP.Buffor[7] = 0x03;
	 							ModbusTCP.Buffor[8] = 2 * ModbusTCP.Data_Len;

	 							for (i = 0; i < ModbusTCP.Data_Len; i++) {
	 								ModbusTCP.Buffor[9 + 2 * i] = ModbusTCP.Register[ModbusTCP.Data_Offset+ i] >> 8;
	 								ModbusTCP.Buffor[10 + 2 * i] =ModbusTCP.Register[ModbusTCP.Data_Offset+ i] & 0xFF;
	 							}
	 							send(0, ModbusTCP.Buffor, 9 + 2 * i);
	 						}
	 					}
	 				}
	 			}
	 			break;
	 		case SOCK_CLOSE_WAIT:
	 			disconnect(0);	//We close the socket and start a connection again
	 			close(0);
	 			break;
	 		default:
	 			break;
	 		}

	 		/*if ((retVal = socket(SOCK_0, Sn_MR_TCP, 5000, 0)) == 0) {//Open socket 0 as TCP_SOCKET with port 5000

	 			if ((retVal = listen(0)) == SOCK_OK) {//Put socket in LISTEN mode. This means we are creating a TCP server
	 				while ((sockStatus = getSn_SR(0)) == SOCK_LISTEN)//While socket is in LISTEN mode we wait for a remote connection
	 					HAL_Delay(100);


	 				while (1) {//OK. Got a remote peer. Let's send a message to it
	 					if ((sockStatus = getSn_SR(0)) == SOCK_ESTABLISHED) {// If connection is ESTABLISHED with remote peer
	 						uint8_t remoteIP[4];
	 						uint16_t remotePort;
	 						getsockopt(0, SO_DESTIP, remoteIP);// Retrieving remote peer IP and port number
	 						getsockopt(0, SO_DESTPORT, (uint8_t*) &remotePort);

	 						retVal=recv(0,&ModbusTCP.BufforTemp,1);

	 					}
	 				}
	 			}
	 		}
	 		disconnect(0);//We close the socket and start a connection again
	 		close(0);*/

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Enables the Clock Security System 
    */
  HAL_RCC_EnableCSS();

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
