#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Versatile_RotaryEncoder.h>
#include "UserInterface.hpp"
#include "HeaterElement.hpp"

// Pin Definitions
#define DISPLAY_CS_PIN 0
#define DISPLAY_DC_PIN 1
#define DISPLAY_RST_PIN 2
#define ENCODER_CLK_PIN 20
#define ENCODER_DT_PIN 21
#define ENCODER_SW_PIN 10
#define THERMO_COUPLE_CS 3
#define PWM_PIN 8

// Initialize components
U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI display(U8G2_R0, DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN);
Versatile_RotaryEncoder encoder(ENCODER_CLK_PIN, ENCODER_DT_PIN, ENCODER_SW_PIN);
HeaterElement heaterElement(THERMO_COUPLE_CS, PWM_PIN);
UserInterface &ui = UserInterface::getInstance();

void setup()
{
  SPI.begin();

  display.begin();
  heaterElement.begin(2.0, 5.0, 1.0); // Example PID coefficients
  ui.begin(display, encoder, heaterElement);

  xTaskCreate([](void *pvParameters)
              {
                    for (;;)
                    {
                        heaterElement.controlHeating();
                        vTaskDelay(pdMS_TO_TICKS(200)); // Delay for 200 ms
                    } },
              "TempTask", 4096, NULL, 1, NULL);
}

void loop()
{
  ui.update();
}