/* 
	Title: LowPower.cpp
	Description: 
	Date: 13/11/2020
	Author: Ethan Bellmer 
*/

#include "lowPower.h"
#include "init.h"


#define INT_PIN PIN_PD2


void MCU_ATTACH_INTERRUPT()
{
	attachInterrupt(digitalPinToInterrupt(INT_PIN), MCU_WAKE_INT, LOW); // Set pin interrupt for waking from sleep
}

void MCU_DETECH_INTERRUPT()
{
	detachInterrupt(digitalPinToInterrupt(INT_PIN));
}

void MCU_SLEEP()
{
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	MCU_ATTACH_INTERRUPT();
	usbUSART.println("going to sleep ...");
	delay(50);
	sleep_mode();
	//sleep_cpu();

	sleep_disable();
	MCU_DETECH_INTERRUPT();

	delay(2000);
	usbUSART.println("Waking up...");
}

void MCU_WAKE_INT()
{
	/*
	sleep_disable();
	MCU_DETECH_INTERRUPT();
	Serial.println("Interrupt");
	delay(2000);
	usbUSART.println("Waking up...");
	*/
}
