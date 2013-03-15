/**
  ******************************************************************************
  * @file    usb_prop.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   All processing related to Joystick Mouse Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ProtocolValue;

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    2,		/* Endpoints */
    1
  };

DEVICE_PROP Device_Property =
  {
    Gamepad_init,
    Gamepad_Reset,
    Gamepad_Status_In,
    Gamepad_Status_Out,
    Gamepad_Data_Setup,
    Gamepad_NoData_Setup,
    Gamepad_Get_Interface_Setting,
    Gamepad_GetDeviceDescriptor,
    Gamepad_GetConfigDescriptor,
    Gamepad_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };
USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    NOP_Process,
    Gamepad_SetConfiguration,
    NOP_Process,
    NOP_Process,
    NOP_Process,
    NOP_Process,
    NOP_Process,
    NOP_Process,
    Gamepad_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)wiiccDeviceDescriptor,
    18
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)wiiccConfigDescriptor,
		34
  };

ONE_DESCRIPTOR Gamepad_Report_Descriptor =
  {
    (uint8_t *)wiiccReportDescriptor,
    74
  };

ONE_DESCRIPTOR Gamepad_Hid_Descriptor =
  {
    (uint8_t*)wiiccConfigDescriptor + 18,
    9
  };

ONE_DESCRIPTOR String_Descriptor[4] =
  {
    {(uint8_t*)wiiccStringLangID, 4},
    {(uint8_t*)wiiccStringVendor, 16},
    {(uint8_t*)wiiccStringProduct, 42},
    {(uint8_t*)wiiccStringSerial, 26}
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Gamepad_init(void)
{

  /* Update the serial number string descriptor with the data from the unique
  ID*/
  // Get_SerialNum();

  pInformation->Current_Configuration = 0;
  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  bDeviceState = UNCONNECTED;
}

void Gamepad_Reset(void)
{
  /* Set Joystick_DEVICE as not configured */
  pInformation->Current_Configuration = 0;
  pInformation->Current_Interface = 0;/*the default Interface*/

  /* Current Feature initialization */
  pInformation->Current_Feature = wiiccConfigDescriptor[7];

#ifdef STM32F10X_CL   
  /* EP0 is already configured in DFU_Init() by USB_SIL_Init() function */
  
  /* Init EP1 IN as Interrupt endpoint */
  OTG_DEV_EP_Init(EP1_IN, OTG_DEV_EP_TYPE_INT, 4);
#else 

  SetBTABLE(0);

  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxCount(ENDP1, 4);
  SetEPRxStatus(ENDP1, EP_RX_DIS);
  SetEPTxStatus(ENDP1, EP_TX_NAK);

  /* Set this device to response on default address */
  SetDeviceAddress(0);
#endif /* STM32F10X_CL */

  bDeviceState = ATTACHED;
}

void Gamepad_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

void Gamepad_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}

void Gamepad_Status_In(void)
{}

void Gamepad_Status_Out (void)
{}

RESULT Gamepad_Data_Setup(uint8_t RequestNo)
{
  uint8_t *(*CopyRoutine)(uint16_t);

  CopyRoutine = NULL;
  if ((RequestNo == GET_DESCRIPTOR)
      && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
      && (pInformation->USBwIndex0 == 0))
  {

    if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
    {
      CopyRoutine = Gamepad_GetReportDescriptor;
    }
    else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
    {
      CopyRoutine = Gamepad_GetHIDDescriptor;
    }

  } /* End of GET_DESCRIPTOR */

  /*** GET_PROTOCOL ***/
  else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
           && RequestNo == GET_PROTOCOL)
  {
    CopyRoutine = Gamepad_GetProtocolValue;
  }


  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

RESULT Gamepad_NoData_Setup(uint8_t RequestNo)
{
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == SET_PROTOCOL))
  {
    return Gamepad_SetProtocol();
  }

  else
  {
    return USB_UNSUPPORT;
  }
}

uint8_t *Gamepad_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

uint8_t *Gamepad_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

uint8_t *Gamepad_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

uint8_t *Gamepad_GetReportDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Gamepad_Report_Descriptor);
}

uint8_t *Gamepad_GetHIDDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Gamepad_Hid_Descriptor);
}

RESULT Gamepad_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

RESULT Gamepad_SetProtocol(void)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

uint8_t *Gamepad_GetProtocolValue(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (uint8_t *)(&ProtocolValue);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
