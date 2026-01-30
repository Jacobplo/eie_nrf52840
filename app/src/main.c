/**
 * @file main.c
 */

#include <inttypes.h>

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/display.h>
#include <zephyr/device.h>

#include "BTN.h"
#include "LED.h"
#include "core/lv_obj_pos.h"
#include "display/lv_display.h"
#include "lv_data_obj.h"
#include "misc/lv_area.h"
#include "misc/lv_color.h"
#include "misc/lv_timer.h"
#include "widgets/button/lv_button.h"
#include "widgets/label/lv_label.h"
#include "core/lv_obj_style_gen.h"

#define SLEEP_MS 1


static const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
static lv_obj_t *screen = NULL;



int main(void) {
  /*
  * Init
  */
  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }
  if(!device_is_ready(display_dev)) {
    return 0;
  }
  screen = lv_screen_active();
  if(screen == NULL) {
    return 0;
  }

  /*
  * Setup
  */
  // Text
  lv_obj_t *label = lv_label_create(screen);
  lv_label_set_text(label, "Cheese wheel");
  lv_obj_set_style_text_color(screen, lv_color_hex(0x0000ff), LV_PART_MAIN);
 
  // Buttons
  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    lv_obj_t *ui_btn = lv_button_create(screen);
    lv_obj_align(ui_btn,LV_ALIGN_CENTER, 50 * (i % 2 ? 1 : -1), 20 * (i < 2 ? -1 : 1));
    lv_obj_t *button_label = lv_label_create(ui_btn);
    char label_text[10];
    snprintf(label_text, 10, "LED %d", i);
    lv_label_set_text(button_label, label_text);
    lv_obj_align(button_label, LV_ALIGN_CENTER, 0, 0);
  }

  


  /*
  * Main
  */
  display_blanking_off(display_dev);
  while (1) {
    lv_timer_handler();
    k_msleep(SLEEP_MS);
  }
  return 0;
}
