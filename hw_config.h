/***************************************************************************
 * Artekit STM32 Gamepad Demo
 * Copyright (C) 2013 Artekit Italy
 * http://www.artekit.eu
 
### hw_config.h
 
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
#ifndef __HW_CONFIG_H__
#define __HW_CONFIG_H__

/* ST USB library wants a file called hw_config.h, so here it is. */

#include <stm32f10x.h>
#include "usb_type.h"

void usbInit(void);
void usbResetTransfer(void);
u8 usbCanTransfer(void);
void usbEndpoint1Callback(void);

#endif  /* __HW_CONFIG_H__ */
