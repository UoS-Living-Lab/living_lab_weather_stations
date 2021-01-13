/* 
	Title: RTC.cpp
	Description: 
	Date: 17/11/2020
	Author: Ethan Bellmer
*/

// Library Includes
#include "rtc.h"
#include "init.h"

// Function Definition
void RTC_INIT(int second, int minute, int hour, int dayOfWeek, int day, int month, int year);
void RTC_SET_TIMER(int interval);
void RTC_ENABLE_TIMER_INTERRUPT();
void RTC_DISABLE_TIMER_INTERRUPT();
void RTC_ENABLE_ALARM_1();
void RTC_DISABLE_ALARM_1();
void RTC_ENABLE_ALARM_2();
void RTC_DISABLE_ALARM_2();
void RTC_SET_ALARM_1();
void RTC_SET_ALARM_2();
void RTC_CLEAR_STATUS();
void RTC_SLEEP(int interval);
void RTC_WAKE(int interval);
void RTC_SET_TIME(int second, int minute, int hour, int dayOfWeek, int day, int month, int year);
void RTC_READ_TIME(int *second, int *minute, int *hour, int *dayOfWeek, int *day, int *month, int *year);
uint8_t readRegisterByte (uint8_t deviceAddress, uint8_t registerAddress);
uint8_t writeRegisterByte (uint8_t deviceAddress, uint8_t registerAddress, uint8_t newRegisterByte);
uint8_t bcdToDec(uint8_t val);
uint8_t decToBcd(uint8_t val);


/*
	x &= ~(1 << n);	// AND inverse (~) forces nth bit of x to be 0. All other bits left alone
	x |= (1 << n);	// OR forces nth bit of x to be 1.  All other bits left alone
*/

/**************************************************************************/
/*!
	@brief  Initialises the MAX31342 RTC
    @param second Seconds as an int
	@param minute Minutes as an int
	@param hour Hours as an int
	@param DOW Day of Week as an int 1 - 7
	@param day Day of month as an int
	@param month Month as an int
	@param year Year as an int
*/
/**************************************************************************/
void RTC_INIT(int second, int minute, int hour, int dayOfWeek, int day, int month, int year)
{
	#define CONFIG_MASK_01 0b00000000
	#define CONFIG_MASK_02 0b00000010

	//	Control Register 1.
	uint8_t CONFIG_DATA_01 = CONFIG_MASK_01;
	CONFIG_DATA_01 |= (1 << 0);	// Exit software reset - SWRSTN = 1 in BIT 0
	CONFIG_DATA_01 &= ~(1 << 3);	// Enable oscillator - OSCONZ = 0 in BIT 3
	writeRegisterByte(RTC_ADDRESS, CONFIG_REG_01, CONFIG_DATA_01); // Write the modified config to the config register.

	//	Set Datetime.
	RTC_SET_TIME(second, minute, hour, dayOfWeek, day, month, year); // Write RTC time to registers 0x06-0x0C.

	//	Control Register 2.
	writeRegisterByte(RTC_ADDRESS, CONFIG_REG_02, CONFIG_MASK_02);	// Write SET_RTC = 1 in BIT 1
	delay(10); // Delay for 10ms to allow RTC to transition RTC enable.

	uint8_t configData02 = CONFIG_MASK_02;
	configData02 &= ~(1 << 1);	// Write SET_RTC = 0 in BIT 1
	writeRegisterByte(RTC_ADDRESS, CONFIG_REG_02, configData02); // Write the modified config to the config register.

	RTC_CLEAR_STATUS();
}

void RTC_SET_TIMER(int interval)
{
	//#define TIMER_CONFIG_MASK 0b00000111
	#define TIMER_CONFIG_MASK 0b00000000

	//uint8_t TIMER_CONFIG = readRegisterByte(RTC_ADDRESS, TIMER_REG); // 03h (Timer_config)
	uint8_t TIMER_CONFIG = TIMER_CONFIG_MASK;
	//TIMER_CONFIG |= (1 << 2);	// Enable timer repeat (1 in BIT 2)
	TIMER_CONFIG &= ~(1 << 4);	// Reset timer to allow setting countdown interval (0 in BIT 4)
	writeRegisterByte(RTC_ADDRESS, TIMER_REG, TIMER_CONFIG); // Write the modified config to the timer config register.

	// Write the interval value to the timer register
	writeRegisterByte(RTC_ADDRESS, TIMER_INIT, decToBcd(interval)); // 900000ms 15m

	void RTC_ENABLE_TIMER_INTERRUPT(); // Enable the timer interrupt

	// Enable timer countdown to begin countdown with specified interval
	TIMER_CONFIG |= (1 << 4);	// Enable timer countdown (1 in BIT 4)
	TIMER_CONFIG &= ~(1 << 5);	// Resume timer (0 in BIT 5)
	writeRegisterByte(RTC_ADDRESS, TIMER_REG, TIMER_CONFIG); // Write the modified config to the timer config register.
}

void RTC_ENABLE_TIMER_INTERRUPT()
{
	uint8_t INTERRUPT_CONFIG = readRegisterByte(RTC_ADDRESS, INTERRUPT_REG); // 04h (Interrupt config)
	INTERRUPT_CONFIG |= (1 << 2); // Enable Timer interrupt enable (1 in BIT 2)
	writeRegisterByte(RTC_ADDRESS, INTERRUPT_REG, INTERRUPT_CONFIG); // Write the modified config to the timer config register.
}

void RTC_DISABLE_TIMER_INTERRUPT()
{
	uint8_t INTERRUPT_CONFIG = readRegisterByte(RTC_ADDRESS, INTERRUPT_REG); // 04h (Interrupt config)
	INTERRUPT_CONFIG &= ~(1 << 2); // Disable Timer interrupt enable (0 in BIT 2)
	writeRegisterByte(RTC_ADDRESS, INTERRUPT_REG, INTERRUPT_CONFIG); // Write the modified config to the timer config register.
}

void RTC_ENABLE_ALARM_1()
{
	uint8_t INTERRUPT_CONFIG = readRegisterByte(RTC_ADDRESS, INTERRUPT_REG); // 04h (Interrupt config)
	INTERRUPT_CONFIG |= (1 << 0); // Enable Alarm 1 interrupt (1 in BIT 0)
	writeRegisterByte(RTC_ADDRESS, INTERRUPT_REG, INTERRUPT_CONFIG); // Write the modified config to the timer config register.
}

void RTC_DISABLE_ALARM_1()
{
	uint8_t INTERRUPT_CONFIG = readRegisterByte(RTC_ADDRESS, INTERRUPT_REG); // 04h (Interrupt config)
	INTERRUPT_CONFIG &= ~(1 << 0); // Disable Alarm 1 interrupt (0 in BIT 0)
	writeRegisterByte(RTC_ADDRESS, INTERRUPT_REG, INTERRUPT_CONFIG); // Write the modified config to the timer config register.
}

void RTC_ENABLE_ALARM_2()
{
	uint8_t INTERRUPT_CONFIG = readRegisterByte(RTC_ADDRESS, INTERRUPT_REG); // 04h (Interrupt config)
	INTERRUPT_CONFIG |= (1 << 1); // Enable Alarm 2 interrupt (1 in BIT 1)
	writeRegisterByte(RTC_ADDRESS, INTERRUPT_REG, INTERRUPT_CONFIG); // Write the modified config to the timer config register.
}

void RTC_DISABLE_ALARM_2()
{
	uint8_t INTERRUPT_CONFIG = readRegisterByte(RTC_ADDRESS, INTERRUPT_REG); // 04h (Interrupt config)
	INTERRUPT_CONFIG &= ~(1 << 1); // Disable Alarm 2 interrupt (0 in BIT 1)
	writeRegisterByte(RTC_ADDRESS, INTERRUPT_REG, INTERRUPT_CONFIG); // Write the modified config to the timer config register.
}

void RTC_SET_ALARM_1()
{
	Wire.beginTransmission(RTC_ADDRESS);
	Wire.write(0xD); // Set pointer to beginning register (seconds)

	// Set alarm flags
	uint8_t seconds = decToBcd(20);
	seconds &= ~(1 << 7);
	Wire.write(seconds); // set seconds

	uint8_t minutes = decToBcd(15);
	minutes |= (1 << 7);
	Wire.write(minutes); // set minutes

	uint8_t hours = decToBcd(12);
	hours |= (1 << 7);
	Wire.write(hours); // set hours

	uint8_t date = decToBcd(05);
	date |= (1 << 7);
	Wire.write(date); // set date (1 to 31)

	uint8_t month = decToBcd(1);
	month |= (1 << 6);
	month |= (1 << 7);
	Wire.write(month); // set month

	uint8_t year = decToBcd(21);
	Wire.write(year); // set year (0 to 99)

	Wire.endTransmission();
}

void RTC_SET_ALARM_2()
{
	Wire.beginTransmission(RTC_ADDRESS);
	Wire.write(0x13); // Set pointer to beginning register (seconds)

	// Set alarm flags
	uint8_t minutes = decToBcd(15);
	minutes &= ~(1 << 7);	//	Disable A2M2 (Minute match alarm)
	//minutes |= (1 << 7);	//	Enable A2M2 (Once per minute alarm)
	Wire.write(minutes);	//	set minutes

	uint8_t hours = decToBcd(12);
	hours |= (1 << 7);	//	Enable A2M3 (Once per minute alarm)
	Wire.write(hours); // set hours

	uint8_t date = decToBcd(05);
	date |= (1 << 7);	//	Enable A2M4 (Once per minute alarm)
	Wire.write(date); // set date (1 to 31)

	Wire.endTransmission();
}

void RTC_CLEAR_STATUS()
{
	readRegisterByte(RTC_ADDRESS, INTERRUPT_STATUS_REG);
}

void RTC_SLEEP()
{
	// Set the configuration for config register 2.
	uint8_t configData02 = readRegisterByte(RTC_ADDRESS, CONFIG_REG_02); // Config_reg2 (0x01)
	
	//	Enable Data Retention mode.
	configData02 |= (1 << 6); // Enable Data Retention mode - DATA_RETEN = 1 in BIT 6.
	writeRegisterByte(RTC_ADDRESS, CONFIG_REG_02, configData02);
	
	// Set the configuration for config register 1.
	uint8_t configData01 = readRegisterByte(RTC_ADDRESS, CONFIG_REG_01); // Config_reg1 (0x00)
	
	//	Disable oscillator.
	configData01 |= (1 << 3); // Disable oscillator - OSCONZ = 1 in BIT 3.
	writeRegisterByte(RTC_ADDRESS, CONFIG_REG_01, configData01); // Write the modified config to the config register.
}

void RTC_WAKE()
{
	// Set the configuration for config register 2.
	uint8_t configData02 = readRegisterByte(RTC_ADDRESS, CONFIG_REG_02); // Config_reg2 (0x01)
	
	//	Enable Data Retention mode.
	configData02 &= ~(1 << 6); // Enable Data Retention mode - DATA_RETEN = 0 in BIT 6.
	writeRegisterByte(RTC_ADDRESS, CONFIG_REG_02, configData02);
	
	// Set the configuration for config register 1.
	uint8_t configData01 = readRegisterByte(RTC_ADDRESS, CONFIG_REG_01); // Config_reg1 (0x00)
	
	//	Disable oscillator.
	configData01 &= ~(1 << 3); // Disable oscillator - OSCONZ = 0 in BIT 3.
	writeRegisterByte(RTC_ADDRESS, CONFIG_REG_01, configData01); // Write the modified config to the config register.
}

void RTC_SET_TIME(int second, int minute, int hour, int dayOfWeek, int day, int month, int year)
{
	// Sets datetime data to MAX31342
	Wire.beginTransmission(RTC_ADDRESS);
	Wire.write(RTC_TIME_REGISTER); // Set pointer to SECONDS register (first in the order)
	Wire.write(decToBcd(second)); // set seconds
	Wire.write(decToBcd(minute)); // set minutes
	Wire.write(decToBcd(hour)); // set hours
	Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Monday, 7=Sunday)
	Wire.write(decToBcd(day)); // set date (1 to 31)
	Wire.write(decToBcd(month)); // set month
	Wire.write(decToBcd(year)); // set year (0 to 99)
	Wire.endTransmission();
}

void RTC_READ_TIME(int *second, int *minute, int *hour, int *dayOfWeek, int *day, int *month, int *year)
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
	*day = bcdToDec(Wire.read());
	*month = bcdToDec(Wire.read());
	*year = bcdToDec(Wire.read());
}

uint8_t readRegisterByte (uint8_t deviceAddress, uint8_t registerAddress)
{
	//uint8_t registerData;
	Wire.beginTransmission(deviceAddress);	// set sensor target
	Wire.write((uint8_t)registerAddress);	// set memory pointer
	Wire.endTransmission(false);

	Wire.requestFrom(deviceAddress, (uint8_t)1);	// request one byte
	//registerData = Wire.read();
	// you could add more data reads here if you request more than one byte
	//return registerData;	// the returned byte from this function is the content from registerAddress
	return Wire.read();
}

uint8_t writeRegisterByte (uint8_t deviceAddress, uint8_t registerAddress, uint8_t newRegisterByte)
{
	uint8_t result;
	Wire.beginTransmission(deviceAddress);
	Wire.write(registerAddress);  
	Wire.write(newRegisterByte); 
	result = Wire.endTransmission(); // Wire.endTransmission(); returns 0 if write operation was successful

	//delay(10);

	if(result > 0)
	{
		usbUSART.print(F("FAIL in I2C register write! Error code: "));
		usbUSART.println(result);
	}

	return result;	// the returned value from this function could be tested as shown above
	//it’s a good idea to check the return from Wire.endTransmission() the first time you write to a sensor 
	//if the first test is okay (result is 0), then I2C sensor coms are working and you don’t have to do extra tests
} 


/**************************************************************************/
/*!
    @brief  Convert a binary coded decimal value to a decimal. RTC stores time/date
   values as BCD.
    @param val BCD value
    @return decimal value
*/
/**************************************************************************/
uint8_t bcdToDec(uint8_t val)
{
	return( (val/16*10) + (val%16) );
}

/**************************************************************************/
/*!
    @brief  Convert a decimal value to BCD format for the RTC registers
    @param val Decimal value
    @return BCD value
*/
/**************************************************************************/
uint8_t decToBcd(uint8_t val)
{
	return( (val/10*16) + (val%10) );
}
