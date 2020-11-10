/*

*/

// Includes
#include <Arduino.h>
#include <Wire.h> // I2C library
//#include <SPI.h>
#include "SparkFunBME280.h" // BME280 library
#include <rn2xx3.h> // RN2483 LoRa radio library

// Variable Definitiions
#define LED_PIN_0 PIN_PD0
#define LED_PIN_1 PIN_PD1

// Class Instantiations
BME280 mySensor;

// Function Declarations
void ledInit();
void ledBlink();

// Body
void setup()
{
	ledInit();

	//Serial1.begin(9600);
	Serial2.begin(9600);

	Wire.pins(PIN_PA2, PIN_PA3);
	Wire.begin(); // Start the I2C interface
	//Wire.setClock(400000); //Increase to fast I2C speed!
	Wire.setClock(100000);


	mySensor.setI2CAddress(0x76);
	mySensor.beginI2C();
	//mySensor.beginI2C(); // Start the I2C interface on the BME
	mySensor.setMode(MODE_SLEEP); // Set BME to sleep
}

void loop()
{
	ledBlink();

	//Serial1.println("Serial1 Test");
	Serial2.println("Weather Station Prototype 1");
	delay(500);


	mySensor.setMode(MODE_FORCED); //Wake up sensor and take reading

	long startTime = millis();
	while(mySensor.isMeasuring() == false) ; //Wait for sensor to start measurment
	while(mySensor.isMeasuring() == true) ; //Hang out while sensor completes the reading    
	long endTime = millis();

	//Sensor is now back asleep but we get get the data

	Serial2.print(" Measure time(ms): ");
	Serial2.print(endTime - startTime);

	Serial2.print(" Humidity: ");
	Serial2.print(mySensor.readFloatHumidity(), 0);

	Serial2.print(" Pressure: ");
	Serial2.print(mySensor.readFloatPressure(), 0);

	Serial2.print(" Alt: ");
	Serial2.print(mySensor.readFloatAltitudeMeters(), 1);
	//Serial2.print(mySensor.readFloatAltitudeFeet(), 1);

	Serial2.print(" Temp: ");
	Serial2.print(mySensor.readTempC(), 2);
	//Serial2.print(mySensor.readTempF(), 2);
	Serial2.println("");

	delay(5000);


}


void ledInit()
{
	// initialize LED digital pin as an output.
	pinMode(LED_PIN_0, OUTPUT);
	pinMode(LED_PIN_1, OUTPUT);
}

void ledBlink()
{
	// turn the LED on (HIGH is the voltage level)
	digitalWrite(LED_PIN_0, HIGH);
	digitalWrite(LED_PIN_1, LOW);
	
	// wait for a second
	delay(1000);
	
	// turn the LED off by making the voltage LOW
	digitalWrite(LED_PIN_0, LOW);
	digitalWrite(LED_PIN_1, HIGH);
}