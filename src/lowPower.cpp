/* 
	Title: LowPower.cpp
	Description: 
	Date: 13/11/2020
	Author: Ethan Bellmer 
*/

#include "lowPower.h"
#include "init.h"


#define INT_PIN PIN_PD2

void MCU_SET_INT()
{
    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), MCU_WAKE_INT, RISING); // Set pin interrupt for waking from sleep
}

void MCU_SLEEP()
{
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	usbUSART.println("going to sleep ...");
	delay(50);
	sleep_cpu();
}

void MCU_WAKE_INT()
{
	Serial.println("Interrupt");
	sleep_disable();
	delay(2000);
	usbUSART.println("Waking up...");
}
