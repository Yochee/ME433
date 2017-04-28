#include<xc.h>
#include"LCD.h"
#include"SPI.h"
void LCD_command(char com){
	LATBbits.LATB15=0;
	LATBbits.LATB7=0;
	spi_io(com);
	LATBbits.LATB7=1;
}
void LCD_data(char dat){
	LATBbits.LATB15=1;
	LATBbits.LATB7=0;
	spi_io(dat);
	LATBbits.LATB7=1;
}
void LCD_data16(unsigned short dat){
	LATBbits.LATB15=1;
	LATBbits.LATB7=0;
	spi_io(dat>>8);
	spi_io(dat);
	LATBbits.LATB7=1;
}
void LCD_init(void){
	int time=0;
	LCD_command(CMD_SWRESET);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/2){;}
	LCD_command(CMD_SLPOUT);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/200){;}
	LCD_command(CMD_PIXFMT);
	LCD_data(0x05);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/200){;}
	LCD_command(CMD_GAMMASET);
	LCD_data(0x04);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/1000){;}
	LCD_command(CMD_GAMRSEL);
	LCD_data(0x01);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/1000){;}
	LCD_command(CMD_NORML);
	LCD_command(CMD_DFUNCTR);
	LCD_data(0b11111111);
	LCD_data(0b00000110);
	int i=0;
	LCD_command(CMD_PGAMMAC);
	for(i=0;i<15;i++){
		LCD_data(pGammaSet[i]);
	}
	LCD_command(CMD_NGAMMAC);
	for(i=0;i<15;i++){
		LCD_data(nGammaSet[i]);
	}
	LCD_command(CMD_FRMCTR1);
	LCD_data(0x08);
	LCD_data(0x02);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/1000){;}
	LCD_command(CMD_DINVCTR);
	LCD_data(0x07);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/1000){;}
	LCD_command(CMD_PWCTR1);
	LCD_data(0x0A);
	LCD_data(0x02);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/1000){;}
	LCD_command(CMD_PWCTR2);
	LCD_data(0x02);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/1000){;}
	LCD_command(CMD_VCOMCTR1);
	LCD_data(0x50);
	LCD_data(99);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/1000){;}
	LCD_command(CMD_VCOMOFFS);
	LCD_data(0);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/1000){;}
	LCD_command(CMD_CLMADRS);
	LCD_data16(0x00);
	LCD_data16(_GRAMWIDTH);
	LCD_command(CMD_PGEADRS);
	LCD_data16(0x00);
	LCD_data16(_GRAMHEIGH);
	LCD_command(CMD_VSCLLDEF);
	LCD_data16(0);
	LCD_data16(_GRAMHEIGH);
	LCD_data16(0);
	LCD_command(CMD_MADCTL);
	LCD_data(0b00001000);
	LCD_command(CMD_DISPON);
	time=_CP0_GET_COUNT();
	while(_CP0_GET_COUNT()<time+48000000/2/1000){;}
	LCD_command(CMD_RAMWR);
}
void LCD_drawPixel(unsigned short x,unsigned short y,unsigned short color){
	LCD_setAddr(x,y,x+1,y+1);
	LCD_data16(color);
}
void LCD_setAddr(unsigned short x0,unsigned short y0,unsigned short x1,unsigned short y1){
	LCD_command(CMD_CLMADRS);
	LCD_data16(x0);
	LCD_data16(x1);
	LCD_command(CMD_PGEADRS);
	LCD_data16(y0);
	LCD_data16(y1);
	LCD_command(CMD_RAMWR);
}
void LCD_clearScreen(unsigned short color){
	int i;
	LCD_setAddr(0,0,_GRAMWIDTH,_GRAMHEIGH);
	for(i=0;i<_GRAMSIZE;i++){
		LCD_data16(color);
	}
}
void print_char(char ch,unsigned short x,unsigned short y,unsigned short color){
	char row;
	unsigned short i,j;
	for(j=0;j<=15;j++){
		for(i=0;i<=4;i++){
			row=ASCII[ch-0x20][i];
			if((row>>j)&0x01){
				LCD_drawPixel(i+x,j+y,color);
			}
		}
	}
}
void print_string(char str[],unsigned short x,unsigned short y,unsigned short color){
	char ch=str[0];
	int i=0,j=0;
	while(1){
		ch=str[i];
		if(ch=='\0')
		break;
		print_char(ch,x+j,y,color);
		j=j+6;
		i++;
	}
}
void clearBar(unsigned short xmin,unsigned short xmax,unsigned short ymin,unsigned short ymax,unsigned short color){
	unsigned short i,j;
	for(i=xmin;i<=xmax;i++){
		for(j=ymin;j<=ymax;j++){
			LCD_drawPixel(i,j,color);
		}
	}
}
void flowbar(short gx,short gy){
	int x_length,y_length;
	x_length=-(int)((float)gx/16500*64);
	y_length=-(int)((float)gy/16500*64);
	if(x_length>2){
		clearBar(62,x_length+64,62,67,MAGENTA);
		clearBar(x_length+65,132,62,67,BLUE);
		clearBar(0,61,62,67,BLUE);
	}
	if(x_length<-2){
		clearBar(x_length+64,67,62,67,MAGENTA);
		clearBar(0,x_length+63,62,67,BLUE);
		clearBar(68,132,62,67,BLUE);
	}
	if(x_length>=-2&&x_length<=2&&x_length!=0){
		clearBar(0,61,62,67,BLUE);
		clearBar(68,132,62,67,BLUE);
	}
	if(y_length>2){
		clearBar(62,67,62,y_length+64,MAGENTA);
		clearBar(62,67,y_length+65,132,BLUE);
		clearBar(62,67,0,61,BLUE);
	}
	if(y_length<-2){
		clearBar(62,67,y_length+64,67,MAGENTA);
		clearBar(62,67,0,y_length+63,BLUE);
		clearBar(62,67,68,132,BLUE);
	}
	if(y_length>=-2&&y_length<=2&&y_length!=0){
		clearBar(62,67,0,61,BLUE);
		clearBar(62,67,68,132,BLUE);
	}
}