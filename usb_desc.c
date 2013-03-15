/***************************************************************************
 * Artekit STM32 Gamepad Demo
 * Copyright (C) 2013 Artekit Italy
 * http://www.artekit.eu
 
### usb_desc.h
 
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
#include "usb_lib.h"
#include "usb_desc.h"

/* Device Descriptor */
const u8 wiiccDeviceDescriptor[18] =
{
	0x12,							/*bLength */
	USB_DEVICE_DESCRIPTOR_TYPE,		/*bDescriptorType*/
	0x00, 0x02,						/*bcdUSB */
	0x00,							/*bDeviceClass*/
	0x00,							/*bDeviceSubClass*/
	0x00,							/*bDeviceProtocol*/
	0x40,							/*bMaxPacketSize 64*/
	0x80, 0x04,						/*idVendor (0x0483)*/
	0x10, 0x51,						/*idProduct = 0x5710*/
	0x00, 0x02,						/*bcdDevice rel. 2.00*/	
	1,								/*Index of string descriptor describing manufacturer */
	2,								/*Index of string descriptor describing product*/
    3,								/*Index of string descriptor describing the device serial number */
    0x01							/*bNumConfigurations*/
};

/* Configuration Descriptor */
const u8 wiiccConfigDescriptor[34] =
{
	0x09,								/* bLength */
	USB_CONFIGURATION_DESCRIPTOR_TYPE,	/* bDescriptorType = Configuration */
	34,									/* wTotalLength: Bytes returned */    
	0x00, 0x01,							/*bNumInterfaces: 1 interface*/
	0x01,								/*bConfigurationValue: Configuration value*/
	0x00,								/*iConfiguration: Index of string descriptor describing the configuration*/
	0xE0,								/*bmAttributes: bus powered */
	0x32,								/*MaxPower 100 mA: this current is used for detecting Vbus*/

	/*Interface*/
	0x09,								/*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,		/*bDescriptorType: Interface descriptor type*/
	0x00,								/*bInterfaceNumber: Number of Interface*/
	0x00,								/*bAlternateSetting: Alternate setting*/
	0x01,								/*bNumEndpoints*/
	0x03,								/*bInterfaceClass: HID*/
	0x00,//0x01,						/*bInterfaceSubClass : 1=BOOT, 0=no boot*/
	0x00,								/*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
	0,									/*iInterface: Index of string descriptor*/
	/*HID*/
	0x09,								/*bLength: HID Descriptor size*/
	HID_DESCRIPTOR_TYPE,				/*bDescriptorType: HID*/
	0x00,								/*bcdHID: HID Class Spec release number*/
	0x01,
	0x00,								/*bCountryCode: Hardware target country*/
	0x01,								/*bNumDescriptors: Number of HID class descriptors to follow*/
	0x22,								/*bDescriptorType*/
	74,									/*wItemLength: Total length of wiiccReportDescriptor*/
	0x00,
    /*Endpoint*/
	0x07,          						/*bLength: Endpoint Descriptor size*/
	USB_ENDPOINT_DESCRIPTOR_TYPE,		/*bDescriptorType:*/
	0x81,								/*bEndpointAddress: Endpoint Address (IN)*/
	0x03,								/*bmAttributes: Interrupt endpoint*/
	0x40, 0x00,							/*wMaxPacketSize: 4 Byte max */
	0x01,								/*bInterval: Polling Interval (32 ms)*/
};

const u8 wiiccReportDescriptor[74] =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x02,                    //   USAGE_PAGE (Simulation Controls)
    0x09, 0xbb,                    //   USAGE (Throttle)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x1f,                    //   LOGICAL_MAXIMUM (31)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x05, 0x02,                    //   USAGE_PAGE (Simulation Controls)
    0x09, 0xbb,                    //   USAGE (Throttle)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x1f,                    //   LOGICAL_MAXIMUM (31)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x09, 0x30,                    //   USAGE (X)
    0x09, 0x31,                    //   USAGE (Y)
    0x09, 0x32,                    //   USAGE (Z)
    0x09, 0x33,                    //   USAGE (Rx)
    0x15, 0x81,                    //   LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //   LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //   USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x10,                    //   REPORT_COUNT (16)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0,                          // END_COLLECTION
    0xc0                           // END_COLLECTION
};

/* USB String Descriptors (optional) */
const uint8_t wiiccStringLangID[4] =
{
	4,
	USB_STRING_DESCRIPTOR_TYPE,
	0x09,
	0x04
};

const uint8_t wiiccStringVendor[16] =
{
	16,
	USB_STRING_DESCRIPTOR_TYPE,
	'A', 0, 'r', 0, 't', 0, 'e', 0, 'k', 0, 'i', 0, 't', 0
};

const uint8_t wiiccStringProduct[42] =
{
	42,						 		/* bLength (total) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'A', 0, 'r', 0, 't', 0, 'e', 0, 'k', 0, 'i', 0, 't', 0, ' ', 0,
	'G', 0, 'a', 0, 'm', 0, 'e', 0, 'P', 0, 'a', 0, 'd', 0, ' ', 0,
	'D', 0, 'e', 0, 'm', 0, 'o', 0	 
};

uint8_t wiiccStringSerial[26] =
{
	26,									/* bLength (total) */
	USB_STRING_DESCRIPTOR_TYPE,			/* bDescriptorType */
	/* Dummy serial number */
	'A', 0, 'K', 0, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0
};
