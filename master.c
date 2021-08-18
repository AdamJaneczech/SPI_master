#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t data = 169;

ISR(USI_OVF_vect){
    cli();
    PORTB ^= 1 << PB3;
    //USISR |= 1<<USIOIF; //clean the counter overflow flag
    sei();
}

void SPI_write(){
    //PORTB &= ~(1 << PB3);   //SS pin LOW
    USISR |= 1<<USIOIF;
    USIDR = data;    //load the actual character into the USI data register
    while(!(USISR & 1<<USIOIF)){ //counter counts from 15 to 0
        _delay_ms(250);
        USICR |= (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
        _delay_ms(250);
    }
    USICR |= (1<<USIWM0)|(1<<USITC);
    USISR |= 1<<USIOIF;
    //USISR |= 1<<USIOIF; //clean the counter overflow flag
}

void SPI_setup(){
    //USICR |= 1 << USIOIE;   //Enable counter overflow interrupt
    USICR |= 1 << USIWM0 |/* 1 << USICS1 | */1 << USICLK;   //Three-wire mode (SPI)
    sei();
}

int main(void){
    DDRB |= 1 << DDB3;   //LED indicator
    DDRB |= 1 << DDB2;   //SCK output
    DDRB |= 1 << DDB1;   //DO
    //PB0 set as input (default) -> DI
    //LED start-up blink
    PORTB &= ~(1 << PB0);
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