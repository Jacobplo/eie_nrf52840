#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)


static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);


int main(void) {
  int ret;

  // Activate all LED pins.
  struct gpio_dt_spec const static pins[] = {led0, led1, led2, led3};
  size_t size_pins = sizeof(pins) / sizeof(pins[0]);
  for(int8_t i = 0; i < size_pins; i++) {
    if(!gpio_is_ready_dt(&pins[i])) return -1;

    ret = gpio_pin_configure_dt(&pins[i], GPIO_OUTPUT_ACTIVE);
    if(ret < 0) {
      return ret;
    }
  }
 

  while (1) {
    gpio_pin_toggle_dt(&led0);
    k_msleep(100);
    gpio_pin_toggle_dt(&led1);
    k_msleep(100);
    gpio_pin_toggle_dt(&led2);
    k_msleep(100);
    gpio_pin_toggle_dt(&led3);
    k_msleep(100);
  }
}
