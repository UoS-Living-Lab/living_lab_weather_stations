/* 
	Title: RTC.cpp
	Description: 
	Date: 17/11/2020
	Author: Ethan Bellmer
*/

// Library Includes
#include "rtc.h"
#include "init.h"

// Variable Declarations
#define RTC_ADDRESS 0x69


// Function Definition
void RTC_INIT(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);
void RTC_SET_TIMER(int interval);
void RTC_SLEEP(int interval);
void RTC_WAKE(int interval);
void RTC_INT_CLEAR(int interval);
void RTC_SET_TIME(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);
void RTC_READ_TIME(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year);
byte bcdToDec(byte val);
byte decToBcd(byte val);
uint8_t readRegister(uint8_t offset);
void writeRegister(uint8_t offset, uint8_t dataToWrite);


void RTC_INIT(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
	uint8_t CONFIG_REG_01 = 0x00;
	uint8_t CONFIG_REG_02 = 0x01;
	uint8_t configData01;
	uint8_t configData02;
	

	// Set the configuration for config register 1.
	configData01 = readRegister(CONFIG_REG_01); // Config_reg1 (0x00)
	/* 
		Exit software reset - SWRSTN = 1 in BIT 0
		Enable oscillator - OSCONZ = 0 in BIT 3
	*/
	configData01 &= ~((0 << 3) | (1 << 0));
	writeRegister(CONFIG_REG_01, configData01); // Write the modified config to the config register.

	RTC_SET_TIME(second, minute, hour, dayOfWeek, dayOfMonth, month, year); // Write RTC time to registers 0x06-0x0C.


	// Set the configuration for config register 2.
	configData02 = readRegister(CONFIG_REG_02); // Config_reg2 (0x01)
	/*
		Write SET_RTC = 1.
	*/
	configData02 &= ~((1 << 1));
	writeRegister(CONFIG_REG_02, configData02);

	delay(10); // Delay for 10ms to allow RTC to transition RTC enable.

	configData02 = readRegister(CONFIG_REG_02); // Config_reg2 (0x01)
	/*
		Write SET_RTC = 0
	*/
	configData02 &= ~((0 << 1));
	writeRegister(CONFIG_REG_02, configData02); // Write the modified config to the config register.
}

void RTC_SET_TIMER(int interval)
{
	uint8_t TIMER_REG = 0x03;
	uint8_t TIMER_INIT = 0x17;
	uint8_t timerConfigData;

	timerConfigData = readRegister(TIMER_REG); // 03h (Timer_config)
	timerConfigData &= ~((0 << 4) | (1 << 2) | (1 << 1)); // Set timer selection to 16hz (1 in BIT 1), and enable timer repeat (1 in BIT 2), and reset timer to allow setting countdown interval
	writeRegister(TIMER_REG, timerConfigData); // Write the modified config to the timer config register.


	writeRegister(TIMER_INIT, decToBcd(interval)); // 900000ms 15m


	timerConfigData = readRegister(TIMER_REG); // 03h (Timer_config)
	timerConfigData &= ~((1 << 4)); // Enable timer countdown (1 in BIT 4)
	writeRegister(TIMER_REG, timerConfigData); // Write the modified config to the timer config register.
}

void RTC_SLEEP()
{
	uint8_t CONFIG_REG_01 = 0x00;
	uint8_t CONFIG_REG_02 = 0x01;
	uint8_t configData01;
	uint8_t configData02;


	// Set the configuration for config register 2.
	configData02 = readRegister(CONFIG_REG_02); // Config_reg2 (0x01)
	/*
		Enable Data Retention mode - DATA_RETEN = 1 in BIT 6.
	*/
	configData02 &= ~((1 << 6));
	writeRegister(CONFIG_REG_02, configData02);
	
	// Set the configuration for config register 1.
	configData01 = readRegister(CONFIG_REG_01); // Config_reg1 (0x00)
	/* 
		Disable oscillator - OSCONZ = 1 in BIT 3.
	*/
	configData01 &= ~((1 << 3));
	writeRegister(CONFIG_REG_01, configData01); // Write the modified config to the config register.
}

void RTC_WAKE()
{
	uint8_t CONFIG_REG_01 = 0x00;
	uint8_t CONFIG_REG_02 = 0x01;
	uint8_t configData01;
	uint8_t configData02;


	// Set the configuration for config register 2.
	configData02 = readRegister(CONFIG_REG_02); // Config_reg2 (0x01)
	/*
		Enable Data Retention mode - DATA_RETEN = 0 in BIT 6.
	*/
	configData02 &= ~((0 << 6));
	writeRegister(CONFIG_REG_02, configData02);
	
	// Set the configuration for config register 1.
	configData01 = readRegister(CONFIG_REG_01); // Config_reg1 (0x00)
	/* 
		Disable oscillator - OSCONZ = 0 in BIT 3.
	*/
	configData01 &= ~((0 << 3));
	writeRegister(CONFIG_REG_01, configData01); // Write the modified config to the config register.
}

void RTC_INT_CLEAR()
{
	uint8_t INT_REG = 0x04;

	uint8_t intData = readRegister(INT_REG); // Interrupt register (0x04)
	intData &= ~((1 << 2) | (1 << 0));
	writeRegister(INT_REG, intData); // Write the modified config to the config register.
}

void RTC_SET_TIME(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
	// sets time and date data to MAX31342
	Wire.beginTransmission(RTC_ADDRESS);
	
	Wire.write(0x06); // set next input to start at the seconds register
	Wire.write(decToBcd(second)); // set seconds
	
	Wire.write(0x07); // set next input to start at the minutes register
	Wire.write(decToBcd(minute)); // set minutes
	
	Wire.write(0x08); // set next input to start at the hours register
	Wire.write(decToBcd(hour)); // set hours

	Wire.write(0x09); // set next input to start at the day register
	Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Monday, 7=Sunday)
	
	Wire.write(0x10); // set next input to start at the date register
	Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
	
	Wire.write(0x11); // set next input to start at the month register
	Wire.write(decToBcd(month)); // set month
	
	Wire.write(0x12); // set next input to start at the year register
	Wire.write(decToBcd(year)); // set year (0 to 99)
	
	Wire.endTransmission();
}

void RTC_READ_TIME(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year)
{
	Wire.beginTransmission(RTC_ADDRESS);
	Wire.write(0x06); // set DS3231 register pointer to 00h
	Wire.endTransmission();
	Wire.requestFrom(RTC_ADDRESS, 7);
	// request seven bytes of data from DS3231 starting from register 00h
	*second = bcdToDec(Wire.read() & 0x7f);
	*minute = bcdToDec(Wire.read());
	*hour = bcdToDec(Wire.read() & 0x3f);
	*dayOfWeek = bcdToDec(Wire.read());
	*dayOfMonth = bcdToDec(Wire.read());
	*month = bcdToDec(Wire.read());
	*year = bcdToDec(Wire.read());
}

byte bcdToDec(byte val)
{
	return( (val/16*10) + (val%16) );
}

byte decToBcd(byte val)
{
	return( (val/10*16) + (val%10) );
}

uint8_t readRegister(uint8_t offset)
{
	//Return value
	uint8_t result = 0;
	uint8_t numBytes = 1;

	Wire.beginTransmission(RTC_ADDRESS);
	Wire.write(offset);
	Wire.endTransmission();

	Wire.requestFrom(RTC_ADDRESS, numBytes);
	while (Wire.available()) // slave may send less than requested
	{
		result = Wire.read(); // receive a byte as a proper uint8_t
	}

	return result;
}

void writeRegister(uint8_t offset, uint8_t dataToWrite)
{
	Wire.beginTransmission(RTC_ADDRESS);
	Wire.write(offset);
	Wire.write(dataToWrite);
	Wire.endTransmission();
}