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
        uint8_t status = thermoCouple.read();

        if (status != STATUS_OK)
        {
            handleThermocoupleError(status);
            errorCount++;

            if (errorCount >= maxErrorCount)
            {
                Serial.println("Too many errors, disabling heater.");
                setHeaterEnabled(false);
            }
            return;
        }

        // Reset error count on successful read
        errorCount = 0;

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

void HeaterElement::handleThermocoupleError(uint8_t status) const
{
    switch (status)
    {
        case 0:
            // No error, everything is OK
            break;
        case STATUS_ERROR:
            Serial.println("Error: Thermocouple short to VCC. Please check the wiring.");
            break;
        case STATUS_NOREAD:
            Serial.println("Error: No temperature read done yet. Please check the wiring.");
            break;
        case STATUS_NO_COMMUNICATION:
            Serial.println("Error: No communication with thermocouple. Please check the wiring.");
            break;
        default:
            Serial.println("Unknown thermocouple error.");
            break;
    }
}