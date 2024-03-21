#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include "u8g2_graph.h"

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 8, /* data=*/ 10, /* select=*/ 7);

void setup() {
    u8g2.begin();

    // Initialize the U8G2_Graph instance
    U8G2_Graph graph(u8g2, 0, 64, 128, 64, 10, 6, 1, 100, u8g2_font_blipfest_07_tn);

    u8g2.firstPage();
    do {
        graph.drawAxes();
    } while (u8g2.nextPage());
}

void loop() {
}