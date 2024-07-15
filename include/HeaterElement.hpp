#ifndef HEATERELEMENT_HPP
#define HEATERELEMENT_HPP

#include <SPI.h>
#include <MAX6675.h>
#include <ArduPID.h>

class HeaterElement
{
public:
    using TemperatureChangeCallback = std::function<void(double)>;

    HeaterElement(uint8_t thermoCoupleCsPin, uint8_t pwmPin);

    void begin(double kp, double ki, double kd);
    void setPidCoefficients(double kp, double ki, double kd);
    void setTargetTemperature(double target);
    void controlHeating();
    void setHeaterEnabled(bool enabled);
    bool isHeaterEnabled() const;
    double getCurrentTemperature() const;

    void onTemperatureChange(TemperatureChangeCallback callback);
    
private:
    void updateCurrentTemperature();
    void updatePIDState();
    void setPWM(uint16_t value);

    uint8_t pwmPin;
    uint32_t readTemperatureInterval = 200;
    uint32_t lastReadTime = 0;
    double currentTemperature = 0.0;
    double targetTemperature = 0.0;
    double output = 0.0;
    bool heaterEnabled = true;

    TemperatureChangeCallback temperatureChangeCallback;

    MAX6675 thermoCouple;
    ArduPID pidController;
};

#endif // HEATERELEMENT_HPP