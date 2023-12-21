#include "pico/stdlib.h"
#include "dfPlayerDriver.h"

void doSomethingMusic() {
    //DfPlayerPico
    DfPlayerPico dfp;

    dfp.reset();
    sleep_ms(2000);

    dfp.specifyVolume(18);
    sleep_ms(200);

    dfp.setRepeatPlay(true);
    sleep_ms(200);

    dfp.next();
    sleep_ms(200);
}
