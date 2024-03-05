#ifndef _SERVER_H_
#define _SERVER_H_

#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_http_server.h"
#include "esp_http_client.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "../../credentials/credentials.h"
#include "servo.h"

extern bool wifi_established;
extern char *TAG;

void wifi_init();
void wifi_event_handler(void *, esp_event_base_t, int32_t, void *);
httpd_handle_t start_webserver(void);
esp_err_t get_root_handler(httpd_req_t *);
esp_err_t get_example_handler(httpd_req_t *);
esp_err_t get_favicon_handler(httpd_req_t *);
esp_err_t get_open_handler(httpd_req_t *);
esp_err_t get_close_handler(httpd_req_t *);

#endif /* _SERVER_H_ */