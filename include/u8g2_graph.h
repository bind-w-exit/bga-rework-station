#ifndef U8G2_GRAPH_H
#define U8G2_GRAPH_H

#include <U8g2lib.h>

class U8G2_Graph
{
public:
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    uint8_t getNumXTicks() const { return numXTicks; }
    uint8_t getNumYTicks() const { return numYTicks; }
    uint16_t getXTickStep() const { return xTickStep; }
    uint16_t getYTickStep() const { return yTickStep; }
    const uint8_t *getFont() const { return font; }

    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    void setWidth(int newWidth) { width = newWidth; }
    void setHeight(int newHeight) { height = newHeight; }
    void setNumXTicks(uint8_t newNumXTicks) { numXTicks = newNumXTicks; }
    void setNumYTicks(uint8_t newNumYTicks) { numYTicks = newNumYTicks; }
    void setXTickStep(uint16_t newXTickStep) { xTickStep = newXTickStep; }
    void setYTickStep(uint16_t newYTickStep) { yTickStep = newYTickStep; }
    void setFont(const uint8_t *newFont) { font = newFont; }

    void draw(U8G2 &u8g2);

private:
    int x = 0, y = 64;                              // Graph location (lower left)
    int width = 128, height = 64;                   // Width and height of graph
    uint8_t numXTicks = 10, numYTicks = 5;          // Number of ticks for the x and y axes;
    uint16_t xTickStep = 10, yTickStep = 100;       // Step size for the x and y axes
    const uint8_t *font = u8g2_font_blipfest_07_tn; // Default font

    void drawAxes(U8G2 &u8g2);
    void drawDataLines(U8G2 &u8g2);
};

#endif