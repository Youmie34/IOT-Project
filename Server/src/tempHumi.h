#ifndef __TEMPHUMI__H__
#define __TEMPHUMI__H__

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "../lib/bme680/bme680.h"
#include <string.h>

#define SDA_GPIO 21
#define SCL_GPIO 22
#define PORT 0
#define ADDR BME680_I2C_ADDR_0

#if defined(CONFIG_IDF_TARGET_ESP32S2)
#define APP_CPU_NUM PRO_CPU_NUM
#endif

// bme680 sensor
extern bme680_t sensor;
// global variables for humidity and temperatur
extern float humi;
extern float temp;

void bme680_init();
void bme680_config();
void startMeasurement();

// setter
void setTemp(float measuredTemp);
void setHumi(float measuredHumi);

// getter
float getTemp();
float getHumi();

#endif /* __TEMPHUMI__H__ */