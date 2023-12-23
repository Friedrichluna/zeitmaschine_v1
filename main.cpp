#include "pico/stdlib.h"

#include "relayTimer.h"
#include "webInterface.h"
#include "musicTimer.h"



int main() {

    stdio_init_all(); //initialisiert Pico

    WebInterface wi = WebInterface(); //erstellt webinterface Objekt
    wi.init_webInterface(); //initialisiert Webinterface
    
    doSomethingMusic();

    while(wi.waiting_for_Input() == true) {
        sleep_ms(200);
    }
    

    int relayTimer_Amount = wi.get_RelayTimer_Amount();
    printf("amount = %d\n",relayTimer_Amount);
    RelayTimer relayTimer_arr[relayTimer_Amount]; //array mit leeren relayTimern

    // for (int i = 0; i < relayTimer_Amount; i++) { //schleife um relayTimer zu füllen
    //     int gpio_pin_switch = wi.get_RelayTimer_gpio_pin_switch(i);
    //     int frequency = wi.get_RelayTimer_frequency(i);
    //     int length_sec = wi.get_RelayTimer_length_sec(i);
    //     int* relay_arr = wi.get_RelayTimer_relay_arr(i);
    //     bool toggleable = wi.get_RelayTimer_toggleable(i);
    //     sleep_ms(1);
        
    //     RelayTimer newRelayTimer(gpio_pin_switch, frequency, length_sec, relay_arr, toggleable);
    //     relayTimer_arr[i] = newRelayTimer;
    // }
    
    //Testinitialisierungen
    int relay_arr1[1] = {2};
    int relay_arr2[1] = {3};
    RelayTimer newRelayTimer1(16, 2, 0, relay_arr1, 1, false);
    RelayTimer newRelayTimer2(15, 3, 0, relay_arr2, 1, true);
    relayTimer_arr[0] = newRelayTimer1;
    relayTimer_arr[1] = newRelayTimer2;

    for (int i = 0; i < relayTimer_Amount; i++) {
            relayTimer_arr[i].initGPIO();
    }

    while(true) {
        sleep_ms(10);
        for (int i = 0; i < relayTimer_Amount; i++) {
            relayTimer_arr[i].routine();
        }
    };
}
