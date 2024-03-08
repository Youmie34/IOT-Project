#include "esp_check.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_http_client.h"

esp_err_t http_event_handler(esp_http_client_event_t *evt);
void urlAufruf(char *);

