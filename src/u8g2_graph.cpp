#include "U8G2_Graph.h"

U8G2_Graph::U8G2_Graph(U8G2 &u8g2, int x, int y, int w, int h, uint8_t num_x_ticks, uint8_t num_y_ticks, uint16_t x_tick_step, uint16_t y_tick_step, const uint8_t *font)
    : u8g2(u8g2), x(x), y(y), w(w), h(h), num_x_ticks(num_x_ticks), num_y_ticks(num_y_ticks), x_tick_step(x_tick_step), y_tick_step(y_tick_step), font(font) {}

void U8G2_Graph::drawAxes()
{
    const int tick_length = 3;

    u8g2.setFont(font);

    // Calculate the maximum width for y-axis values
    uint16_t max_y_value = num_y_ticks * y_tick_step;
    char maxNumberString[6];                     // Assuming a maximum of 5 characters for a uint16_t value
    sprintf(maxNumberString, "%u", max_y_value); // Convert to string

    uint8_t left_margin = u8g2.getStrWidth(maxNumberString) + 1;
    uint8_t bottom_margin = u8g2.getAscent() + 1;

    // Draw x-axis and y-axis
    u8g2.drawHVLine(x + left_margin, y - bottom_margin - 1, w - left_margin, 0);
    u8g2.drawHVLine(x + left_margin, y - bottom_margin - 1, h - bottom_margin, 3);

    // Calculate the pixel step size for each tick
    float x_ticks_pixel_step = static_cast<float>(w - left_margin) / num_x_ticks;
    float y_ticks_pixel_step = static_cast<float>(h - bottom_margin) / num_y_ticks;

    // Draw x-axis ticks and labels
    for (int i = 1; i <= num_x_ticks; ++i)
    {
        int x_position = x + left_margin + i * x_ticks_pixel_step;
        u8g2.drawHVLine(x_position, y - bottom_margin + 1, tick_length, 1);

        char tickLabel[6];                         // Assuming a maximum of 5 characters for a uint16_t value
        sprintf(tickLabel, "%u", i * x_tick_step); // Convert to string

        // Print the label in the left of the tick
        u8g2.setCursor(x_position - u8g2.getStrWidth(tickLabel) - 1, y);
        u8g2.print(tickLabel);
    }

    // Draw y-axis ticks and labels
    for (int i = 1; i <= num_y_ticks; ++i)
    {
        int y_position = y - bottom_margin - i * y_ticks_pixel_step;
        u8g2.drawHVLine(x + left_margin - 1, y_position, tick_length, 2);

        // Print the label in the bottom of the tick
        u8g2.setCursor(x, y_position + u8g2.getAscent() + 2);
        u8g2.print(i * y_tick_step);
    }
}