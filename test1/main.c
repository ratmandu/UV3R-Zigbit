#include "main.h"

int main(void) {
    clock_prescale_set(0);
    TOPLED_PORT = (1<<TOPLED_PIN);
    TOPLED_DDR |= (1<<TOPLED_PIN);
    
    rda_init();
    
    //PORTD |= (1<<PD5);
    //DDRD  |= (1<<PD5);
    
    rda_setTxRx(0);

    while (1) {
        rda_setTxRx(0);
        TOPLED_PORT &= ~(1<<TOPLED_PIN);
        rda_setRef();
        _delay_ms(60000);
    }

    return 1;
}

