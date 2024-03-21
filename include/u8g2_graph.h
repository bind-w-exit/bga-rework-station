#ifndef U8G2_GRAPH_H
#define U8G2_GRAPH_H

#include <U8g2lib.h>

class U8G2_Graph
{
public:
    U8G2_Graph(U8G2 &u8g2, int x, int y, int w, int h, uint8_t num_x_ticks, uint8_t num_y_ticks, uint16_t x_tick_step, uint16_t y_tick_step, const uint8_t *font);
    void drawAxes();

private:
    int x, y;                          // graph location (lower left)
    int w, h;                          // width and height of graph
    uint8_t num_x_ticks, num_y_ticks;  // number of ticks for the x and y axes;
    uint16_t x_tick_step, y_tick_step; // step size for the x and y axes
    const uint8_t *font;

    U8G2 &u8g2; // Reference to the U8G2 display object
};

#endif