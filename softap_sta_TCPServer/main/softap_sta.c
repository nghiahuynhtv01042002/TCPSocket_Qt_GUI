#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include <stdio.h>
#include "driver/gpio.h"
#include "driver/uart.h"

/* WiFi configuration */
#define EXAMPLE_ESP_WIFI_STA_SSID           "Nghiatv01"
#define EXAMPLE_ESP_WIFI_STA_PASSWD         "01042002"
#define EXAMPLE_ESP_WIFI_AP_SSID            "Nghiatv@esp"
#define EXAMPLE_ESP_WIFI_AP_PASSWD          "123456789"
#define EXAMPLE_ESP_WIFI_CHANNEL            1 // Replace CONFIG_ESP_WIFI_AP_CHANNEL with the actual channel number
#define EXAMPLE_MAX_STA_CONN                4 // Replace CONFIG_ESP_MAX_STA_CONN_AP with the actual max connection number
#define EXAMPLE_ESP_MAXIMUM_RETRY           5 // Replace CONFIG_ESP_MAXIMUM_STA_RETRY with the actual maximum retry number
#define BLINK_GPIO 2
/* TCP Server parameters */
#define PORT 1234
#define LISTENQ 5

/* Event group */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG_AP = "WiFi SoftAP";
static const char *TAG_STA = "WiFi Sta";
static const char *TAG_TCP = "TCP Server";

static int s_retry_num = 0;
static EventGroupHandle_t s_wifi_event_group;

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *) event_data;
        ESP_LOGI(TAG_AP, "Station " MACSTR " joined, AID=%d", MAC2STR(event->mac), event->aid);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *) event_data;
        ESP_LOGI(TAG_AP, "Station " MACSTR " left, AID=%d", MAC2STR(event->mac), event->aid);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        ESP_LOGI(TAG_STA, "Station started");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG_STA, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG_STA, "connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(TAG_STA, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

esp_netif_t *wifi_init_softap(void)
{
    esp_netif_t *esp_netif_ap = esp_netif_create_default_wifi_ap();

    wifi_config_t wifi_ap_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_AP_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_AP_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_AP_PASSWD,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .required = false,
            },
        },
    };

    if (strlen(EXAMPLE_ESP_WIFI_AP_PASSWD) == 0) {
        wifi_ap_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_ap_config));

    ESP_LOGI(TAG_AP, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             EXAMPLE_ESP_WIFI_AP_SSID, EXAMPLE_ESP_WIFI_AP_PASSWD, EXAMPLE_ESP_WIFI_CHANNEL);

    return esp_netif_ap;
}

esp_netif_t *wifi_init_sta(void)
{
    esp_netif_t *esp_netif_sta = esp_netif_create_default_wifi_sta();

    wifi_config_t wifi_sta_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_STA_SSID,
            .password = EXAMPLE_ESP_WIFI_STA_PASSWD,
            .scan_method = WIFI_ALL_CHANNEL_SCAN,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_sta_config));

    ESP_LOGI(TAG_STA, "wifi_init_sta finished.");

    return esp_netif_sta;
}
void init_GPIO(void){
    // gpio_set_direction(BLINK_GPIO,GPIO_MODE_OUTPUT);
    
    gpio_set_direction(BLINK_GPIO,GPIO_MODE_INPUT_OUTPUT);

}

void tcp_server_task(void *pvParameters)
{
    char rx_buffer[128];
    char addr_str[128];
    int addr_family;
    int ip_protocol;

    while (1) {
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
        if (listen_sock < 0) {
            ESP_LOGE(TAG_TCP, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG_TCP, "Socket created");

        int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            ESP_LOGE(TAG_TCP, "Socket unable to bind: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG_TCP, "Socket bound, port %d", PORT);

        err = listen(listen_sock, LISTENQ);
        if (err != 0) {
            ESP_LOGE(TAG_TCP, "Error occurred during listen: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG_TCP, "Socket listening");

        while (1) {
            struct sockaddr_in source_addr;
            socklen_t addr_len = sizeof(source_addr); // Change uint to socklen_t
            int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
            if (sock < 0) {
                ESP_LOGE(TAG_TCP, "Unable to accept connection: errno %d", errno);
                break;
            }

            inet_ntoa_r(source_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
            ESP_LOGI(TAG_TCP, "Socket accepted ip address: %s", addr_str);

            while (1) {
                int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
                if (len < 0) {
                    ESP_LOGE(TAG_TCP, "recv failed: errno %d", errno);
                    break;
                } else if (len == 0) {
                    ESP_LOGI(TAG_TCP, "Connection closed");
                    break;
                } else {
                    rx_buffer[len] = 0;
                    ESP_LOGI(TAG_TCP, "Received %d bytes: %s", len, rx_buffer);
                    //chang led status
                      // Kiểm tra nếu rx_buffer là "ON"
                      int en_ack =0;
                    if (strcmp(rx_buffer, "ON") == 0) {
                        // Bật pin 2
                        gpio_set_level(BLINK_GPIO, 1);
                        ESP_LOGI(TAG_TCP, "Pin 2 turned ON");
                        en_ack = 0;
                    } else if (strcmp(rx_buffer, "OFF") == 0) {
                        // Tắt pin 2
                        gpio_set_level(BLINK_GPIO, 0);
                        ESP_LOGI(TAG_TCP, "Pin 2 turned OFF");
                        en_ack = 0;
                    }  
                    else{
                        en_ack =1;
                    }
                    
 


                    char ack_rx_buffer[256];
                    int led_status = gpio_get_level(BLINK_GPIO) ;
                    printf("Led status: %s",led_status==1?"ON":"OFF");
                    if(led_status == 0 && en_ack == 0){
                        sprintf(ack_rx_buffer,"Led Status: OFF ");
                        send(sock, ack_rx_buffer, strlen(ack_rx_buffer), 0);
                    }
                    else if(led_status == 1 && en_ack == 0){
                        sprintf(ack_rx_buffer,"Led Status: ON");
                        send(sock, ack_rx_buffer, strlen(ack_rx_buffer), 0);

                    }
                    if(en_ack){
                        sprintf(ack_rx_buffer,"ack: %s",rx_buffer);
                        send(sock, ack_rx_buffer, strlen(ack_rx_buffer), 0);
                    }


                }
            }

            if (sock != -1) {
                ESP_LOGE(TAG_TCP, "Shutting down socket and restarting...");
                shutdown(sock, 0);
                close(sock);
            }
        }

        if (listen_sock != -1) {
            ESP_LOGE(TAG_TCP, "Shutting down listening socket and restarting...");
            shutdown(listen_sock, 0);
            close(listen_sock);
        }
    }

    vTaskDelete(NULL);
}

void app_main(void)
{
    //  gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    init_GPIO();

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    wifi_init_softap();
    wifi_init_sta();

    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG_STA, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_STA_SSID, EXAMPLE_ESP_WIFI_STA_PASSWD);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG_STA, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_STA_SSID, EXAMPLE_ESP_WIFI_STA_PASSWD);
    } else {
        ESP_LOGE(TAG_STA, "UNEXPECTED EVENT");
        return;
    }

    xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);
}
