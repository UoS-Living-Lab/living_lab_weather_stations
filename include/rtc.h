/* 
	Title: RTC.h
	Description: 
	Date: 17/11/2020
	Author: Ethan Bellmer
*/

#include <Arduino.h> // Workaround for byte not declared error

// Function Definition
void RTC_INIT(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);
void RTC_SET_TIMER(int interval);
void RTC_SLEEP(int interval);
void RTC_WAKE(int interval);
void RTC_INT_CLEAR(int interval);
//void RTC_SET_TIME(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);
//void RTC_READ_TIME(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year);
uint8_t readRegister(uint8_t offset);