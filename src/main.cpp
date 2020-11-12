/*

*/

// Includes
#include "init.h"
#include "bme280.h"
//#include "LoRaWAN.h" // Disable for testing outside of LoRa range


// Function Declarations
void ledInit();
void ledBlink();

// Body
void setup()
{
	ledInit();

	usbUSART.begin(9600);
	loraUSART.begin(57600);
	//while (!usbUSART && !loraUSART){}
	delay(2000); // Wait for USART to come up. 

	usbUSART.println("Weather Station Prototype 1");

	//INIT_TTN(); // Initialise the LoRa radio & connect to network // Disable for testing outside of LoRa range

	I2C_INIT(); // Initialise the I2C interface
	BME_INIT(); // Init the BME sensor on the I2C bus and set the sensor to low power sleep
	
	uint8_t *payload = (uint8_t *) 200; // TTN test payload
	
	//ttn.sendBytes(payload, sizeof(payload));
	delay(2000);
}

void loop()
{
	ledBlink();

	BME_READ(); // Read data from the BME and return to sleep
	
	usbUSART.print(" Measure time(ms): ");
	usbUSART.print(md.endTime - md.startTime);
	
	float h = BME_GET_HUMIDITY();
	float p = BME_GET_PRESSURE();
	float a = BME_GET_ALT_M();
	float t = BME_GET_TEMP_C();
	float rp = BME_GET_REF_PRESSURE();
	float dp = BME_GET_DEWPOINT_C();

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

	//usbUSART.println("TXing");
    //ttn.sendBytes(payload, sizeof(payload)); //one byte, blocking function

	delay(1000);
}
