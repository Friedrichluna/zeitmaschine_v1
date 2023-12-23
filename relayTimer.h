#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

#define MAX_RELAY_TIMERS 32
#define MAX_RELAY_AMOUNT 16
#define ITERATIONS_PER_SECOND 100;


void print_something() {
    printf("test");
}

class RelayTimer {
private:
    int gpio_pin_switch;
    int frequency;
    int length_sec;
    int relay_arr[MAX_RELAY_AMOUNT];
    int relay_amount;
    bool toggleable;
    bool activateTimer = false;
    bool switchOn;
    bool switchOnLastIteration;
    int iterations_per_second = ITERATIONS_PER_SECOND;

    int counter = 0; 

public:
    RelayTimer(int gpio_pin_switch_ = 0, int frequency_ = 0, int length_sec_ = 0,int relay_arr_[MAX_RELAY_AMOUNT] = {},int relay_amount_ = 0, bool toggleable_ = false) {
        gpio_pin_switch = gpio_pin_switch_;
        frequency = frequency_;
        length_sec = length_sec_;
        relay_arr[MAX_RELAY_AMOUNT] = {};
        relay_amount = relay_amount_;
        for (int i = 0; i < relay_amount; i++) {
            relay_arr[i] = relay_arr_[i];
        }
        toggleable = toggleable_;
        
    }
    void initGPIO(){
        // Initialize Relais-Pins
        for (int i = 0; i < relay_amount; i++){
            gpio_init(relay_arr[i]);
            gpio_set_dir(relay_arr[i], GPIO_OUT);
        }
        //Initialize switch
        gpio_init(gpio_pin_switch);
        gpio_set_dir(gpio_pin_switch, GPIO_IN);
        gpio_pull_up(gpio_pin_switch);
        switchOnLastIteration = !gpio_get(gpio_pin_switch);
    }

    void routine(){
        //Bestimmung, ob Timer aktiv sein soll (Toggle oder Hold Modus)
        if(toggleable){
		    //Wenn Schalter an ist und in der vorherigen Iteration aus war, dann wird activateTimer getogglet
            switchOn = !gpio_get(gpio_pin_switch);
		    if(switchOn && !switchOnLastIteration){
			    //toggles activateTimer
			    activateTimer = !activateTimer;
                printf("switch %d toggled\n", gpio_pin_switch);
		    }   
		//setzt switchOnLastIteration auf aktuellen Wert des Switches für die nächste Iteration
		switchOnLastIteration = switchOn;
	    }
	    else{
            //Wenn toggleable auf false ist, wird activateTimer true, solange button gedrückt ist
		    activateTimer = !gpio_get(gpio_pin_switch);
	    }

        if(activateTimer){
            counter = counter + 1;
            printf("switch %d on\n", gpio_pin_switch);
            //Überprüft, ob Zeit abgelaufen ist oder Zeit auf 0 (Dauerbetrieb, solange Schalter an)
            if(counter < length_sec*iterations_per_second || length_sec == 0){
                //überprüft, ob Relais getoggelt werden soll 
                //(Wenn counter Vielfaches von ITERATIONS_PER_SECOND durch 2*frequency erreicht
                // ==> da zweimal pro Periode Relais getoggelt wird (An und Aus))
                int a = (counter - 1) % (iterations_per_second/(2*frequency));
                printf("ergebnis1 = %d\n", a);
                int b = (iterations_per_second/(2*frequency));
                printf("ergebnis2 = %d\n", b);
                
                printf("iterations_per_second = %d\n", iterations_per_second);
                printf("frequenzy = %d\n", frequency);
                if((counter - 1) % (iterations_per_second/(2*frequency)) == 0){
                    printf("toggle\n");
                    relaisToggle(relay_arr, relay_amount);
                }
            }
            //Schalte Relais aus, wenn Zeit abgelaufen
            else if(counter ==length_sec*iterations_per_second){
                relaisReset(relay_arr, relay_amount);
            }
        }
        else{
            //Wenn activateTimer false ist, wird counter auf 0 gesetzt und die Relais ausgeschalten
            printf("switch %d off\n", gpio_pin_switch);
            if(counter != 0){
                counter = 0;
                relaisReset(relay_arr, relay_amount);
            }
        }

    }

private:
    //Funktion zum Togglen von Relais
    void relaisToggle(int relais_pin[], int amount){
        printf("toggle pin method");
        for (int i = 0; i < amount; i++){
            printf("toggle pin %d\n", relais_pin[i]);
            if(gpio_get(relais_pin[i])){
                gpio_put(relais_pin[i], 0);
            }
            else{
                gpio_put(relais_pin[i], 1);
            }   
        }
    }
    
    // Funktion schaltet alle Relais aus
    void relaisReset(int relais_pin[], int amount){
        for (int i = 0; i < amount; i++){
            gpio_put(relais_pin[i], 0);
        }
    }
};
