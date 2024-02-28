#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "iot_servo.h"

// servo on pin 22 (jumper 6)
#define SERVO_PIN 22

 // open window
void openWindow()
{
    for (int angle = 0; angle <= 180; angle += 10)
    {
        iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, angle);
        printf("angle %d\n", angle);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// close window
void closeWindow()
{
    for (int angle = 180; angle >= 0; angle -= 10)
    {
        iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, angle);
        printf("angle %d\n", angle);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main()
{
    // configure servo
    servo_config_t servo_cfg = {
        .max_angle = 180,
        .min_width_us = 690,  // equals "0°"
        .max_width_us = 2780, // equals "180°"
        .freq = 50,
        .timer_number = LEDC_TIMER_0,
        .channels = {
            .servo_pin = {SERVO_PIN},
            .ch = {LEDC_CHANNEL_0},
        },
        .channel_number = 1,
    };

    // initialize servo
    esp_err_t err = iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_cfg);
    if (err != ESP_OK)
    {
        ESP_LOGE("app_main", "Servo initialization failed");
        vTaskDelete(NULL);
    }

    //openWindow();
    closeWindow();

    // deinitialize servo
    iot_servo_deinit(LEDC_LOW_SPEED_MODE);

    // delete task
    vTaskDelete(NULL);
}
