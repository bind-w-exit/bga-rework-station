#include "UserInterface.hpp"

constexpr fds_t UserInterface::formData[];
constexpr muif_t UserInterface::menuItems[];

// Define static member variables
bool UserInterface::isNeedsRedraw = false;
bool UserInterface::isHeaterEnabled = false;
uint8_t UserInterface::temperatureSetpointOne = 0;
uint8_t UserInterface::temperatureSetpointTwo = 0;
uint8_t UserInterface::temperatureSetpointThree = 0;
double UserInterface::currentTemperature = 0.0;

UserInterface::UserInterface()
{
}

UserInterface &UserInterface::getInstance()
{
    static UserInterface instance;
    return instance;
}

void UserInterface::begin(U8G2 &display, Versatile_RotaryEncoder &encoder, HeaterElement &heaterElement)
{
    UserInterface::display = &display;
    UserInterface::encoder = &encoder;
    UserInterface::heaterElement = &heaterElement;

    mui.begin(display, formData, menuItems, sizeof(menuItems) / sizeof(muif_t));
    mui.gotoForm(5, 0);

    encoder.setHandleRotate(handleEncoderRotation);
    encoder.setHandlePressRelease(handleEncoderPressRelease);
    encoder.setHandleLongPressRelease(handleEncoderLongPressRelease);

    heaterElement.onTemperatureChange(onTemperatureChanged);
}

void UserInterface::update()
{
    if (mui.isFormActive())
    {
        if (isNeedsRedraw)
        {
            drawUI();
            isNeedsRedraw = false;
        }

        isNeedsRedraw = encoder->ReadEncoder(); // Read and process encoder

        setTargetTemperature();
    }
    else
    {
        mui.gotoForm(5, 0); // Restart the menu system if it becomes inactive
    }
}

void UserInterface::handleEncoderRotation(int8_t rotation)
{
    UserInterface &ui = getInstance();
    if (rotation > 0)
    {
        ui.mui.prevField();
    }
    else
    {
        ui.mui.nextField();
    }
}

void UserInterface::handleEncoderPressRelease()
{
    UserInterface &ui = getInstance();
    ui.mui.sendSelect();
}

void UserInterface::handleEncoderLongPressRelease()
{
    UserInterface &ui = getInstance();
    ui.mui.sendSelectWithExecuteOnSelectFieldSearch();
}

void UserInterface::setTargetTemperature()
{
    double target = temperatureSetpointOne * 100 +
                    temperatureSetpointTwo * 10 +
                    temperatureSetpointThree;

    heaterElement->setTargetTemperature(target);
    heaterElement->setHeaterEnabled(isHeaterEnabled);
}

void UserInterface::onTemperatureChanged(double temperature)
{
    currentTemperature = temperature;
    isNeedsRedraw = true;
}

void UserInterface::drawUI()
{
    display->firstPage();
    do
    {
        mui.draw();
    } while (display->nextPage());
}