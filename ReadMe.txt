DESCRIPTION:
 This example demonstrate the PWMSP001 App. 
 The Period Match Interrupt of PWMSP001 will be used to determine the duty cycle of the LED.
 The on-board LED (Port 0.5) will be dim due to duty cycle of 10%.
 The on-board LED (Port 0.5) will be bright due to duty cycle of 90%.
 The NVIC002 App needs to be connected accordingly for the PWMSP001 period match interrupt.
  
REQUIREMENTS:
- J-Link Drivers installed on PC
- J-Link v4.59 or above
- XMC1100 CPU Card (Boot Kit)

SETUP:Connect the USB cable with XMC1100 CPU card.

HOW TO CREATE THE PROJECT:
1) Instantiate PWMSP001 App and NVIC002 App from App Selection View
2) In PWMSP001, Select Edge-Aligned Mode, enter the PWM frequency as 1500 Hz, Select Period Match Initialization in 
	 PWM Configuration Tab.
3) In NVIC002_0 App, Select "Enable Interrupt At Initialization".
    Enter the "pwm_period_interrupt" in the User defined Interrupt Handler.
4) Make the following Signal Connection:
   - Connect the "Period Match Interrupt" signal of PWMSP001/0 to the "Interrupt Node" 
     signal of NVIC002/0 App.
5) Manual Pin assignment: This is to define the board specific I/O constraints
	 In this case, PWMSP001/0 pin_directoutput resource is assigned to port P0.5 as a LED indicator.
6) Call PWMSP001_Start() after Dave_Init().
7) Define the "PWM_Period_Interrupt" interrupt handler function in the code.
7) Generate the code and build the project.
8) Download and run the project. 


OBSERVATIONS:
1. LED at P0.5 brightness increases and decreases.

