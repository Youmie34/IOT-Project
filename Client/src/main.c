#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_wifi.h"
#include "esp_check.h"
#include "esp_log.h"
#include "iot_servo.h"
#include "servo.h"
#include "esp_event.h"
#include <string.h>
#include "esp_http_server.h"
#include "esp_http_client.h"
#include "nvs_flash.h"
#include "../../credentials/credentials.h"

// TODO:server-connection beautify

bool wifi_established = false;
char *TAG = "Event_Handling";

static void wifi_init();
static void wifi_event_handler(void *, esp_event_base_t, int32_t, void *);
httpd_handle_t start_webserver(void);
esp_err_t get_root_handler(httpd_req_t *);
esp_err_t get_example_handler(httpd_req_t *);
esp_err_t get_favicon_handler(httpd_req_t *);
esp_err_t get_open_handler(httpd_req_t *);
esp_err_t get_close_handler(httpd_req_t *);

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

static void wifi_init()
{
    esp_netif_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    cfg.nvs_enable = false;
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_LOST_IP, &wifi_event_handler, NULL));
    wifi_config_t wifi_config =
        {
            .sta =
                {
                    .ssid = ESP_WIFI_SSID,
                    .password = ESP_WIFI_PASS,
                },
        };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        wifi_established = false;
        ESP_LOGI(TAG, "connect to the AP");
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        wifi_established = false;
        ESP_LOGI(TAG, "retry to connect to the AP");
        esp_wifi_connect();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip: " IPSTR, IP2STR(&(event->ip_info.ip)));
        wifi_established = true;
    }
    else
    {
        ESP_LOGI(TAG, "unhandled event (%s) with ID %d!", event_base, (int)event_id);
    }
}

/* URI handler structure for GET / uri */
httpd_uri_t uri_get_root = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_root_handler,
    .user_ctx = NULL};
/* URI handler structure for another GET / uri */
httpd_uri_t uri_get_example = {
    .uri = "/example",
    .method = HTTP_GET,
    .handler = get_example_handler,
    .user_ctx = NULL};

/* URI handler structure for another GET / uri */
httpd_uri_t uri_get_favicon = {
    .uri = "/favicon.ico",
    .method = HTTP_GET,
    .handler = get_favicon_handler,
    .user_ctx = NULL};

httpd_uri_t uri_get_open = {
    .uri = "/open",
    .method = HTTP_GET,
    .handler = get_open_handler,
    .user_ctx = NULL};

httpd_uri_t uri_get_close = {
    .uri = "/close",
    .method = HTTP_GET,
    .handler = get_close_handler,
    .user_ctx = NULL};

httpd_handle_t start_webserver(void)
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    /* Port 80*/
    config.server_port = 80;
    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;
    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK)
    {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &uri_get_root);
        httpd_register_uri_handler(server, &uri_get_example);
        httpd_register_uri_handler(server, &uri_get_favicon);
        httpd_register_uri_handler(server, &uri_get_open);
        httpd_register_uri_handler(server, &uri_get_close);
    }

    return server;
}

esp_err_t get_root_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    /* Send a simple response */
    const char resp[] = "Hello World!";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t get_example_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    /* Send a simple response */
    const char resp[] = "Hello World von Example!";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t get_favicon_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    /* Send a simple response */
    const char resp[] = "";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t get_open_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    /* Send a simple response */
    const char resp[] = "Fenster wird geöffnet";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    openWindow();
    return ESP_OK;
}

esp_err_t get_close_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    /* Send a simple response */
    const char resp[] = "Fenster wird geöffnet";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    closeWindow();
    return ESP_OK;
}