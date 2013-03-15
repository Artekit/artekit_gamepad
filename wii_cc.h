/***************************************************************************
 * Artekit STM32 Gamepad Demo
 * Copyright (C) 2013 Artekit Italy
 * http://www.artekit.eu
 
### wii_cc.h
 
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

#include <stm32f10x.h>
#include "i2c.h"

#define WII_CONTROLLER_ID	0x52

typedef struct wiiCCButtonsTag
{
	u8 dummy:1;
	u8 rtrigger_push:1;
	u8 plus:1;
	u8 home:1;
	u8 minus:1;
	u8 ltrigger_push:1;
	u8 down:1;
	u8 right:1;
	
	u8 up:1;
	u8 left:1;
	u8 zr:1;
	u8 x:1;
	u8 a:1;
	u8 y:1;
	u8 b:1;
	u8 zl:1;	
	
} WII_CC_BUTTONS;

/* WII Classic Controller Data */
typedef struct wiiCCDataTag
{
	s8 left_analog_x;
	s8 left_analog_y;
	
	s8 right_analog_x;
	s8 right_analog_y;
	
	s8 left_trigger;
	s8 right_trigger;
	
	WII_CC_BUTTONS buttons;	
	
} WII_CC_DATA;

void wiiCCInit(void);
void wiiCCRead(WII_CC_DATA* data);
