/*
 * main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"


#define SLEEP_MS 1

#define ARDUINO_SPI_NODE DT_NODELABEL(arduino_spi)
#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)

static const struct gpio_dt_spec dcx_gpio = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, dcx_gpios);
//static const struct spi_cs_control cs_ctrl = (struct spi_cs_control){
//  .gpio = GPIO_DT_SPEC_GET(ARDUINO_SPI_NODE, cs_gpios),
//  .delay = 0u,
//};


int main(void) {
  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  if(!gpio_is_ready_dt(&dcx_gpio)) {
    return 0;
  }
  if(gpio_pin_configure_dt(&dcx_gpio, GPIO_OUTPUT_LOW)) {
    return 0;
  }



  while(1) {
    k_msleep(SLEEP_MS);
  }
	return 0;
}
