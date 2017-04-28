#ifndef I2C_H
#define I2C_H
void i2c_master_setup(void);
void i2c_master_start(void);
void i2c_master_restart(void);
void i2c_master_send(char);
char i2c_master_recv(void);
void i2c_master_ack(int);
void i2c_master_stop(void);
#endif