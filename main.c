/*
 * DHT11.c
 *
 * Created: 09/01/2023 17:18:10
 * Author : Mwas
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char number[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
int digit;
void display();
void read();
int hund;
int tens;
int ones;



int main(void)
{
	
	DDRB=0xff;
	DDRD=0xfe;
	
	
    /* Replace with your application code */
    while (1) 
    {
		read(567);
		
		
		
		
    }
	return(0);
}

void display(int digit){
	PORTB=number[digit];
	_delay_ms(10);
}
void read(int temp){
	hund=temp/100;
	PORTC=~(0x01);
	display(hund);
	PORTC=0x0f;
	tens=(temp-(hund*100))/10;
	PORTC=~(0x02);
	display(tens);
	PORTC=0x0f;
	ones=temp%10;
	PORTC=~(0x04);
	display(ones);
	PORTC=0x0f;
	
	
}

void start_signal(){
	DDRD=0xff;
	PORTD=0x00;
	_delay_ms(20);
	PORTD=0x01;
}

void response_signal(){
	DDRD=0xf0;
	
	
}