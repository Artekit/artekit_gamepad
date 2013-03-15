/**
  ******************************************************************************
  * @file    usb_istr.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   ISTR events interrupt service routines
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

#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_pwr.h"
//#include "usb_istr.h"

/* ST USB library want these to be defined here */
__IO uint16_t wIstr;
void (*pEpInt_IN[7])(void) =
{
	usbEndpoint1Callback,
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
};
void (*pEpInt_OUT[7])(void) =
{
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
};

/*******************************************************************************
* Function Name  : USB_Istr
* Description    : ISTR events interrupt service routine
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Istr(void)
{
	wIstr = _GetISTR();

	if (wIstr & ISTR_CTR & wInterrupt_Mask)
	{
		/* servicing of the endpoint correct transfer interrupt */
		/* clear of the CTR flag into the sub */
		CTR_LP();
	}
	
	if (wIstr & ISTR_RESET & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_RESET);
		Device_Property.Reset();
	}
	
	if (wIstr & ISTR_DOVR & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_DOVR);
	}

	if (wIstr & ISTR_ERR & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_ERR);
	}

	if (wIstr & ISTR_WKUP & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_WKUP);
		Resume(RESUME_EXTERNAL);
	}

	if (wIstr & ISTR_SUSP & wInterrupt_Mask)
	{
		/* check if SUSPEND is possible */
		if (fSuspendEnabled)
		{
			Suspend();
		} else {
			/* if not possible then resume after xx ms */
			Resume(RESUME_LATER);
		}
		
		/* clear of the ISTR bit must be done after setting of CNTR_FSUSP */
		_SetISTR((uint16_t)CLR_SUSP);
	}
	
	if (wIstr & ISTR_SOF & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_SOF);
	}

	if (wIstr & ISTR_ESOF & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_ESOF);
		/* resume handling timing is made with ESOFs */
		Resume(RESUME_ESOF); /* request without change of the machine state */
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

