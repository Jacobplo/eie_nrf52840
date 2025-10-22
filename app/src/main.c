/*
 * main.c
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1

int main(void) {
  if (BTN_init() < 0) {
    return 0;
  }

  if (LED_init() < 0) {
    return 0;
  }

  int value = 0;
  while (1) {
    if (BTN_check_clear_pressed(BTN0)) {
      value++;
    }
    if (value == 16) {
      value = 0;
    }
    
    // First digit
    if (value & 0b0001) {
      LED_set(LED3, LED_ON);
    }
    else {
      LED_set(LED3, LED_OFF);
    }

    // Second digit
    if (value & 0b0010) {
      LED_set(LED2, LED_ON);
    }
    else {
      LED_set(LED2, LED_OFF);
    }

    // Third digit
    if (value & 0b0100) {
      LED_set(LED1, LED_ON);
    }
    else {
      LED_set(LED1, LED_OFF);
    }

    // Fourth digit
    if (value & 0b1000) {
      LED_set(LED0, LED_ON);
    }
    else {
      LED_set(LED0, LED_OFF);
    }

    k_msleep(SLEEP_MS);
  }
	return 0;
}
