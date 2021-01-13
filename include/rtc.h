/* 
	Title: RTC.h
	Description: 
	Date: 17/11/2020
	Author: Ethan Bellmer
*/

#include <Arduino.h> // Workaround for byte not declared error

// Variable Declarations
#define RTC_ADDRESS 0x69

// Registers
#define CONFIG_REG_01 0x00
#define CONFIG_REG_02 0x01
#define TIMER_REG 0x03
#define TIMER_INIT 0x17
//#define INT_REG 0x04
#define INTERRUPT_REG 0x04
#define INTERRUPT_STATUS_REG 0x05
#define RTC_TIME_REGISTER 0x06

#define ALARM_1_SEC_REG 0xD
#define ALARM_1_MIN_REG 0xE
#define ALARM_1_HRS_REG 0xF
#define ALARM_1_DAY_DATE_REG 0x10
#define ALARM_1_MON_REG 0x11
#define ALARM_1_YEAR_REG 0x12

#define ALARM_2_MIN_REG 0x13
#define ALARM_2_HRS_REG 0x14
#define ALARM_2_DAY_DATE_REG 0x15

// Function Definition
void RTC_INIT(int second, int minute, int hour, int dayOfWeek, int day, int month, int year);
void RTC_SET_TIMER(int interval);
void RTC_SLEEP(int interval);
void RTC_WAKE(int interval);
//void RTC_SET_TIME(int second, int minute, int hour, int dayOfWeek, int dayOfMonth, int month, int year);
//void RTC_READ_TIME(int *second, int *minute, int *hour, int *dayOfWeek, int *day, int *month, int *year);
uint8_t readRegisterByte (uint8_t deviceAddress, uint8_t registerAddress);
uint8_t bcdToDec(uint8_t val);
uint8_t decToBcd(uint8_t val);

void RTC_ENABLE_TIMER_INTERRUPT();
void RTC_DISABLE_TIMER_INTERRUPT();
void RTC_ENABLE_ALARM_1();
void RTC_DISABLE_ALARM_1();
void RTC_ENABLE_ALARM_2();
void RTC_DISABLE_ALARM_2();

void RTC_SET_ALARM_1();
void RTC_SET_ALARM_2();

void RTC_CLEAR_STATUS();

