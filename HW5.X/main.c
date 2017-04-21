#include<xc.h>
#include<sys/attribs.h>
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
void i2c_master_setup(void){
	I2C2BRG=233;
	I2C2CONbits.ON=1;
}
void i2c_master_start(void){
	I2C2CONbits.SEN=1;
	while(I2C2CONbits.SEN){;}
}
void i2c_master_restart(void){
	I2C2CONbits.RSEN=1;
	while(I2C2CONbits.RSEN){;}
}
void i2c_master_send(unsigned char byte){
	I2C2TRN=byte;
	while(I2C2STATbits.TRSTAT){;}
	if(I2C2STATbits.ACKSTAT){
		LATAbits.LATA4=1;
	}
}
unsigned char i2c_master_recv(void){
	I2C2CONbits.RCEN=1;
	while(!I2C2STATbits.RBF){;}
	return I2C2RCV;
}
void i2c_master_ack(int val){
	I2C2CONbits.ACKDT=val;
	I2C2CONbits.ACKEN=1;
	while(I2C2CONbits.ACKEN){;}
}
void i2c_master_stop(void){
	I2C2CONbits.PEN=1;
	while(I2C2CONbits.PEN){;}
}
void initExpander(){
	ANSELBbits.ANSB2=0;
	ANSELBbits.ANSB3=0;
	i2c_master_setup();
	i2c_master_start();
	i2c_master_send(64);
	i2c_master_send(0);
	i2c_master_send(240);
	i2c_master_stop();
}
void setExpander(char pin,char level){
	i2c_master_start();
	i2c_master_send(64);
	i2c_master_send(10);
	if(level==1)
	i2c_master_send(1<<pin);
	else if(level==0)
	i2c_master_send(0<<pin);
	i2c_master_stop();
}
char getExpander(){
	i2c_master_start();
	i2c_master_send(64);
	i2c_master_send(9);
	i2c_master_restart();
	i2c_master_send(65);
	char a=i2c_master_recv()>>7;
	i2c_master_ack(1);
	i2c_master_stop();
	return a;
}
int main(){
	TRISAbits.TRISA4=0;
	TRISBbits.TRISB4=1;
	LATAbits.LATA4=0;
	initExpander();
	while(1){
		char level=getExpander();
		setExpander(0,level);
	}
}