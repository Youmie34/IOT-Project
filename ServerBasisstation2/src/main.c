#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_event.h"
#include <string.h>
#include "wifi.h"
#include "server.h"

void app_main()
{
    wifi_init();
    start_webserver();

    /*while (true)
    {
        if (wifi_established)
        {
            // WiFi Verbindung hergestellt
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS); // wait 100 ms
    }*/
}
