/**
 *  @file		RS485WindSensorSample.ino
 *
 *  @author		maro
 *  @date		mm/dd/yyyy
 *  @version	x.y
 *
 *  @brief 		Main
 *
 *  @section DESCRIPTION
 *
 *  This example shows how to use the RS485WindSensor library
 *
 *  used libraries and references:
 *  	-Modbus  "https://github.com/maroprjs/Modbus"
 *
 *
 */



/////////// INcludes /////////////////
#include "TypeDefs.h"
#include <stdio.h>
#include "Modbus.h"
#include "WindSensor.h"

/////////defines //////////////////////

#define MODBUS_SPEED (9600)

#define PUBLISH_INTERVAL   (1500)

////////// Object Instantiation //////////////////
char measData[512] ;

/******************************************
 * Modbus for Temperature-/Humidity- and Wind- Sensor
 */
Modbus modbus(0,3,0);//master adrress, serial id, tx pin(not used, this parameter is overwritten in library)

/*******************
 * RS485 Wind Sensor
 */
windspeed_t wind = 0;
char str_wind[6];
WindSensor windSensor(&modbus, &wind);

//////// Global primitive variables /////////////////
interval_t publish_interval = PUBLISH_INTERVAL;
elapsedMillis_t currentMillis = 0;
elapsedMillis_t previousMillis = 0;



void setup()
{
	Serial.begin( 9600 );
	modbus.begin(MODBUS_SPEED);
	windSensor.setup();
}


void loop()
{

	//////////////////// Let it roll  ////////////////////////////////
	windSensor.loop();
	currentMillis = millis();
	if (currentMillis - previousMillis >= publish_interval) {

		/////////////////////   Measurement Reports /////////////////////////
       /****
        *   %.2f not working here due performance...
        *  3 is mininum width, 1 is precision; float value is copied onto str_temp
        */
       dtostrf(wind, 3, 1, str_wind);
	   sprintf(measData,"{\"topic\":\"meas\",\"wind\":%s}", str_wind);
	   //sprintf(measData,"{\"topic\":\"meas\",\"wind\":%.2f}", wind );
	   Serial.println(measData);
	   memset(measData, 0, 512);

	   previousMillis = millis();
    }

}
