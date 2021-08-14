#define F_CPU 8000000

#include <avr/io.h>
#include <avr/delay.h>

char data = 169;

void SPI_write(){
    //PORTB &= ~(1 << PB3);   //SS pin LOW
    USIDR = data;    //load the actual character into the USI data register
    if(USIOIF == 0b0){   
        PORTB ^= 1<<PB3;
        _delay_ms(500);
        PORTB ^= 1<<PB3;
        _delay_ms(50);
    }
    while(USIOIF==0){
        USICR |= (1<<USIWM0)|(1<<USICLK)|(1<<USITC);    //toggle the clock pin
    }
    /*for(char y = 0; y < 8; y++){
        USICR = (1<<USIWM0)|(1<<USITC);    //toggle the clock pin
        USICR = (1<<USIWM0)|(1<<USICLK)|(1<<USITC);  //toggle the register shifter & the clock pin
    }*/
    /*if(USISR & 1<<USIOIF){
        PORTB |= 1 << PB3;
    }*/
    USISR |= 1<<USIOIF; //clean the counter overflow flag
}

void SPI_setup(){
    USICR |= 1 << USIWM0;   //Three-wire mode (SPI)
}

int main(void){
    DDRB |= 1 << PB3;   //LED indicator
    DDRB |= 1 << PB2;   //SCK output
    //PB1 set as input (default)
    DDRB |= 1 << PB0;   //MOSI
    //LED start-up blink
    PORTB ^= 1 << PB3;
    _delay_ms(200);
    PORTB ^= 1 << PB3;

    SPI_setup();
    while(1){
        if(PINB & (1<<PB4)){
            SPI_write();
        }
            
    }
}