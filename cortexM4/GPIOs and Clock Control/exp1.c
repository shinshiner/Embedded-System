#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "debug.h"
#include "gpio.h"
#include "hw_types.h"
#include "pin_map.h"
#include "sysctl.h"


#define   FASTFLASHTIME			(uint32_t)500000
#define   SLOWFLASHTIME			(uint32_t)4000000

uint32_t delay_time,key_value;


void 		Delay(uint32_t value);
void 		S800_GPIO_Init(void);

void L0Flash(){
	delay_time = FASTFLASHTIME;
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);			// Turn on the LED.
		Delay(delay_time);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);							// Turn off the LED.
		Delay(delay_time);
}

void L1Flash(){
	delay_time = FASTFLASHTIME;
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);			// Turn on the LED.
		Delay(delay_time);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);							// Turn off the LED.
		Delay(delay_time);
}

void R3(){
	while(1){		
		if (GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	== 0){
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
		}
		else if(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	== 1){
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);
		}
		
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
		if (GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_1)	== 0){
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
		}
		else if(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_1)	== 1){
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);
		}
		Delay(25);
		GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1);
	}
}

void R4(){
	int times = 0;
	while(1){
		if(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	== 0){
			while(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	== 0);
			times = (times + 1) % 4;
		}
		switch (times){
			case 1:
			{
				GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
				L0Flash();
				break;}
			case 2:
			{
				GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
				//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);
				break;}
			case 3:
			{
				GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
				L1Flash();
				break;}
			case 0:
			{
				GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1);
				//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
				break;}
		}
		Delay(20);
	}
}

int main(void)
{
	S800_GPIO_Init();
	//SysCtlClockFreqSet(SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_25MHZ, 12500000);
	//SysCtlClockFreqSet(SYSCTL_USE_OSC | SYSCTL_OSC_INT, 12500000);
	SysCtlClockFreqSet(SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_CFG_VCO_320, 100000000);
	/*while(1)
  {
		key_value = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	;				//read the PJ0 key value

		if (GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	== 0)						//USR_SW1-PJ0 pressed
			delay_time							= FASTFLASHTIME;
		else
			delay_time							= SLOWFLASHTIME;
		
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);			// Turn on the LED.
		Delay(delay_time);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);							// Turn off the LED.
		Delay(delay_time);
   }*/
	R4();
}



void Delay(uint32_t value)
{
	uint32_t ui32Loop;
	for(ui32Loop = 0; ui32Loop < value; ui32Loop++){};
}


void S800_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);						//Enable PortF
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);						//Enable PortJ	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));			//Wait for the GPIO moduleJ ready	
	
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);			//Set PF0 as Output pin
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1);//Set the PJ0,PJ1 as input pin
	GPIOPadConfigSet(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}


