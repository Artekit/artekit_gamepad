/***************************************************************************
 * Artekit STM32 Gamepad Demo
 * Copyright (C) 2013 Artekit Italy
 * http://www.artekit.eu
 
### i2c.c
 
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

void i2cInit(u32 speed)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	/* Enable peripheral clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	/* Configure pins 6 and 7 of port B */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	/* I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = speed;
	I2C_Init(I2C1, &I2C_InitStructure);
	
	/* Enable I2C1 */
	I2C_Cmd(I2C1, ENABLE);
}

void i2cRead(u8* buf, u8 slave, u16 qty)
{  
	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Test EV5 */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* In the case of a single data transfer disable ACK before reading the data */
	if(qty == 1) 
	{
		I2C_AcknowledgeConfig(I2C1, DISABLE);
	}

	/* Send address to read */
	I2C_Send7bitAddress(I2C1, (slave<<1)|0x01, I2C_Direction_Receiver);
  
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
	/* While there is data to be read */
	while(qty)  
	{
		/* Test on EV7 and clear it */
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
		{
			if(qty == 2)
			{
				/* Disable Acknowledgement */
				I2C_AcknowledgeConfig(I2C1, DISABLE);
			}

			if(qty == 1)
			{
				/* Send STOP Condition */
				I2C_GenerateSTOP(I2C1, ENABLE);
			}
      
			/* Read a byte */
			*buf = I2C_ReceiveData(I2C1);

			/* Point to the next location where the byte read will be saved */
			buf++; 
      
			/* Decrement the read bytes counter */
			qty--;    
		}   
	}

	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}

void i2cWrite(u8* buf, u8 slave, u16 qty)
{
	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
  
	/* Test EV5 */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 
  
	/* Send slave address */
	I2C_Send7bitAddress(I2C1, (slave<<1), I2C_Direction_Transmitter);

	/* Test EV6*/
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

	/* While there is data to be written */
	while(qty--)  
	{
		/* Send the current byte */
		I2C_SendData(I2C1, *buf); 

		/* Point to the next byte to be written */
		buf++; 
  
		/* Test EV8 */
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}

	/* Send STOP */
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void i2cDeinit( void)
{
	I2C_DeInit(I2C1);
}

