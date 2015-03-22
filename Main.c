/*=========================================================================== *
 * Copyright (c) 2014, Infineon Technologies AG                                *
 * All rights reserved.                                                        *
 *                                                                             *
 * Redistribution and use in source and binary forms, with or without          *
 * modification, are permitted provided that the following conditions are met: *
 * Redistributions of source code must retain the above copyright notice, this *
 * list of conditions and the following disclaimer. Redistributions in binary  *
 * form must reproduce the above copyright notice, this list of conditions and *
 * the following disclaimer in the documentation and/or other materials        *
 * provided with the distribution. Neither the name of the copyright holders   *
 * nor the names of its contributors may be used to endorse or promote         *
 * products derived from this software without specific prior written          *
 * permission.                                                                 *
 *                                                                                                                                                                                                                                                                                                                   *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" *
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,       *
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      *
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR           *
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,       *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,         *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; *
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,    *
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR     *
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF      *
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                  *
 * To improve the quality of the software, users are encouraged to share       *
 * modifications, enhancements or bug fixes with                               *
 * Infineon Technologies AG (dave@infineon.com).                               *
 *                                                                             *
 * ========================================================================== */

/********************************************************************************
**                                                                            **
** PLATFORM : Infineon XMC1000 Series                                         **
**                                                                            **
** AUTHOR : Application Engineering Team                                      **
**                                                                            **
** Project version update to v 1.0.0 (Initial version)                        **
**                           v 1.0.2 (Updated with latest Dave apps,linker and**
**                                    startup files)                          **
**                                                                            **
** MODIFICATION DATE : May 27, 2014                                           **
**                                                                            **
*******************************************************************************/


/*
 *  Main source file for this PWMSP001 demo. This file contains the main
 *  tasks of the demo and is responsible for the initial application hardware
 *  configuration
 */

/*******************************************************************************
 ** INCLUDE FILES                                                             **
 ******************************************************************************/

#include <DAVE3.h>			//Declarations from DAVE3 Code Generation
#include "XMC1100.h"
#include "GPIO.h"

#define PIN_IS_HIGH()      P0_5_read()
#define PIN_LOW_DEASSERT() P0_6_set()
#define PIN_LOW_ASSERT()   P0_6_reset()
#define PIN_LOW_TOGGLE()   P0_6_toggle()

#define PIN_LOW_INIT() \
	P0_6_set_mode(OUTPUT_OD_GP); \
	P0_6_set();

#define PIN_IS_INIT() \
	P0_5_set_large_hysteresis(); \
	P0_5_read(); \
	//P0_5_set_mode(INPUT);



int main(void)
{
	status_t status;		// Declaration of return variable for DAVE3 APIs


	DAVE_Init();			// Initialization of DAVE Apps

	
	/* Starts the PWMSP001 App (LED) */
	status = PWMSP001_Start((PWMSP001_HandleType*)&PWMSP001_Handle0);
	if(status != DAVEApp_SUCCESS)
	{
		PWMSP001_Stop((PWMSP001_HandleType*)&PWMSP001_Handle0);
	}

	PIN_LOW_INIT();
	PIN_IS_INIT();



	bool lastPinState = PIN_IS_HIGH();
	bool trigger = FALSE;
	int noTriggerCnt=0;
	int speed=0;
	int speedLow=0;
	int speedLowHigh=0;


	while(1)
	{
		speed++;
		speedLow++;
		
		int cnt_high=0;
		int cnt_low=0;
		for (int i = 0; i < 0x1FF; i++)
		{
			if (PIN_IS_HIGH())
				cnt_high++;
			else
				cnt_low++;
		}
		
		
		if (cnt_low > 0)
		{
			if (lastPinState)
			{
				trigger = TRUE;
				speedLowHigh = speed*16/10/2;
				speed=0;
			}
			lastPinState = FALSE;
		}
		else if (cnt_high > 0)
		{
			lastPinState = TRUE;
		}
		
		if (speedLow >= speedLowHigh)
		{
			speedLow = 0;
			
			if (!trigger)
				noTriggerCnt++;
			else
				noTriggerCnt=0;
			trigger = FALSE;
			
			if (noTriggerCnt > 1)
			{
				noTriggerCnt=1;
				PIN_LOW_DEASSERT();
			}
			else
			{
				PIN_LOW_TOGGLE();
			}
		}
				
		

	}
	return 0;
}


/*
 * PWM_Period_Interrupt handler: executes every period match of PWMSP001/0.
 */
void PWM_Period_Interrupt(void)
{
#define FFAST   10
#define FSLOW   5
#define PERIODE   (FFAST*4)
#define PERCHANGE (FFAST*2)
	static uint32_t state = 0;
	status_t status;

	/* state machine to change the different duty cycle */
	switch(state)
	{
		
		case 0: //status =  PWMSP001_SetCompare(&PWMSP001_Handle0, 0x4AFF); /* Updating of duty cycle to 10% */
		    // Change PWM frequency to 20 Hz and duty cycle of 50%
		    status = PWMSP001_SetPwmFreqAndDutyCycle((PWMSP001_HandleType*)&PWMSP001_Handle0, FFAST, 50);
		    break;
		
		case PERCHANGE: //status =  PWMSP001_SetCompare(&PWMSP001_Handle0, 0x0855); /* Updating of duty cycle to 90% */
		    // Change PWM frequency to 10 Hz and duty cycle of 50%
		    status = PWMSP001_SetPwmFreqAndDutyCycle((PWMSP001_HandleType*)&PWMSP001_Handle0, FSLOW, 50);
		    break;

		default:
			break;
	}

	/* Updation of the state machine */
	state++;
	if(state>=PERIODE)
	{
		state= 0;
	}

	if(status != DAVEApp_SUCCESS)
	{
		PWMSP001_Stop((PWMSP001_HandleType*)&PWMSP001_Handle0);
	}
}
