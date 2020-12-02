/*

*/

#include "init.h"
#include "LoRaWAN.h"
#include "ttn-otaa-credentials.h"
#include "ttn-apb-credentials.h"

#define OTAA 1 // TODO: #2 OTAA not working with TTN Gateway

void INIT_TTN()
{
		// Wait a maximum of 10s for Serial Monitor
	//while (!usbUSART && millis() < 10000)
	//	;


	if (OTAA)
	{
		usbUSART.println("-- STATUS");
		ttn.showStatus();

		usbUSART.println("-- JOIN");
		ttn.join(appEui, appKey);
	}
	else
	{
		usbUSART.println("-- PERSONALIZE");
		ttn.personalize(devAddr, nwkSKey, appSKey);

		usbUSART.println("-- STATUS");
		ttn.showStatus();
	}
}