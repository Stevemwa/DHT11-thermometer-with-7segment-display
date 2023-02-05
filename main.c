/*
 * DHT11.c
 *
 * Created: 09/01/2023 17:18:10
 * Author : Mwas
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>

void display();
void read();

#define DHT11_PIN PD0
unsigned char c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;
unsigned char number[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
int k;

void Request()				/* Micro controller send start pulse/request */
{
	DDRD |= (1<<DHT11_PIN); /* PIN D0 as output */
	PORTD &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);			/* wait for 20ms */
	PORTD |= (1<<DHT11_PIN);	/* set to high pin */
}

void Response()				/* receive response from DHT11 */
{
	DDRD &= ~(1<<DHT11_PIN);/* PIN D0 as input */
	while(PIND & (1<<DHT11_PIN));/* while true=wait=!0=1&1=1 until false =0=exit while=0&1 */
	while((PIND & (1<<DHT11_PIN))==0);/* wait for a high pulse/1 when PIND=1 (PIND & 1)=1 which !=0 thus false exit while */
	while(PIND & (1<<DHT11_PIN));/* wait for low pulse while true=wait= !0 = (1&1)=1 until false =0=exit while=0&1 */
}

unsigned char Receive_data()			/* receive data */
{
	for (int q=0; q<8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0);  /* wait for a high pulse when PIND=1 (PIND & 1)=1 which !=0 thus false exit while */
		_delay_us(40);
		if(PIND & (1<<DHT11_PIN))/* if PIND is still high after 40us/ greater than 40us */
		c = (c<<1)|(0x01);	/* then DHT11 has sent a logic HIGH */
		else			/* otherwise its logic LOW */
		c = (c<<1);
		while(PIND & (1<<DHT11_PIN));/* wait for low pulse */
	}
	return c;
}

int main(void)
{
	int data;
	DDRB=0xff;
	DDRC=0x0f;
	
	while(1)
	{  
		_delay_ms(2000);
		Request();		/* send start pulse */
		Response();		/* receive response */
		I_RH=Receive_data();	/* store first eight bit in I_RH */
		D_RH=Receive_data();	/* store next eight bit in D_RH */
		I_Temp=Receive_data();	/* store next eight bit in I_Temp */
		D_Temp=Receive_data();	/* store next eight bit in D_Temp */
		CheckSum=Receive_data();/* store next eight bit in CheckSum */
		
		if ((I_RH + D_RH + I_Temp + D_Temp) == CheckSum)
		{
		for (int a=0;a<=1000;a++)
		{
			
			PORTC=~(0x01);
			PORTB=0x79;
			_delay_ms(20);
			PORTC=~(0x00);
			
			PORTC=~(0x02);
			PORTB=0x77;
			_delay_ms(20);
			PORTC=~(0x00);
			
			PORTC=~(0x04);
			PORTB=0x77;
			_delay_ms(20);
			PORTC=~(0x00);

		}
		}
		
		else
		{
			unsigned char value;
			for(int i=0;i<=7;i++){
				
				value =(I_Temp &(00000001 << i));
				if(value == 00000000){
					 k=k+0;
					}else{
					k=k +(pow(2,i));
				}
			}
			data=k*100;
			k=0;
			read(data);
			
			for(int i=0;i<=7;i++){
				
				value =(D_Temp &(00000001 << i));
				if(value == 00000000){
					k=k+0;
					}else{
					k=k +(pow(2,i));
				}
			}
			data=k*100;
			k=0;
			
			read(data);
			
			
			
		
		}
		
		_delay_ms(10);
	}
}


void display(int digit){
	PORTB=number[digit];
	_delay_ms(10);
}
void read(int temp){
	int thao=temp/1000;
	PORTC=~(0x01);
	display(thao);
	PORTC=0x0f;
	
	int hund=(temp-(thao*1000))/100;
	PORTC=~(0x02);
	display(hund);
	PORTC=0x0f;
	
	int tens=(temp-((hund*100)+(thao*1000)))/10;
	PORTC=~(0x04);
	display(tens);
	PORTC=0x0f;
	
	int ones=(temp-((hund*100)+(thao*1000)+(tens*10)));
	PORTC=~(0x08);
	display(ones);
	PORTC=0x0f;
	
	
}