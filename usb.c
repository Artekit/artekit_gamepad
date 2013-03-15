/***************************************************************************
 * Artekit STM32 Gamepad Demo
 * Copyright (C) 2013 Artekit Italy
 * http://www.artekit.eu
 
### usb.c
 
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
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_istr.h"

volatile u8 usbTransferEnabled = 1;

void usbInterruptsConfig(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the USB interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
}

void usbInit(void)
{
	volatile DEVICE_INFO *pInfo = &Device_Info;
	
	usbInterruptsConfig();
	USB_Init();
	
	while (pInfo->Current_Configuration == 0);
}

void usbEndpoint1Callback(void)
{
	usbTransferEnabled = 1;
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	USB_Istr();
}

void usbResetTransfer(void)
{
	usbTransferEnabled = 0;
}

u8 usbCanTransfer(void)
{
	return usbTransferEnabled;
}
