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
int main(){
	__builtin_disable_interrupts();
	__builtin_mtc0(_CP0_CONFIG,_CP0_CONFIG_SELECT,0xa4210583);
	BMXCONbits.BMXWSDRM=0;
	INTCONbits.MVEC=1;
	DDPCONbits.JTAGEN=0;
	TRISAbits.TRISA4=0;
	TRISBbits.TRISB4=1;
	LATAbits.LATA4=1;
	__builtin_enable_interrupts();
	while(1){
		_CP0_SET_COUNT(0);
		while(_CP0_GET_COUNT()<12000){;}
		LATAbits.LATA4=!LATAbits.LATA4;
		while(!PORTBbits.RB4){;}
	}
	return 0;
}