/*
Module: 7100_catena_i2c.ino

Function:
        Test sketch to connect MCCI Catena4610 board and Piera Particle Sensor-7100 using I2C Protocol

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

#include <Wire.h>
#include <Catena.h>
#include <IpsI2C.h>

#define  I2C_ADDRESS  0x4b

cIpsSensor ips_sensor;

//int success_count;
//int fail_count;
//int last = true;

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
    // Initiate USB serial at 115200 baud
    Serial.begin(115200);
    while (! Serial);
    Serial.println("\n****  7100 Particle Sensor Example ****");
    // Wait on IPS boot
    delay(1000);

    //ips_sensor.begin();

    while(!ips_sensor.begin())
        {
        Serial.println("7100 Particle Sensor not connected!\n");
        delay(5000);
        }

    Serial.println("7100 Particle Sensor connected successfully!\n");
    delay (2000);
    // Initiate I2C connection SDA 21, SCL 22 by default
    // Set different pins by using:
    // ips_sensor.begin(SDA_PIN, SCL_PIN)
    //ips_sensor.begin(36, 35);;
    Wire.begin(I2C_ADDRESS);
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
    // Get new IPS sensor readings
    // Not meant to run more than once per second
    Serial.println("\nI2C Test Loop");

    ips_sensor.update();

    Serial.println("\nI2C Test Loop after update()");
    //Enable debugging
    // ips_sensor.setDebug(true);

    // Print PC0.1 via USB serial
    // Serial.println(ips_sensor.getPC01());

    // Print PM1.0 via USB serial
    Serial.print("PM1.0: ");
    Serial.print(ips_sensor.getPM10());
    Serial.print("\n");

    // Print PM2.5 via USB serial
    Serial.print("PM2.5: ");
    Serial.print(ips_sensor.getPM25());
    Serial.print("\n");

    // Print PM10 via USB serial
    Serial.print("PM10: ");
    Serial.print(ips_sensor.getPM100());
    Serial.print("\n");

    // Print PC1.0 via USB serial
    Serial.print("PC1.0: ");
    Serial.print(ips_sensor.getPC10());
    Serial.print("\n");

    // ips_sensor.setFan(true);
    // last = !last;

    // Print sensor status
    // int status = ips_sensor.getStatus();
    // Serial.println(status);

    // Print Vref value
    // int vref = ips_sensor.getVref();
    // Serial.println(vref);

    // Testing I2C accuracy
    // if (vref == 1800) {
    //   success_count++;
    // } else {
    //   fail_count++;
    // }
    // if (status == 1) {
    //   success_count++;
    // } else {
    //   fail_count++;
    // }
    // Serial.print("Success_count: ");
    // Serial.print(success_count);
    // Serial.print(" Fail_count: ");
    // Serial.print(fail_count);
    // Serial.print("\n");

    delay(1000);
    }
