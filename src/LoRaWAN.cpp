/*
	Title: LoRaWAN.cpp
	Description: 
	Date: 09/11/2020
	Author: Ethan Bellmer 
*/

#include "init.h"
#include "LoRaWAN.h"
#include "ttn-otaa-credentials.h"
#include "ttn-apb-credentials.h"

#define USE_OTAA 1 // TODO: #2 OTAA not working with TTN Gateway


void message(const uint8_t *payload, size_t size, port_t port);


void TTN_INIT()
{
	switch(USE_OTAA)	
	{
		case 0:
			usbUSART.println("-- PERSONALIZE");
			ttn.personalize(devAddr, nwkSKey, appSKey);

			usbUSART.println("-- STATUS");
			ttn.showStatus();
		case 1:
			ttn.onMessage(message);
			usbUSART.println("-- JOIN");
			ttn.join(appEui, appKey);

			usbUSART.println("-- STATUS");
			ttn.showStatus();
	}
}

void TTN_SHOW_STATUS()
{
	ttn.showStatus();
}

size_t GET_TTN_HE_EUI()
{
	char *buffer[512];
	return ttn.getHardwareEui(*buffer, sizeof(buffer));
}

void TTN_SEND_BYTES(uint8_t payload[], port_t port = 1)
{
	ttn.sendBytes(payload, sizeof(payload)); // Send payload data to TTN
}

int8_t TTN_GET_BYTES(port_t port = 1)
{
	return ttn.poll(port);
}

void message(const uint8_t *payload, size_t size, port_t port)
{
  usbUSART.println("-- MESSAGE");
  usbUSART.print("Received " + String(size) + " bytes on port " + String(port) + ":");

  for (int i = 0; i < size; i++)
  {
    usbUSART.print(" " + String(payload[i]));
  }

  usbUSART.println();
}