/*
Module: 7100_catena_uart.ino

Function:
        Test sketch to connect MCCI Catena4610 board and Piera Particle Sensor-7100 using UART Protocol

Copyright notice:
        This file copyright (C) 2022 by
        MCCI Corporation
        3520 Krums Corners Road
        Ithaca, NY 14850
        An unpublished work. All rights reserved.
        This file is proprietary information, and may not be disclosed or
        copied without the prior permission of MCCI Corporation.

Author:
        Pranau R, MCCI Corporation   June 2022
*/

#include <Catena.h>

String serial1Out;
String serialIn;

/****************************************************************************\
|
|   Code.
|
\****************************************************************************/

/*

Name:	setup()

Function:
        Arduino setup function.

Definition:
        void setup(
            void
            );

Description:
        To initiate a serial connection between board and display and to check the connectivity of 7100 Particle Sensor.

Returns:
        No explicit result.

*/

void setup() 
    {
    Serial.begin(115200);
    delay(500);

    while (! Serial)
    Serial1.begin(115200);
    Serial.println("");
    Serial.println("****  7100 Particle Sensor Example  ****");
    Serial.println("");

    // Wait 5 seconds for 7100 to boot up before sending commands
    delay(5000);

    // Send commands to the 7100 (see datasheet for full list)
    // Reset settings to defaults
    Serial1.write("$Wfactory=\r\n");
    delay(100);

    // Disable Azure integration
    Serial1.write("$Wazure=0\r\n");
    delay(100);
    }

/*
Name:   loop()

Function:
        To get particle readings and display it in serial monitor.

Definition:
        void loop (
            void
            );

Returns:
        No explicit result.
*/

void loop()
    {
    delay(200);
    // Check serial data from sensor
    if (Serial1.available())
        {
        serial1Out = Serial1.readStringUntil('\n');
        String fullOutput = serial1Out + "\r\n";
        if (serial1Out)
            {
            // Write sensor data to USB serial output
            Serial.write(fullOutput.c_str());
            }
        }
    }