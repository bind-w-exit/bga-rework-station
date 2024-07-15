#include "HeaterElement.hpp"

HeaterElement::HeaterElement(uint8_t thermoCoupleCsPin, uint8_t pwmPin)
    : thermoCouple(thermoCoupleCsPin, &SPI), pidController(), pwmPin(pwmPin)
{
}

void HeaterElement::begin(double kp, double ki, double kd)
{
    pinMode(pwmPin, OUTPUT);

    thermoCouple.begin();
    pidController.begin(&currentTemperature, &output, &targetTemperature, kp, ki, kd);
    pidController.setOutputLimits(0, 255);
}

void HeaterElement::setTargetTemperature(double target)
{
    targetTemperature = target;
}

void HeaterElement::setPidCoefficients(double kp, double ki, double kd)
{
    pidController.setCoefficients(kp, ki, kd);
}

void HeaterElement::controlHeating()
{
    updateCurrentTemperature();

    if (heaterEnabled)
    {
        updatePIDState();
        setPWM(static_cast<uint16_t>(output));
    }
}

void HeaterElement::updateCurrentTemperature()
{
    uint32_t currentTime = millis();
    if (currentTime - lastReadTime >= readTemperatureInterval)
    {
        lastReadTime = currentTime;
        thermoCouple.read();

        double newTemperature = thermoCouple.getTemperature();
        if (newTemperature != currentTemperature)
        {
            currentTemperature = newTemperature;
            if (temperatureChangeCallback)
            {
                temperatureChangeCallback(currentTemperature);
            }
        }
    }
}

void HeaterElement::updatePIDState()
{
    pidController.compute();
}

void HeaterElement::setPWM(uint16_t value)
{
    analogWrite(pwmPin, value);
}

void HeaterElement::setHeaterEnabled(bool enabled)
{
    heaterEnabled = enabled;

    if (heaterEnabled)
    {
        pidController.start();
    }
    else
    {
        pidController.reset();
        pidController.stop();
        setPWM(0);
    }
}

bool HeaterElement::isHeaterEnabled() const
{
    return heaterEnabled;
}

double HeaterElement::getCurrentTemperature() const
{
    return currentTemperature;
}

void HeaterElement::onTemperatureChange(TemperatureChangeCallback callback)
{
    temperatureChangeCallback = callback;
}
