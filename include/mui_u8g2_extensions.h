#ifndef MUI_U8G2_EXTENSIONS_H
#define MUI_U8G2_EXTENSIONS_H

#include "clib/mui_u8g2.h"

/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
extern "C" {
#endif

void mui_u8g2_ex_draw_button_utf(mui_t *ui, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text);
void mui_u8g2_ex_draw_button_pi(mui_t *ui, u8g2_uint_t width, u8g2_uint_t padding_h, const char *text);

uint8_t mui_u8g2_ex_u8_opt_line_wa_mud_pi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_ex_double_draw_temp(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_ex_double_draw_temp_angle(mui_t *ui, uint8_t msg);

uint8_t mui_hrule(mui_t *ui, uint8_t msg);

#ifdef __cplusplus
}
#endif

#endif /* MUI_U8G2_EXTENSIONS_H */