#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <Versatile_RotaryEncoder.h>
#include "u8g2_graph.h"
#include "mui_u8g2_extensions.h"

// Pin Definitions
#define ENCODER_CLK_PIN 5
#define ENCODER_DT_PIN 6
#define ENCODER_SW_PIN 7

// Display initialization
U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/4, /* data=*/3, /* cs=*/0, /* dc=*/1, /* reset=*/2);

MUIU8G2 mui;
U8G2_Graph graph;

// Encoder initialization
Versatile_RotaryEncoder encoder(ENCODER_CLK_PIN, ENCODER_DT_PIN, ENCODER_SW_PIN);

// Variables
uint8_t isRedraw = 1;
uint8_t rotateEvent = 0;    // 0 = not turning, 1 = CW, 2 = CCW
uint8_t pressEvent = 0;     // 0 = not pushed, 1 = pushed
uint8_t longPressEvent = 0; // 0 = not pushed, 1 = pushed

uint8_t bgaProfile = 0;

uint8_t bottomTemp = 205;
uint8_t topTemp = 350;

// Callback function for drawing graph
uint8_t drawGraph(mui_t *ui, uint8_t msg)
{
  if (msg == MUIF_MSG_DRAW)
  {
    graph.setX(mui_get_x(ui));
    graph.setY(mui_get_y(ui));
    graph.setHeight(52);
    graph.setWidth(100);
    graph.setNumXTicks(6);
    graph.setNumYTicks(5);
    graph.draw(u8g2);
  }
  return 0;
}

// Menu Items
muif_t menuItems[] = {
    MUIF_U8G2_FONT_STYLE(0, u8g2_font_new3x9pixelfont_tf),               // Normal text style
    MUIF_U8G2_FONT_STYLE(1, u8g2_font_open_iconic_all_1x_t),             // Icons font
    MUIF_U8G2_FONT_STYLE(3, u8g2_font_blipfest_07_tr),                   // Squarew let
    MUIF_RO("GH", drawGraph),                                            // Custom MUIF callback to draw graph
    MUIF_BUTTON("ST", mui_u8g2_btn_goto_wm_fi),                          // Start button
    MUIF_BUTTON("SP", mui_u8g2_btn_goto_wm_fi),                          // Stop button
    MUIF_BUTTON("SS", mui_u8g2_btn_goto_wm_fi),                          // Settings button
    MUIF_VARIABLE("BP", &bgaProfile, mui_u8g2_ex_u8_opt_line_wa_mud_pi), // BGA profile selection
    MUIF("BT", 0, &bottomTemp, mui_u8g2_ex_draw_temp),                   // Bottom temp
    MUIF("TT", 0, &topTemp, mui_u8g2_ex_draw_temp)                       // Top temp
};

// Main Form Definition
fds_t formData[] = MUI_FORM(1)
    MUI_STYLE(1) MUI_XYT("ST", 6, 62, "\u00D3") MUI_XYT("SW", 64, 32, "ABABAHALAMAHA")
        MUI_STYLE(0) MUI_XYAT("BP", 32, 10, 80, "Lead-Free Profile (SnAgCu)|Lead-Based Profile (SnPb)|Mixed Alloy Profile (e.g., SnAgCu with Bi)|Custom Profile (Variable Alloy Ratios)|Low-Temperature Profile (SnBiAg)|High-Temperature Profile (SnAgCu with Increased TAL)|Extended Soak Profile (SnPb with Longer Soak Time)|Fine Pitch Profile (SnAgCu with Reduced TAL)|Custom Profile 1|Cus")
            MUI_STYLE(1) MUI_XYT("SS", 122, 62, "\u0081") MUI_XY("GH", 12, 64) MUI_STYLE(3) MUI_XYT("BT", 0, 0, "BT:") MUI_XYT("TT", 120, 0, "TT:");

// Function Prototypes
void handleRotate(int8_t rotation);
void handlePressRelease();
void handleLongPressRelease();
void handleEvents();

void setup()
{
  encoder.setHandleRotate(handleRotate);
  encoder.setHandlePressRelease(handlePressRelease);
  encoder.setHandleLongPressRelease(handleLongPressRelease);

  u8g2.begin();
  mui.begin(u8g2, formData, menuItems, sizeof(menuItems) / sizeof(muif_t));
  mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
}

void loop()
{
  if (mui.isFormActive())
  {
    if (isRedraw)
    {
      u8g2.firstPage();
      do
      {
        encoder.ReadEncoder(); // Read and process encoder
        mui.draw();
        encoder.ReadEncoder();
      } while (u8g2.nextPage());
      isRedraw = 0;
    }
    encoder.ReadEncoder();
    handleEvents();
  }
  else
  {
    /* The menu should never become inactive, but if so, then restart the menu system */
    mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
  }
}

void handleEvents()
{
  if (pressEvent == 1)
  {
    mui.sendSelect();
    isRedraw = 1;
    pressEvent = 0;
  }
  if (longPressEvent == 1)
  {
    mui.sendSelectWithExecuteOnSelectFieldSearch();
    isRedraw = 1;
    longPressEvent = 0;
  }
  if (rotateEvent == 1)
  {
    mui.nextField();
    isRedraw = 1;
    rotateEvent = 0;
  }
  if (rotateEvent == 2)
  {
    mui.prevField();
    isRedraw = 1;
    rotateEvent = 0;
  }
}

void handleRotate(int8_t rotation)
{
  if (rotation > 0)
    rotateEvent = 2; // CW
  else
    rotateEvent = 1; // CCW
}

void handlePressRelease()
{
  pressEvent = 1;
}

void handleLongPressRelease()
{
  longPressEvent = 1;
}