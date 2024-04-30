#include "mui_u8g2_extensions.h"

#define MUI_U8G2_V_PADDING 1

uint8_t mui_u8g2_ex_draw_temp(mui_t *ui, uint8_t msg)
{
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  char text[30]; // Adjust the size as per your requirement

  switch (msg)
  {
  case MUIF_MSG_DRAW:
    sprintf(text, "%s %d", ui->text, *value);

    u8g2_t *u8g2 = mui_get_U8g2(ui);
    u8g2_SetFontDirection(u8g2, 1);
    u8g2_DrawUTF8(mui_get_U8g2(ui), mui_get_x(ui), mui_get_y(ui), text);

    u8g2_SetFontDirection(u8g2, 0);
    break;
  case MUIF_MSG_FORM_START:
    break;
  case MUIF_MSG_FORM_END:
    break;
  case MUIF_MSG_CURSOR_ENTER:
    break;
  case MUIF_MSG_CURSOR_SELECT:
    break;
  case MUIF_MSG_VALUE_INCREMENT:
    break;
  case MUIF_MSG_VALUE_DECREMENT:
    break;
  case MUIF_MSG_CURSOR_LEAVE:
    break;
  case MUIF_MSG_TOUCH_DOWN:
    break;
  case MUIF_MSG_TOUCH_UP:
    break;
  }
  return 0;
}

void mui_u8g2_ex_draw_button_utf(mui_t *ui, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text)
{
  if (text == NULL)
    text = "";

  u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t text_width = u8g2_GetStrWidth(u8g2, text);

  if (text_width > width)
  {
    u8g2_uint_t max_chars = 0;
    u8g2_uint_t current_width = 0;
    const char *ptr = text;

    while (*ptr != '\0' && current_width <= width)
    {
      current_width += u8g2_GetGlyphWidth(u8g2, *ptr);
      if (current_width <= width)
        max_chars++;
      ptr++;
    }

    char clipped_text[max_chars + 1];
    strncpy(clipped_text, text, max_chars);
    clipped_text[max_chars] = '\0';

    u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), flags, width, padding_h, padding_v, clipped_text);
  }
  else
  {
    u8g2_DrawButtonUTF8(mui_get_U8g2(ui), mui_get_x(ui), mui_get_y(ui), flags, width, padding_h, padding_v, text);
  }
}

void mui_u8g2_ex_draw_button_pi(mui_t *ui, u8g2_uint_t width, u8g2_uint_t padding_h, const char *text)
{
  mui_u8g2_ex_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), width, padding_h, MUI_U8G2_V_PADDING, text);
}

uint8_t mui_u8g2_ex_u8_opt_line_wa_mud_pi(mui_t *ui, uint8_t msg)
{
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  switch (msg)
  {
  case MUIF_MSG_DRAW:
    if (mui_fds_get_nth_token(ui, *value) == 0)
    {
      *value = 0;
      mui_fds_get_nth_token(ui, *value);
    }
    mui_u8g2_ex_draw_button_pi(ui, ui->arg, 1, ui->text);

    break;
  case MUIF_MSG_FORM_START:
    break;
  case MUIF_MSG_FORM_END:
    break;
  case MUIF_MSG_CURSOR_ENTER:
    break;
  case MUIF_MSG_CURSOR_SELECT:
  case MUIF_MSG_VALUE_INCREMENT:
  case MUIF_MSG_VALUE_DECREMENT:
    /* toggle between normal mode and capture next/prev mode */
    ui->is_mud = !ui->is_mud;
    break;
  case MUIF_MSG_CURSOR_LEAVE:
    break;
  case MUIF_MSG_TOUCH_DOWN:
    break;
  case MUIF_MSG_TOUCH_UP:
    break;
  case MUIF_MSG_EVENT_NEXT:
    if (ui->is_mud)
    {
      (*value)++;
      if (mui_fds_get_nth_token(ui, *value) == 0)
        *value = 0;
      return 1;
    }
    break;
  case MUIF_MSG_EVENT_PREV:
    if (ui->is_mud)
    {
      if (*value == 0)
        *value = mui_fds_get_token_cnt(ui);
      (*value)--;
      return 1;
    }
    break;
  }
  return 0;
}