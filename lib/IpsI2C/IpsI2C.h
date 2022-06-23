#pragma once
#include <cstdint>
#include <Wire.h>

union bytesToPM
    {
    float f;
    unsigned char byte[4];
    };

class cIpsSensor
    {
private:
    static constexpr bool kfDebug = false;

public:
// the address type:
    enum class Address : std::int8_t
        {
        Error = -1,
        IPS7100 = 0x4b,
        };

    // the type for pin assignments, in case the ready pin is used
    using Pin_t = std::int8_t;

    // constructor
    cIpsSensor(TwoWire &wire, Address Address = Address::IPS7100, Pin_t pinReady = -1)
        : m_wire(&wire)
        , m_address(Address)
        , m_pinReady(pinReady)
        {}

    // neither copyable nor movable
    cIpsSensor(const cIpsSensor&) = delete;
    cIpsSensor& operator=(const cIpsSensor&) = delete;
    cIpsSensor(const cIpsSensor&&) = delete;
    cIpsSensor& operator=(const cIpsSensor&&) = delete;

    // the errors
    enum class Error : std::uint8_t
        {
        Success = 0,
        NoWire,
        CommandWriteFailed,
        CommandWriteBufferFailed,
        InternalInvalidParameter,
        I2cReadShort,
        I2cReadRequest,
        I2cReadLong,
        Busy,
        NotMeasuring,
        Crc,
        Uninitialized,
        InvalidParameter,
        InternalInvalidState,
        SensorUpdateFailed,
        };

    enum class State : std::uint8_t
        {
        Uninitialized,      /// this->begin() has never succeeded.
        End,                /// this->begin() succeeded, followed by this->end()
        Initial,            /// initial after begin [indeterminate]
        Idle,               /// idle (not measuring)
        Triggered,          /// continuous measurement running, no data available.
        Ready,              /// continuous measurement running, data availble.
        };

private:
    // this is internal -- centralize it but require that clients call the
    // public method (which centralizes the strings and the search)
    static constexpr const char * const m_szErrorMessages =
        "Success\0"
        "NoWire\0"
        "CommandWriteFailed\0"
        "CommandWriteBufferFailed\0"
        "InternalInvalidParameter\0"
        "I2cReadShort\0"
        "I2cReadRequest\0"
        "I2cReadLong\0"
        "Busy\0"
        "NotMeasuring\0"
        "Crc\0"
        "Uninitialized\0"
        "InvalidParmaeter\0"
        "InternalInvalidState\0"
        ;

    // this is internal -- centralize it but require that clients call the
    // public method (which centralizes the strings and the search)
    static constexpr const char * const m_szStateNames =
        "Uninitialized" "\0"
        "End"           "\0"
        "Initial"       "\0"
        "Idle"          "\0"
        "Triggered"     "\0"
        "Ready"         "\0"
        ;

public:
    cIpsSensor();
    virtual ~cIpsSensor();
    unsigned long pc_values[7] = {0, 0, 0, 0, 0, 0, 0};
    float pm_values[7] = {0, 0, 0, 0, 0, 0, 0};
    //void begin(int sda = 21, int scl = 22);
    bool begin();
    void end();
    void update();
    unsigned long *getPC();
    unsigned long getPC01();
    unsigned long getPC03();
    unsigned long getPC05();
    unsigned long getPC10();
    unsigned long getPC25();
    unsigned long getPC50();
    unsigned long getPC100();
    float *getPM();
    float getPM01();
    float getPM03();
    float getPM05();
    float getPM10();
    float getPM25();
    float getPM50();
    float getPM100();
    int getVref();
    int getStatus();
    bool setFan(bool);
    bool start();
    void read_i2c(unsigned char, int, uint8_t[], bool checksum = false);
    bool write_i2c(unsigned char, unsigned char);
    uint16_t get_checksum(uint8_t *byte, int);
    void setDebug(bool);
    bool isRunning() const
        {
        return this->m_state > State::End;
        }
    bool setLastError(Error e)
        {
        this->m_lastError = e;
        return e == Error::Success;
        }


private:
    TwoWire *m_wire;                /// pointer to bus to be used for this device
    std::uint32_t m_tReady;         /// estimated time next measurement will be ready (millis)
    Address m_address;              /// I2C address to be used
    Pin_t m_pinReady;               /// alert pin, or -1 if none.
    Error m_lastError;              /// last error.
    State m_state                   /// current state
        { State::Uninitialized };   // initially not yet started.

    static constexpr std::uint16_t getUint16BE(const std::uint8_t *p)
        {
        return (p[0] << 8) + p[1];
        }
    static constexpr std::int16_t getInt16BE(const std::uint8_t *p)
        {
        return std::int16_t((p[0] << 8) + p[1]);
        }
    static float getFloat32BE(const std::uint8_t *p);
};