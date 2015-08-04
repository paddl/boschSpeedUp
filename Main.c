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


#define PIN_SW_HIGH()      P1_4_read()
#define PIN_IS_HIGH()      P0_6_read()
#define PIN_LOW_DEASSERT() P0_5_set()
#define PIN_LOW_ASSERT()   P0_5_reset()
#define PIN_LOW_TOGGLE()   P0_5_toggle()

#define PIN_LOW_INIT() \
	P0_5_set_mode(OUTPUT_OD_GP); \
	P0_5_set();

#define PIN_IS_INIT() \
    P0_6_set_mode(INPUT_PU); \
	P0_6_set_large_hysteresis(); \
	P0_6_read(); \

#define PIN_SW_INIT() \
    P1_4_set_mode(INPUT_PU); \
	P1_4_set_large_hysteresis(); \
	P1_4_read(); \




int main(void)
{
	//status_t status;		// Declaration of return variable for DAVE3 APIs


	DAVE_Init();			// Initialization of DAVE Apps

	//PWMSP001_Stop((PWMSP001_HandleType*)&PWMSP001_Handle0);

	
	/* Starts the PWMSP001 App (LED)
	status = PWMSP001_Start((PWMSP001_HandleType*)&PWMSP001_Handle0);
	if(status != DAVEApp_SUCCESS)
	{
		PWMSP001_Stop((PWMSP001_HandleType*)&PWMSP001_Handle0);
	}*/

	PIN_LOW_INIT();
	PIN_IS_INIT();
	PIN_SW_INIT();

	bool lastPinState = PIN_IS_HIGH();
	bool trigger = FALSE;
	int noTriggerCnt=0;
	int speed=0;
	int speedLow=0;
	int speedLowHigh=0;

	int pinCheckCycles = 100;

#ifdef DEBUG

#define DEBUG_CNT_BUFFER_SIZE 100
	static int debugCount = 0;
	static int debugSpeedBuffer[DEBUG_CNT_BUFFER_SIZE];
#endif




	while(1)
	{
		speed++;
		speedLow++;
		

		int cnt_high=0;
		int cnt_low=0;
		for (int i = 0; i < pinCheckCycles; i++)
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
				if (PIN_SW_HIGH())
				{
					speedLowHigh =  (speed<<3)/10; // *16/10/2
				}
				else
				{
					speedLowHigh =  speed>>1; // /2
				}

#ifdef DEBUG
				debugSpeedBuffer[debugCount%DEBUG_CNT_BUFFER_SIZE] = speed;
				debugCount++;
				debugSpeedBuffer[debugCount%DEBUG_CNT_BUFFER_SIZE] = speedLowHigh;
				debugCount++;
#endif
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
#define MAXFREQ 7
	static uint32_t state = 0;
	static uint32_t cycles = 0;
	static status_t status = DAVEApp_SUCCESS;
	/* state machine to change the different duty cycle */
	if (cycles == 0)
	{
		cycles = (state%MAXFREQ) + 1;
		//status = PWMSP001_SetPwmFreqAndDutyCycle((PWMSP001_HandleType*)&PWMSP001_Handle0, cycles, 50);
		cycles = cycles<<2;

		state++;
		if(status != DAVEApp_SUCCESS)
		{
			//PWMSP001_Stop((PWMSP001_HandleType*)&PWMSP001_Handle0);
		}
	}
	cycles--;
}
