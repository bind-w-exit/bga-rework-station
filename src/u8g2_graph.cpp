#include "U8G2_Graph.h"

void U8G2_Graph::drawAxes(U8G2 &u8g2)
{
    const int tick_length = 3;

    u8g2.setFont(font);

    // Calculate the maximum width for y-axis values
    uint16_t max_y_value = numYTicks * yTickStep;
    char maxNumberString[6];                     // Assuming a maximum of 5 characters for a uint16_t value
    sprintf(maxNumberString, "%u", max_y_value); // Convert to string

    uint8_t left_margin = u8g2.getStrWidth(maxNumberString) + 1;
    uint8_t bottom_margin = u8g2.getAscent() + 1;

    // Draw x-axis and y-axis
    u8g2.drawHVLine(x + left_margin, y - bottom_margin - 1, width - left_margin, 0);
    u8g2.drawHVLine(x + left_margin, y - bottom_margin - 1, height - bottom_margin, 3);

    // Calculate the pixel step size for each tick
    float x_ticks_pixel_step = static_cast<float>(width - left_margin) / numXTicks;
    float y_ticks_pixel_step = static_cast<float>(height - bottom_margin) / numYTicks;

    // Draw x-axis ticks and labels
    for (int i = 1; i <= numXTicks; ++i)
    {
        int x_position = x + left_margin + i * x_ticks_pixel_step;
        u8g2.drawHVLine(x_position, y - bottom_margin + 1, tick_length, 1);

        char tickLabel[6];                       // Assuming a maximum of 5 characters for a uint16_t value
        sprintf(tickLabel, "%u", i * xTickStep); // Convert to string

        // Print the label in the left of the tick
        u8g2.setCursor(x_position - u8g2.getStrWidth(tickLabel) - 1, y);
        u8g2.print(tickLabel);
    }

    // Draw y-axis ticks and labels
    for (int i = 1; i <= numYTicks; ++i)
    {
        int y_position = y - bottom_margin - i * y_ticks_pixel_step;
        u8g2.drawHVLine(x + left_margin - 1, y_position, tick_length, 2);

        // Print the label in the bottom of the tick
        u8g2.setCursor(x, y_position + u8g2.getAscent() + 2);
        u8g2.print(i * yTickStep);
    }
}

void U8G2_Graph::drawDataLines(U8G2 &u8g2)
{
}

void U8G2_Graph::draw(U8G2 &u8g2)
{
    drawAxes(u8g2);
    drawDataLines(u8g2);
}