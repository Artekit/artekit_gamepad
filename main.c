/***************************************************************************
 * Artekit STM32 Gamepad Demo
 * Copyright (C) 2013 Artekit Italy
 * http://www.artekit.eu
 
### main.c
 
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

***************************************************************************/
#include "stm32f10x.h"
#include "sys.h"
#include "wii_cc.h"
#include <usb_lib.h>

WII_CC_DATA wii_data;

void RCC_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	/* USB */
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

void wiiCCtoUSB(WII_CC_DATA* data)
{	
	u8 packet[8];

	packet[0] = (u8) data->left_trigger;
	packet[1] = (u8) data->right_trigger;
	packet[2] = (u8) data->left_analog_x;
	packet[3] = (u8) data->left_analog_y;
	packet[4] = (u8) data->right_analog_x;
	packet[5] = (u8) data->right_analog_y;
	
	packet[6] = *((u8*) &data->buttons);
	packet[7] = *(((u8*) &data->buttons)+1);
	
	usbResetTransfer();

	USB_SIL_Write(EP1_IN, packet, 8);
	SetEPTxValid(ENDP1);
	
	while (usbCanTransfer() == 0);
}

int main(void)
{
	RCC_Configuration();

	sysInitSystemTimer();
	
	usbInit();
		
	sysDelayMs(25);
		
	wiiCCInit();
	
	for (;;)
	{
		wiiCCRead(&wii_data);
		wiiCCtoUSB(&wii_data);
	}

	return 0;
}
