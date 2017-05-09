#include"SPI.h"
#include<xc.h>
#include<sys/attribs.h>
char spi_io(char o){
	SPI1BUF=o;
	while(!SPI1STATbits.SPIRBF){;}
	return SPI1BUF;
}
void setVoltage(char channel,char voltage){
	char a,b;
	a=channel<<7;
	a=a|0b01110000;
	a=a|(voltage>>4);
	b=voltage<<4;
	LATBbits.LATB7=0;
	spi_io(a);
	spi_io(b);
	LATBbits.LATB7=1;
}
void initSPI1(void){
	SDI1Rbits.SDI1R=0b0100;
	RPA1Rbits.RPA1R=0b0011;
	TRISBbits.TRISB7=0;
	LATBbits.LATB7=1;
	ANSELBbits.ANSB15=0;
	TRISBbits.TRISB15=0;
	LATBbits.LATB15=0;
	SPI1CON=0;
	SPI1BUF;
	SPI1BRG=1;
	SPI1STATbits.SPIROV=0;
	SPI1CONbits.CKE=1;
	SPI1CONbits.MSTEN=1;
	SPI1CONbits.ON=1;
}