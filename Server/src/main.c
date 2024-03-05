#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <string.h>
#include "tempHumi.h"

void app_main()
{
    // settings for i2c communication with bme680
    ESP_ERROR_CHECK(i2cdev_init());
    bme680_init();
    bme680_config();

    TickType_t last_wakeup = xTaskGetTickCount();

    while (1)
    {
        startMeasurement();
        float temp = getTemp();
        float humi = getHumi();
        printf("Temperatur: %.2f °C, Humidität: %.2f %%\n", temp, humi);
        // passive waiting until 1 second is over
        vTaskDelayUntil(&last_wakeup, 1000 / portTICK_PERIOD_MS);
    }
}