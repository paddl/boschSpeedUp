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
//#define PIN_SW_HIGH()      FALSE
#define PIN_IS_HIGH()      P0_6_read()
#define PIN_LOW_DEASSERT() P0_5_set()
#define PIN_LOW_ASSERT()   P0_5_reset()
#define PIN_LOW_TOGGLE()   P0_5_toggle()
#define PIN_STATUS_DEASSERT() P1_3_reset()
#define PIN_STATUS_ASSERT()   P1_3_set()
#define PIN_STATUS_TOGGLE()   P1_3_toggle()

#define PIN_LOW_INIT() \
	P0_5_set_mode(OUTPUT_OD_GP); \
	P0_5_set();

#define PIN_STATUS_INIT() \
	P1_3_set_mode(OUTPUT_OD_GP); \
	P1_3_set();


#define PIN_IS_INIT() \
    P0_6_set_mode(INPUT_PU); \
	P0_6_set_large_hysteresis(); \
	P0_6_read(); \

#define PIN_SW_INIT() \
    P1_4_set_mode(INPUT_PU); \
	P1_4_set_large_hysteresis(); \
	P1_4_read(); \

#define SLEEP_COUNT 100

#define pinCheckCycles  100

#define DEFAULT_PERIODE 0xFFFF
	unsigned int speed=0,speedMeas=DEFAULT_PERIODE;
	unsigned int speedLowHigh=DEFAULT_PERIODE;
	unsigned int noTriggerCnt=0;
	unsigned int speedLow=0;
	bool trigger = FALSE;


int main(void)
{
	DAVE_Init();			// Initialization of DAVE Apps

	PIN_LOW_INIT();
	PIN_IS_INIT();
	PIN_SW_INIT();
	PIN_STATUS_INIT();

	// enable deep sleep
	PPB->SCR |= PPB_SCR_SLEEPDEEP_Msk;

	while(1)
	{
		speed++;
		speedLow++;
		
		// some delay
		int cnt_low=0;
		for (int i = 0; i < pinCheckCycles; i++)
		{
			;
		    cnt_low++;
		}

		if (speed > speedMeas)
		{
		}


		if (speedLow >= speedLowHigh)
		{
			if (!trigger)
			{
				if (noTriggerCnt <= SLEEP_COUNT)
					noTriggerCnt++;
				else
				{
					PIN_STATUS_DEASSERT();
					PIN_LOW_DEASSERT();
					speed = DEFAULT_PERIODE;
					speedLow = DEFAULT_PERIODE;
					noTriggerCnt = 0;
					speedLowHigh  = DEFAULT_PERIODE;

					while(!trigger)
						__WFE();  // sleep and wait for event

					PIN_STATUS_ASSERT();
					PIN_LOW_TOGGLE();
				}
			}
			else
			{
				noTriggerCnt=0;
			}


			speedLow = 0;
			PIN_LOW_TOGGLE();
			trigger = FALSE;
		}

	}
	return 0;
}



/**
 * @brief ERU Event Handler (Handler for external Interrupt): Turns on P0.5 on ERU event
 * @param[in/out] None
 *
 */
void ERU_Event_Handler(void)
{
	uint32_t status;

	/* Reads Status Flag */
	status = RD_REG(ERU001_Handle0.ERURegs->EXICON[ERU001_Handle0.InputChannel],ERU_EXICON_FL_Msk , ERU_EXICON_FL_Pos);

	if(status)
    {
		// falling edge on pin 2.5
		trigger = TRUE;
		if (PIN_SW_HIGH())
		{
			speedLowHigh =  (speed<<3)/10; // *16/10/2
		}
		else
		{
			speedLowHigh =  speed>>1; // /2
		}

		speedMeas = speed;
		speed=0;

    	ERU001_ClearFlag(ERU001_Handle0);	/* Clears the Status Flag */
    }
}

