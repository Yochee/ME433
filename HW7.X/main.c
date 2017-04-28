#include<xc.h>
#include<sys/attribs.h>
#include<stdio.h>
#include"SPI.h"
#include"I2C.h"
#include"LCD.h"
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
#define STRLENGTH 100
#define FREQUENCY 20
void I2C_read_multiple(char,char,char*,int);
void IMUinit(void);
int main(void){
	TRISAbits.TRISA4=0;
	TRISBbits.TRISB4=1;
	LATAbits.LATA4=1;
	initSPI1();
	LCD_init();
	LCD_clearScreen(BLUE);
	IMUinit();
	char data[STRLENGTH];
	short data_combine[STRLENGTH];
	long count=24000000/(FREQUENCY);
	int i=0;
	while(1){
		_CP0_SET_COUNT(0);
		while(_CP0_GET_COUNT()<count){;}
		I2C_read_multiple(0x6a,0x20,data,14);
		for(i=0;i<7;i++){
			data_combine[i]=data[2*i+1]<<8|data[2*i];
		}
		flowbar(data_combine[4],data_combine[5]);
	}
	return 0;
}
void I2C_read_multiple(char address,char regis,char *data,int length){
	i2c_master_start();
	char write=address<<1|0;
	char read=address<<1|1;
	i2c_master_send(write);
	i2c_master_send(regis);
	i2c_master_restart();
	i2c_master_send(read);
	int i=0;
	for(i=0;i<length;i++){
		*data++=i2c_master_recv();
		if(i==(length-1)){
			i2c_master_ack(1);
			break;
		}
		i2c_master_ack(0);
	}
	i2c_master_stop();
}
void IMUinit(void){
	ANSELBbits.ANSB2=0;
	ANSELBbits.ANSB3=0;
	i2c_master_setup();
	i2c_master_start();
	i2c_master_send(0xd4);
	i2c_master_send(0x10);
	i2c_master_send(0x82);
	i2c_master_stop();
	i2c_master_start();
	i2c_master_send(0xd4);
	i2c_master_send(0x11);
	i2c_master_send(0x88);
	i2c_master_stop();
	i2c_master_start();
	i2c_master_send(0xd4);
	i2c_master_send(0x04);
	i2c_master_stop();
}