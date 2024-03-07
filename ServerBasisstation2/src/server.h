#include "esp_http_server.h"

httpd_handle_t start_webserver(void);
esp_err_t get_root_handler(httpd_req_t *);
esp_err_t get_example_handler(httpd_req_t *);
esp_err_t get_favicon_handler(httpd_req_t *);
esp_err_t get_open_handler(httpd_req_t *);
esp_err_t get_close_handler(httpd_req_t *);