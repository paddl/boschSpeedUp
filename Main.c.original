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


int main(void)
{
//	status_t status;		// Declaration of return variable for DAVE3 APIs


	DAVE_Init();			// Initialization of DAVE Apps

	/* Starts the PWMSP001 App (LED) */
	PWMSP001_Start(&PWMSP001_Handle0);

	while(1)
	{

	}
	return 0;
}


/*
 * PWM_Period_Interrupt handler: executes every period match of PWMSP001/0.
 */
void PWM_Period_Interrupt(void)
{
	static uint32_t state = 0;
	status_t status;

	/* state machine to change the different duty cycle */
	switch(state)
	{
		/* Updating of duty cycle to 10% */
		case 0: status =  PWMSP001_SetCompare(&PWMSP001_Handle0, 0x4AFF);
		    break;

		/* Updating of duty cycle to 90% */
		case 2000: status =  PWMSP001_SetCompare(&PWMSP001_Handle0, 0x0855);
		    break;

		default:
			break;
	}

	/* Updation of the state machine */
	state++;
	if(state>=4000)
	{
		state= 0;
	}

	if(status != DAVEApp_SUCCESS)
	{
		PWMSP001_Stop((PWMSP001_HandleType*)&PWMSP001_Handle0);
	}
}
