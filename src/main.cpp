/* 
	Title: Main.cpp
	Description: 
	Date: 09/11/2020
	Author: Ethan Bellmer 
*/

// Library Includes
#include "init.h"
#include "bme280.h"
#include "LoRaWAN.h" // Disable for testing outside of LoRa range
#include "adc.h"
#include "lowPower.h"
#include "deviceID.h"

// Variable Definitions
#define BAT_SENSE_EN 	PIN_PF4
#define RTC_INT 		PIN_PD2

//
void setup()
{
	delay(3000); // Wait for USART to come up.

	usbUSART.begin(9600);
	loraUSART.begin(57600);

	delay(5000); // Wait for USART to come up. 

	usbUSART.print("Weather Station Prototype ");
	usbUSART.println(UNIT_ID);

	
	TTN_INIT(); // Initialise the LoRa radio & connect to network // Disable for testing outside of LoRa range
	
	I2C_INIT(); // Initialise the I2C interface
	RTC_INIT(00,05,10,05,01,01,21); // Init the RTC module // TODO: #1 Get datetime from TTN
	
	BME_INIT(); // Init the BME sensor on the I2C bus and set the sensor to low power sleep
	ADC_INIT_ALL();	// initialise all available ADCs (sensors & power)

	pinMode(BAT_SENSE_EN, OUTPUT);	// Set Bettery Sense enable pin to output
	pinMode(RTC_INT, INPUT);	// Set RTC interrupt pin as input

	RTC_DISABLE_TIMER_INTERRUPT(); // Disable timer interrupt in case it is active.
	RTC_DISABLE_ALARM_1(); // Disable Alarm 1 interrupt in case it is active.
	RTC_ENABLE_ALARM_2(); // Enable Alarm 2 interrupts
	RTC_SET_ALARM_2();
	RTC_CLEAR_STATUS();	

	MCU_ATTACH_INTERRUPT();	// Set pin interrupt for waking from sleep

	delay(50);
}

void loop()
{
	//ledBlink();

	BME_READ(); // Read data from the BME and return to sleep

	float rainDetect = SENSOR_ADC_GET_CHANNEL(0);
	float ch3 = SENSOR_ADC_GET_CHANNEL(3);
	float ch1 = SENSOR_ADC_GET_CHANNEL(1);
	float ch2 = SENSOR_ADC_GET_CHANNEL(2);

	/*
	float solarV = POWER_ADC_GET_CHANNEL(0);
	float solarI = POWER_ADC_GET_CHANNEL(1);

	digitalWrite(BAT_SENSE_EN, HIGH);
	float battV = POWER_ADC_GET_CHANNEL(3);
	digitalWrite(BAT_SENSE_EN, LOW);
	*/

	// Sensor ADC Testing
	usbUSART.println("Sensor ADC Testing: ");
	usbUSART.print("Rain Detect (ch0): ");
	usbUSART.print(rainDetect,5);
	usbUSART.print(" CH1: ");
	usbUSART.print(ch1,5);
	usbUSART.print(" CH2: ");
	usbUSART.print(ch2,5);
	usbUSART.print(" CH3: ");
	usbUSART.println(ch3,5);
	usbUSART.println("");

	/*
	// Power ADC testing
	usbUSART.println("Power ADC Testing: ");
	usbUSART.print("Solar Voltage (ch0): ");
	usbUSART.print(solarV,5);
	usbUSART.print(" Solar Current (ch01): ");
	usbUSART.print(solarI,5);
	usbUSART.print(" Battery Voltage: ");
	usbUSART.println(battV,5);
	usbUSART.println("");
	*/

	uint8_t payload[12];

	int16_t ttemp = BME_GET_TEMP_C() * 100;	// Multiply temperature by 100 to convert it to an integer
	payload[0] = highByte(ttemp);
	payload[1] = lowByte(ttemp);

	payload[2] = BME_GET_HUMIDITY();	// Humidity is always under 255 so no conversion needed
	
	uint16_t pressure = BME_GET_PRESSURE() / 100; // Pressure is 6 digits, but uint16_t supports up to 5 digits. uint32_t likely a waste of packet width.
	payload[3] = highByte(pressure);
	payload[4] = lowByte(pressure);

	uint16_t alt = BME_GET_ALT_M() * 10;	// Multiply altitude by 10 to convert it to an integer
	payload[5] = highByte(alt);
	payload[6] = lowByte(alt);

	digitalWrite(BAT_SENSE_EN, HIGH);
	uint16_t bV = POWER_ADC_GET_CHANNEL(3) * 100;
	digitalWrite(BAT_SENSE_EN, LOW);
	payload[7] = highByte(bV);
	payload[8] = lowByte(bV);
	
	uint16_t sV = POWER_ADC_GET_CHANNEL(0) * 100;
	payload[9] = highByte(sV);
	payload[10] = lowByte(sV);

	//payload[11] = (uint8_t) POWER_ADC_GET_CHANNEL(1);
	payload[11] = (bool) SENSOR_ADC_GET_RAIN();

	TTN_SEND_BYTES(payload);	// Send payload data to TTN

	MCU_SLEEP();	// Set the MCU into sleep mode and wait for RTC iunterrupt
	RTC_CLEAR_STATUS();	// Clear the interrupt register of the RTC, ready for the next cycle.
}
