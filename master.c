#include <avr/io.h>

char data[] = "Hello";
void SPI_write();
void SPI_setup();

int main(void){
    //DDRB |= 1 << PB3;   //SS
    DDRB |= 1 << PB2;   //SCK output
    //PB1 set as input (default)
    DDRB |= 1 << PB0;   //MOSI

    SPI_setup();
}

void SPI_write(){
    //PORTB &= ~(1 << PB3);   //SS pin LOW
    for(char i = 0; i < sizeof data; i++){
        USIDR = data[i];    //load the actual character into the USI data register
        for(char y = 0; y < 8; y++){
            USICR = (1<<USIWM0)|(1<<USITC);    //toggle the clock pin
            USICR = (1<<USIWM0)|(1<<USICLK)|(1<<USITC);  //toggle the register shifter & the clock pin
        }
    }
    //...
    //PORTB |= 1 << PB3;  //SS pin HIGH
}

void SPI_setup(){
    USICR |= 1 << USIWM0;   //Three-wire mode (SPI)
}