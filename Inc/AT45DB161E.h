/*
 * AT45DB161E.h
 *
 *  Created on: 04.01.2018
 *      Author: MikolajP
 */

#ifndef AT45DB161E_H_
#define AT45DB161E_H_

#include "stdint.h"
#include "stdbool.h"

void AT45DB161E_Init();

void AT45DB161E_Chip_Erase();
void AT45DB161E_Enable_Sector_Protection();
void AT45DB161E_Disable_Sector_Protection();

uint16_t AT45DB161E_Status_Register();
void AT45DB161E_Device_ID();



#endif /* AT45DB161E_H_ */
