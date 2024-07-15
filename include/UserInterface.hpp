#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <Arduino.h>
#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <Versatile_RotaryEncoder.h>
#include "HeaterElement.hpp"
#include "mui_u8g2_extensions.h"

class UserInterface
{
public:
    static UserInterface &getInstance();
    void begin(U8G2 &display, Versatile_RotaryEncoder &encoder, HeaterElement &heaterElement);
    void update();

private:
    UserInterface();
    UserInterface(const UserInterface &) = delete;            // Disallows copy constructor to avoid unintended object copies?
    UserInterface &operator=(const UserInterface &) = delete; // Disallows copy assignment to prevent unintended object assignments?

    static void handleEncoderRotation(int8_t rotation);
    static void handleEncoderPressRelease();
    static void handleEncoderLongPressRelease();
    static void onTemperatureChanged(double temperature);

    void setTargetTemperature();
    void drawUI();

    // UI variables
    static bool isNeedsRedraw;
    static bool isHeaterEnabled;
    static uint8_t temperatureSetpointOne;
    static uint8_t temperatureSetpointTwo;
    static uint8_t temperatureSetpointThree;
    static double currentTemperature;

    U8G2 *display = nullptr;
    Versatile_RotaryEncoder *encoder = nullptr;
    HeaterElement *heaterElement = nullptr;
    MUIU8G2 mui;

    // Forms Definition
    static constexpr fds_t formData[] =
        MUI_FORM(2)                                                                                                                 // Main menu
        MUI_STYLE(0) MUI_DATA("GP", MUI_3 "Profile Management|" MUI_5 "Manual work|" MUI_4 "Temperature Calibration|" MUI_5 "Exit") // Menu
        MUI_XYA("GC", 5, 13, 0) MUI_XYA("GC", 5, 25, 1) MUI_XYA("GC", 5, 37, 2) MUI_XYA("GC", 5, 49, 3) MUI_XYA("GC", 5, 61, 4)     // Menu buttons

        MUI_FORM(3)                                                                                                                                             // Profiles menu
        MUI_STYLE(0) MUI_DATA("GP", MUI_10 "View Profiles|" MUI_20 "Create Profile|" MUI_30 "Edit Profile|" MUI_40 "Delete Profile|" MUI_2 "Back to Main Menu") // Menu
        MUI_XYA("GC", 5, 13, 0) MUI_XYA("GC", 5, 25, 1) MUI_XYA("GC", 5, 37, 2) MUI_XYA("GC", 5, 49, 3) MUI_XYA("GC", 5, 61, 4)                                 // Menu buttons

        MUI_FORM(4)                                     // Calibration menu
        MUI_STYLE(0)                                    // Style
        MUI_LABEL(18, 10, "Calibration menu")           // Label
        MUI_XY("HR", 0, 15)                             // Horizontal line
        MUI_LABEL(5, 30, "Adjust temp: ")               // Label
        MUI_XY("NR", 64, 30)                            // Numeric input field
        MUI_LABEL(5, 40, "PID P: ")                     // Label
        MUI_XY("NR", 64, 40)                            // Numeric input field
        MUI_LABEL(5, 50, "PID I: ")                     // Label
        MUI_XY("NR", 64, 50)                            // Numeric input field
        MUI_LABEL(5, 58, "PID D: ")                     // Label
        MUI_XY("NR", 64, 60)                            // Numeric input field
        MUI_STYLE(1) MUI_XYAT("GB", 7, 12, 2, "\u0075") // Button

        MUI_FORM(5)                                      // Manual work
        MUI_STYLE(0) MUI_LABEL(18, 10, "Manual work:")   // Label
        MUI_LABEL(1, 25, "Current: ")                    // Label
        MUI_XY("CT", 60, 25)                             // Current temperature field
        MUI_LABEL(1, 35, "Set: ")                        // Label
        MUI_XYAT("N1", 60, 35, 16, "0-5")                // 1st digit of temperature setpoint
        MUI_XYAT("N2", 68, 35, 16, "0-9")                // 2nd digit of temperature setpoint
        MUI_XYAT("N3", 76, 35, 16, "0-9")                // 3rd digit of temperature setpoint
        MUI_LABEL(1, 45, "Enable heater: ")              // Label
        MUI_XY("CB", 60, 45)                             // Checkbox
        MUI_XY("CD", 70, 45)                             // Checkbox
        MUI_LABEL(1, 55, "Status: ")                     // Label
        MUI_XY("TS", 60, 55)                             // Status field
        MUI_STYLE(1) MUI_XYAT("GB", 7, 12, 2, "\u0075"); // Button

    // Menu Items
    static constexpr muif_t menuItems[] = {
        MUIF_U8G2_FONT_STYLE(0, u8g2_font_new3x9pixelfont_tf),            // Normal text style
        MUIF_U8G2_FONT_STYLE(1, u8g2_font_open_iconic_all_1x_t),          // Icons font
        MUIF_U8G2_FONT_STYLE(3, u8g2_font_blipfest_07_tr),                // Square text style
        MUIF_BUTTON("GB", mui_u8g2_btn_goto_wm_fi),                       // Goto buttons
        MUIF("CT", 0, &currentTemperature, mui_u8g2_ex_double_draw_temp), // Current temperature field
        MUIF_RO("GP", mui_u8g2_goto_data),                                // Scrollable jump table
        MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),                      // Menu buttons
        MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pf),                      // Menu buttons
        MUIF_U8G2_LABEL(),
        MUIF_RO("HR", mui_hrule),
        MUIF_VARIABLE("CB", &isHeaterEnabled, mui_u8g2_u8_chkbox_wm_pi),                             // Heater enable checkbox
        MUIF_U8G2_U8_MIN_MAX("N1", &temperatureSetpointOne, 0, 5, mui_u8g2_u8_min_max_wm_mud_pi),    // 1st digit of frequency
        MUIF_U8G2_U8_MIN_MAX("N2", &temperatureSetpointTwo, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),    // 2nd digit of frequency
        MUIF_U8G2_U8_MIN_MAX("N3", &temperatureSetpointThree, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi)}; // 3rd digit of frequency
};

#endif // USERINTERFACE_HPP