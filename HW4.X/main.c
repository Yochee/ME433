#include<xc.h>
#include<math.h>
#include<sys/attribs.h>
#define PI 3.1415926535898
#pragma config DEBUG=2
#pragma config JTAGEN=0
#pragma config ICESEL=3
#pragma config PWP=0x3F
#pragma config BWP=1
#pragma config CP=1
#pragma config FNOSC=3
#pragma config FSOSCEN=0
#pragma config IESO=0
#pragma config POSCMOD=2
#pragma config OSCIOFNC=1
#pragma config FPBDIV=0
#pragma config FCKSM=2
#pragma config WDTPS=0x14
#pragma config WINDIS=1
#pragma config FWDTEN=0
#pragma config FWDTWINSZ=3
#pragma config FPLLIDIV=1
#pragma config FPLLMUL=7
#pragma config FPLLODIV=1
#pragma config UPLLIDIV=1
#pragma config UPLLEN=0
#pragma config USERID=0
#pragma config PMDL1WAY=0
#pragma config IOL1WAY=0
#pragma config FUSBIDIO=1
#pragma config FVBUSONIO=1
unsigned char SPI1_IO(unsigned char);
void setVoltage(char,unsigned char);
void setup(void);
int main(){
	setup();
	int i;
	while(1){
		for(i=0;i<200;i++){
			setVoltage(0,127.5+127.5*sin((float)i*0.02*PI));
			setVoltage(1,255*(float)i*0.005);
			_CP0_SET_COUNT(0);
			while(_CP0_GET_COUNT()<24000){;}
		}
	}
	return 0;
}
unsigned char SPI1_IO(unsigned char o){
	SPI1BUF=o;
	while(!SPI1STATbits.SPIRBF){;}
	return SPI1BUF;
}
void setVoltage(char channel,unsigned char voltage){
	unsigned char a,b;
	a=channel<<7;
	a=a|0b01110000;
	a=a|(voltage>>4);
	b=voltage<<4;
	LATBbits.LATB7=0;
	SPI1_IO(a);
	SPI1_IO(b);
	LATBbits.LATB7=1;
}
void setup(void){
	SDI1Rbits.SDI1R=4;
	RPA1Rbits.RPA1R=3;
	SPI1CON=0;
	SPI1BUF;
	SPI1BRG=1;
	SPI1STATbits.SPIROV=0;
	SPI1CONbits.CKE=1;
	SPI1CONbits.MSTEN=1;
	SPI1CONbits.ON=1;
	TRISBbits.TRISB7=0;
}