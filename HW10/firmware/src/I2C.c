#include"I2C.h"
#include<xc.h>
#include<sys/attribs.h>
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
void i2c_master_send(char byte){
	I2C2TRN=byte;
	while(I2C2STATbits.TRSTAT){;}
	if(I2C2STATbits.ACKSTAT){
		LATAbits.LATA4=1;
	}
}
char i2c_master_recv(void){
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