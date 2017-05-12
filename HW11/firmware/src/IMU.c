#include<xc.h>
#include"IMU.h"
#include"I2C.h"
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