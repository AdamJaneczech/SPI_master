#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char data = 169;

ISR(USI_OVF_vect){
    cli();
    PORTB ^= 1 << PB3;
    //USISR |= 1<<USIOIF; //clean the counter overflow flag
    sei();
}

void SPI_write(){
    //PORTB &= ~(1 << PB3);   //SS pin LOW
    USIDR = data;    //load the actual character into the USI data register
    while(USISR >= 0){ //counter counts from 15 to 0
        //USICR |= (1<<USIWM0)|(1<<USITC);    //toggle the clock pin
        _delay_ms(10);
        USICR |= (1<<USIWM0)|(1<<USICLK)|(1<<USITC);    //toggle the clock pin
        _delay_ms(10);
    }
    //USISR |= 1<<USIOIF; //clean the counter overflow flag
}

void SPI_setup(){
    USICR |= 1 << USIOIE;   //Enable counter overflow interrupt
    USICR |= 1 << USIWM0;   //Three-wire mode (SPI)
    sei();
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