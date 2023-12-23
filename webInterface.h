//Kopiert von Github
#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"

#define MAX_RELAY_TIMERS 32
#define MAX_RELAY_AMOUNT 16




class WebInterface {
private:
    int relayTimer_Amount = 2; //menge an RelayTimer die auf der Webseite erstellt wurden
    
    //argumente für den RelayTimer
    int gpio_pin_switch[MAX_RELAY_TIMERS];
    int frequency[MAX_RELAY_TIMERS] = {};
    int length_sec[MAX_RELAY_TIMERS] = {};
    int relay_arr[MAX_RELAY_TIMERS][MAX_RELAY_AMOUNT] = {{}};
    bool toggleable[MAX_RELAY_TIMERS] = {};

public:
    WebInterface(){
        //const char WIFI_SSID[] = "TABLET";
        //const char WIFI_PASSWORD[] = "12345678";
    }

    void init_webInterface() {
        stdio_init_all(); 

        cyw43_arch_init();

        cyw43_arch_enable_sta_mode();

        // Connect to the WiFI network - loop until connected
        while(cyw43_arch_wifi_connect_timeout_ms("TABLET", "12345678", CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
            printf("Attempting to connect...\n");
        }
        // Print a success message once connected
        printf("Connected! \n");
        
        // Initialise web server
        httpd_init();
        printf("Http server initialised\n");

        // Configure SSI and CGI handler
        ssi_init(); 
        printf("SSI Handler initialised\n");
        cgi_init();
        printf("CGI Handler initialised\n");
    }

    

    bool waiting_for_Input() {
        return false;
    }

    int get_RelayTimer_Amount(){
        return relayTimer_Amount;
    }

    //array mit Arumenten für RelayTimer
    int get_RelayTimer_gpio_pin_switch(int index){ 
        return gpio_pin_switch[index];
    }
    int get_RelayTimer_frequency(int index){ 
        return frequency[index];
    }
    int get_RelayTimer_length_sec(int index){ 
        return length_sec[index];
    }
    int* get_RelayTimer_relay_arr(int index){ 
        return relay_arr[index];
    }
    bool get_RelayTimer_toggleable(int index){ 
        return toggleable[index];
    }
};
