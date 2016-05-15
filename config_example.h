// All parameters are case sensitive
#define SSID "my_ap"
#define PASSWORD "my_key"
#define HTTP_HOST "www.webserver.com"
#define HTTP_PORT 80
#define HTTP_PATH "/power.txt"
#define PIN_POWER_ON 4 // Pin for powering on
#define PIN_POWER_OFF 5 // Pin for powering off
#define PIN_RELAY 13 // Pin for direct relay control
#define DIRECT_RELAY 1 // Define if controlling a relay directly (no remote)
#define WIFI_POLLING_INTERVAL 5000 // How often to try to get WIFI connected
#define HTTP_POLLING_INTERVAL 50000 // How often to poll file over HTTP (ms)
#define HTTP_TIMEOUT 10000 // How long to wait for HTTP reply (ms)
