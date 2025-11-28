/*
 * main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/uuid.h>

#include "BTN.h"
#include "LED.h"
#include "zephyr/bluetooth/att.h"
#include "zephyr/bluetooth/gap.h"

#define SLEEP_MS 1


// BLE
#define BT_UUID_SERVICE BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0)

#define BT_UUID_CHARACTERISTIC BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1)
#define BT_UUID_CHARACTERISTIC_TWO BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x5678abcdef2)

#define BT_CUSTOM_CHARACTERISTIC_MAX_DATA_LENGTH 20

static const char name[] = CONFIG_BT_DEVICE_NAME;
static const struct bt_data ble_advertising_data[] = {
  BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
  BT_DATA(BT_DATA_NAME_COMPLETE, name, sizeof(name)),
};

static uint8_t ble_custom_characteristic_user_data[BT_CUSTOM_CHARACTERISTIC_MAX_DATA_LENGTH] = {};
static uint8_t ble_characteristic_two_user_data[BT_CUSTOM_CHARACTERISTIC_MAX_DATA_LENGTH] = {};

static const struct bt_uuid_128 ble_custom_service_uuid = BT_UUID_INIT_128(BT_UUID_SERVICE);

static const struct bt_uuid_128 ble_custom_characteristic_uuid = BT_UUID_INIT_128(BT_UUID_CHARACTERISTIC);
static const struct bt_uuid_128 ble_characteristic_two_uuid = BT_UUID_INIT_128(BT_UUID_CHARACTERISTIC_TWO);



static ssize_t ble_custom_characteristic_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset) {
  const char *value = attr->user_data;
  return bt_gatt_attr_read(conn, attr, buf, len, offset, value, strlen(value));
}
static ssize_t ble_characteristic_two_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset) {
  const char *value = attr->user_data;
  return bt_gatt_attr_read(conn, attr, buf, len, offset, value, strlen(value));
}

static ssize_t ble_custom_characteristic_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags) {
  char *value = attr->user_data;

  if(offset + len > BT_CUSTOM_CHARACTERISTIC_MAX_DATA_LENGTH) {
    return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
  }

  memcpy(value + offset, buf, len);
  value[offset + len] = 0;

  return len;
}
static ssize_t ble_characteristic_two_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags) {
  char *value = attr->user_data;

  if(offset + len > BT_CUSTOM_CHARACTERISTIC_MAX_DATA_LENGTH) {
    return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
  }

  memcpy(value + offset, buf, len);
  value[offset + len] = 0;

  return len;
}

BT_GATT_SERVICE_DEFINE(
  ble_custom_service,
  BT_GATT_PRIMARY_SERVICE(&ble_custom_service_uuid),
  BT_GATT_CHARACTERISTIC(
    &ble_custom_characteristic_uuid.uuid,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    ble_custom_characteristic_read_cb,
    ble_custom_characteristic_write_cb,
    ble_custom_characteristic_user_data
  ),
  BT_GATT_CHARACTERISTIC(
    &ble_characteristic_two_uuid.uuid,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    ble_characteristic_two_read_cb,
    ble_characteristic_two_write_cb,
    ble_custom_characteristic_user_data
  )
);

int main(void) {

  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  int ret = bt_enable(NULL);
  if(ret) {
    printk("bt_enable(): failed: %d", ret);
    return 0;
  }

  ret = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ble_advertising_data, ARRAY_SIZE(ble_advertising_data), NULL, 0);
  if(ret) {
    printk("bt_le_adv_start(): failed: %d", ret);
    return 0;
  }

  while(1) {
    k_msleep(SLEEP_MS);
  }
	return 0;
}
