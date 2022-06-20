#include <Wire.h>
#include <Catena.h>
#include <IpsI2C.h>

#define  I2C_ADDRESS  0x4b

cIpsSensor ips_sensor;

//int success_count;
//int fail_count;
//int last = true;

void setup()
    {
    // Initiate USB serial at 115200 baud
    Serial.begin(115200);
    while (! Serial);
    Serial.println("\nI2C Test");
    // Wait on IPS boot
    delay(1000);

    // Initiate I2C connection SDA 21, SCL 22 by default
    // Set different pins by using:
    // ips_sensor.begin(SDA_PIN, SCL_PIN)
    //ips_sensor.begin(36, 35);;
    Wire.begin(I2C_ADDRESS);
    }

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
