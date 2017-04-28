#include<xc.h>
#include<sys/attribs.h>
#include<stdio.h>
#include"ILI9163C.h"
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
#define STRINGLENTH 100
#define BARWIDTH 10
void bar(unsigned short x,unsigned short y,unsigned short color,unsigned short back_color){
	unsigned short i,j;
	long int count=24000000/500;
	for(i=0;i<=100;i++){
		for(j=0;j<=BARWIDTH;j++){
			LCD_drawPixel(i+x,y+j,color);
		}
		char ch1[10],ch2[10];
		sprintf(ch1,"%d",i);
		print_string(ch1,x+45,y+5+BARWIDTH,color);
		clearBar(x+47,x+90,y+BARWIDTH+25,y+BARWIDTH+41,back_color);
		sprintf(ch2,"FPS:%.2f",(double)_CP0_GET_COUNT()/count);
		print_string(ch2,x+25,y+25+BARWIDTH,color);
		_CP0_SET_COUNT(0);
		while(_CP0_GET_COUNT()<count){;}
		clearBar(x+45,x+70,y+BARWIDTH+5,y+BARWIDTH+21,back_color);
	}
	clearBar(x,x+100,y,y+BARWIDTH,back_color);
}
int main(){
	SPI1_init();
	LCD_init();
	LCD_clearScreen(WHITE);
	char str[STRINGLENTH];
	sprintf(str,"Hello World!");
	print_string(str,28,32,BLACK);
	while(1){
		bar(10,50,BLACK,WHITE);
	}
}