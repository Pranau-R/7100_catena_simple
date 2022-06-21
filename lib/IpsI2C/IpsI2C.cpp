#include <Arduino.h>
#include <Wire.h>

#include "IpsI2C.h"

// For CRC16 checksum
#define POLY 0x8408

bool ips_debug = true;
cIpsSensor::cIpsSensor()
    {
    //default constructor
    }
bool cIpsSensor::begin()
{
  	// if no Wire is bound, fail.
    if (this->m_wire == nullptr)
        return this->setLastError(Error::NoWire);

    if (this->isRunning())
        return true;

    this->m_wire->begin();
    // assume it's in idle state.
    this->m_state = this->m_state == State::End ? State::Triggered : State::Initial;
    return true;
    /*Serial.print("cIpsSensor begin");
    Serial.print(sda);
    Serial.print(scl);
    Serial.print("\n");
    Wire.begin(sda, scl);
    Wire.setTimeOut(3000);
    //Wire.setClock(100000);
    delay(100);
    Wire.beginTransmission(0x4B);
    Wire.write(0x10);
    Wire.write(0x01);
    Wire.endTransmission();*/
    // Wire.beginTransmission(0x4B);
    // Wire.write(0x22);
    // Wire.write(0x00);
    // Wire.endTransmission();
    // Wire.beginTransmission(0x4B);
    // Wire.write(0x2d);
    // Wire.endTransmission();
    }

void cIpsSensor::end()
    {
    if (this->isRunning())
        this->m_state = State::End;
    }

void cIpsSensor::update()
    {
    Serial.println("\nI2C Test inside update()");
    // Read PC data
    uint8_t pc_raw_values[30];
    this->read_i2c(0x11, 30, pc_raw_values, true);
    Serial.println("\nI2C Test after read_i2c1 update()");

    // Assemble PC values (unsigned long) from 4 bytes via bitwise
    this->pc_values[0] = pc_raw_values[3] | (pc_raw_values[2] << 8) | (pc_raw_values[1] << 16) | (pc_raw_values[0] << 24);
    this->pc_values[1] = pc_raw_values[7] | (pc_raw_values[6] << 8) | (pc_raw_values[5] << 16) | (pc_raw_values[4] << 24);
    this->pc_values[2] = pc_raw_values[11] | (pc_raw_values[10] << 8) | (pc_raw_values[9] << 16) | (pc_raw_values[8] << 24);
    this->pc_values[3] = pc_raw_values[15] | (pc_raw_values[14] << 8) | (pc_raw_values[13] << 16) | (pc_raw_values[12] << 24);
    this->pc_values[4] = pc_raw_values[19] | (pc_raw_values[18] << 8) | (pc_raw_values[17] << 16) | (pc_raw_values[16] << 24);
    this->pc_values[5] = pc_raw_values[23] | (pc_raw_values[22] << 8) | (pc_raw_values[21] << 16) | (pc_raw_values[20] << 24);
    this->pc_values[6] = pc_raw_values[27] | (pc_raw_values[26] << 8) | (pc_raw_values[25] << 16) | (pc_raw_values[24] << 24);

    Serial.println("\nI2C Test before read_i2c2 update()");
    // Read PM data
    uint8_t pm_raw_values[32];
    this->read_i2c(0x12, 32, pm_raw_values, true);
    Serial.println("\nI2C Test after read_i2c2 update()");
    // Assemble PM values (float) from 4 bytes via union
    for (size_t i = 0; i < 7; ++i)
        {
        bytesToPM b;
        for (size_t j = 0; j < 4; ++j)
            {
            b.byte[j] = pm_raw_values[j + (i * 4)];
            }
        this->pm_values[i] = b.f;
        }
    }

// Get CRC16 checksum
uint16_t cIpsSensor::get_checksum(uint8_t *byte, int len)
    {
    int i, j;
    uint16_t data = 0;
    uint16_t crc = 0xffff;
    for (j = 0; j < len; j++)
        {
        data = (uint16_t)0xff & byte[j];
        for (i = 0; i < 8; i++, data >>= 1)
            {
            if ((crc & 0x0001) ^ (data & 0x0001))
                crc = (crc >> 1) ^ POLY;
            else
                crc >>= 1;
            }
        }
    crc = ~crc;
    data = crc;
    crc = (crc << 8) | (data >> 8 & 0xff);
    return crc;
    }

void cIpsSensor::read_i2c(unsigned char command, int reply_size, uint8_t res_array[], bool checksum)
    {
    //Serial.println("\nI2C Test inside readi2c()");
    bool checksum_pass = false;
    while (!checksum_pass)
        {
        //Serial.println("\nI2C Test inside readi2c() while loop");
        uint8_t *new_command = &command;
        Wire.write(new_command, 8);
        //Serial.println("\nI2C Test inside readi2c() after write()");
        Wire.requestFrom(0x4B, reply_size);
        for (int n = 0; n < reply_size; n++)
            {
            //Serial.println("\nI2C Test inside readi2c() for loop");
            res_array[n] = Wire.read();
            }

        // Debug raw bytes
        if (ips_debug)
            {
            //Serial.println("\nI2C Test inside readi2c() ips_debug raw bytes");
            Serial.print("[ ");
            for (int n = 0; n < reply_size; n++)
                {
                Serial.print(res_array[n]);
                Serial.print(" ");
                }
            Serial.print("]\n");
            }

        if (!checksum)
            {
            //Serial.println("\nI2C Test inside readi2c() !checksum");
            break;
            }
        uint16_t message_checksum = this->get_checksum(res_array, reply_size - 2);
        uint16_t received_checksum = (res_array[reply_size - 2] * 256) + res_array[reply_size - 1];
        if (ips_debug)
            {
            //Serial.println("\nI2C Test inside readi2c() ips_debug");
            Serial.print("Expected checksum: ");
            Serial.print(message_checksum);
            Serial.print(" Received checksum: ");
            Serial.print(received_checksum);
            Serial.print("\n");
            }
        if (message_checksum == received_checksum)
            {
            Serial.println("\nI2C Test inside readi2c() checksum = true");
            checksum_pass = true;
            }
        else
            {
            //Serial.println("\nI2C Test inside readi2c() checksum failed");
            // Checksum failed;
            if (ips_debug)
                {
                Serial.println("Checksum Failed.");
                }
            delay(100);
            }
        //Serial.println("\nI2C Test inside readi2c() while loop end");
        }
    Serial.println("\nI2C Test inside readi2c() after while loop");
    }

boolean cIpsSensor::write_i2c(unsigned char command, unsigned char parameter)
    {
    Wire.beginTransmission(0x4B);
    Wire.write(command);
    Wire.write(parameter);
    Wire.endTransmission();
    return true;
    }

unsigned long *cIpsSensor::getPC()
    {
    return this->pc_values;
    };

unsigned long cIpsSensor::getPC01()
    {
    return this->pc_values[0];
    };
unsigned long cIpsSensor::getPC03()
    {
    return this->pc_values[1];
    };
unsigned long cIpsSensor::getPC05()
    {
    return this->pc_values[2];
    };
unsigned long cIpsSensor::getPC10()
    {
    return this->pc_values[3];
    };
unsigned long cIpsSensor::getPC25()
    {
    return this->pc_values[4];
    };
unsigned long cIpsSensor::getPC50()
    {
    return this->pc_values[5];
    };
unsigned long cIpsSensor::getPC100()
    {
    return this->pc_values[6];
    };

float *cIpsSensor::getPM()
    {
    return this->pm_values;
    };

float cIpsSensor::getPM01()
    {
    return this->pm_values[0];
    }
float cIpsSensor::getPM03()
    {
    return this->pm_values[1];
    }
float cIpsSensor::getPM05()
    {
    return this->pm_values[2];
    }
float cIpsSensor::getPM10()
    {
    return this->pm_values[3];
    }
float cIpsSensor::getPM25()
    {
    return this->pm_values[4];
    }
float cIpsSensor::getPM50()
    {
    return this->pm_values[5];
    }
float cIpsSensor::getPM100()
    {
    return this->pm_values[6];
    }

int cIpsSensor::getVref()
    {
    // Read Vref
    uint8_t message[4];
    this->read_i2c(0x69, 4, message, true);
    unsigned short int vref;
    vref = message[1] | (message[0] << 8);
    return vref;
    }

int cIpsSensor::getStatus()
    {
    // Read Status
    uint8_t message[3];
    this->read_i2c(0x6A, 3, message, true);
    unsigned short int status;
    status = message[0];
    return status;
    }

bool cIpsSensor::setFan(bool status)
    {
    bool result;
    // Read Status
    if (status)
        {
        result = this->write_i2c(0x2B, 1);
        }
    else
        {
        result = this->write_i2c(0x2B, 0);
        }
    return result;
    }

void cIpsSensor::setDebug(bool debug_setting)
    {
    ips_debug = debug_setting;
    }