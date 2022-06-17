#include <Catena.h>

String serial1Out;
String serialIn;

void setup() 
    {
    Serial.begin(115200);
    delay(500);

    while (! Serial)
    Serial1.begin(115200);
    Serial.println("");
    Serial.println("****  7100 ESP32 Example ****");
    Serial.println("****  Piera Systems      ****");
    Serial.println("");
    Serial.println("Warming up 7100...");

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
