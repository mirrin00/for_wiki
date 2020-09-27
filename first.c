#ifndef F_CPU
#define F_CPU 4000000UL
#endif

#include<util/delay.h>
#include<avr/io.h>

unsigned char run[] = {
	0b00000001,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000,
};

int main(){
	DDRB &= ~1;
	PORTB |= 1;
	DDRA = 0xFF;
	int cur = 0, way = 1;
	while(1){
		PORTA = run[cur];
		_delay_ms(100);
		if(PINB & 1) way *= -1;
		cur+=way;
		if(cur < 0) cur = 7;
		cur = cur % 8;
	}
}
