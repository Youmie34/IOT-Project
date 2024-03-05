#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <string.h>
#include "tempHumi.h"

const float epsilon = 0.01;

void app_main()
{
    // settings for i2c communication with bme680
    ESP_ERROR_CHECK(i2cdev_init());
    bme680_init();
    bme680_config();

    TickType_t last_wakeup = xTaskGetTickCount();

    // start to measure temperature and humidity
    while (1)
    {
        startMeasurement();
        int temp = (int)getTemp();
        int humi = (int)getHumi();
        printf("Temperatur: %d °C, Humidität: %d%%\n", temp, humi);
        // passive waiting until 1 second is over
        vTaskDelayUntil(&last_wakeup, 1000 / portTICK_PERIOD_MS);
    }
}