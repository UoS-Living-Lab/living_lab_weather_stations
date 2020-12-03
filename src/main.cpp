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


// Variable Declarations
// Function Declarations



// Body
void setup()
{
	ledInit();

	usbUSART.begin(9600);
	loraUSART.begin(57600);

	delay(2000); // Wait for USART to come up. 

	usbUSART.println("Weather Station Prototype 1");

	//INIT_TTN(); // Initialise the LoRa radio & connect to network // Disable for testing outside of LoRa range

	I2C_INIT(); // Initialise the I2C interface
	RTC_INIT(00,00,13,02,01,12,2020); // Init the RTC module // TODO: #1 Get datetime from TTN
	MCU_SET_INT(); // Set pin interrupt for waking from sleep
	BME_INIT(); // Init the BME sensor on the I2C bus and set the sensor to low power sleep
	SENSOR_ADC_INIT(); // Init the sensor ADC 
	

	RTC_SET_TIMER(1);

	//uint8_t *payload = (uint8_t *) 200; // TTN test payload
	

	pinMode(PIN_PD2, INPUT);


	//ttn.sendBytes(payload, sizeof(payload));
	delay(2000);
}

void loop()
{
	ledBlink();

	BME_READ(); // Read data from the BME and return to sleep
	
	//usbUSART.print(" Measure time(ms): ");
	//usbUSART.print(md.endTime - md.startTime);
	
	float h = BME_GET_HUMIDITY();
	float p = BME_GET_PRESSURE();
	float a = BME_GET_ALT_M();
	float t = BME_GET_TEMP_C();
	float rp = BME_GET_REF_PRESSURE();
	float dp = BME_GET_DEWPOINT_C();

	/*
	usbUSART.print(" Humidity: ");
	usbUSART.print(h, 0);
	
	usbUSART.print(" Pressure: ");
	usbUSART.print(p, 0);
	
	usbUSART.print(" Alt: ");
	usbUSART.print(a, 1);
	
	usbUSART.print(" Temp: ");
	usbUSART.print(t, 2);

	usbUSART.print(" Dew Point: ");
	usbUSART.print(dp, 2);

	usbUSART.println("");
	
	usbUSART.print(" Reference pressure: ");
	usbUSART.print(rp, 0);
	usbUSART.println("");
	*/

	float rainDetect = SENSOR_ADC_GET_CHANNEL(0);
	float ch3 = SENSOR_ADC_GET_CHANNEL(3);
	float ch1 = SENSOR_ADC_GET_CHANNEL(1);
	float ch2 = SENSOR_ADC_GET_CHANNEL(2);

	usbUSART.println("");

	usbUSART.print("Rain Detect (ch0): ");
	usbUSART.print(rainDetect,5);

	usbUSART.print(" CH1: ");
	usbUSART.print(ch1,5);

	usbUSART.print(" CH2: ");
	usbUSART.print(ch2,5);

	usbUSART.print(" CH3: ");
	usbUSART.print(ch3,5);

	usbUSART.println("");


	usbUSART.print("INT PIN: ");
	uint8_t INT_PIN = digitalRead(PIN_PD2);
	usbUSART.print(INT_PIN);

	usbUSART.print(" 0x00: ");
	uint8_t tmp = readRegister(0x00);
	usbUSART.print(tmp);

	usbUSART.print(" 0x01: ");
	tmp = readRegister(0x01);
	usbUSART.print(tmp);
	
	usbUSART.print(" 0x03: ");
	tmp = readRegister(0x03);
	usbUSART.print(tmp);
	usbUSART.println("");

	//usbUSART.println("TXing");
	//ttn.sendBytes(payload, sizeof(payload)); //one byte, blocking function

	delay(5000);
}
