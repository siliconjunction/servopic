/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "set_functions.h"
#include "config.h"



/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
void delay_us(volatile unsigned char us_count);
void processChar(char ch);
void atError();

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

unsigned char ledR = 20;
unsigned char ledG = 10;
unsigned char ledB = 30;


int toSint(unsigned char c) {
	if (c & 0x80) {
		return -((int)(0x7f & c));
	}
	else {
		return (int)c;
	}
}

void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

	ANSELA = 0;

	// LED pin
	TRISAbits.TRISA4 = 0;

	// Tx pin
	TRISAbits.TRISA0 = 1;
	// Rx pin
	TRISAbits.TRISA1 = 1;
	// Servo pin
	TRISAbits.TRISA5 = 0;	

	OSCCON = 0xf0;
                            //1xxx xxxx - PLL On
                            ////x110 1xxx - 4 MHz
							//x111 0xxx - 8/32 MHz
							//xxxx xx00 - for 32 mhz
                            //xxxx xx1x - internal oscillator block

	T2CON = 0x7f;			//xxxx xx11 - Prescaler = 64
							//xxxx x1xx - Tmr2 on
							//x111 1xxx - Postscaler = 16 (0=1, 1=2 ... 11=12)
	// Crystal is 32MHz, prescaler set to 64, postscaler set to 16
	// Clock = 4 / 32MHz = 0.125us
	// Timer period = 0.125us * 64 * 16 = 128us
	// Interrupt period = 20000us i.e. 20000/128 = 156
	PR2 = 156;
   
  	PIE1bits.TMR2IE = 1;	// Enable Timer2 interrupt

	ADCON1 = 0;
	OPTION_REGbits.nWPUEN = 0;	// enable weak pullups
	WPUAbits.WPUA3 = 1;			// Enable weak-pullup for RA3

    TXSTAbits.SYNC = 0;     // 
	TXSTAbits.BRGH = 0;
	BAUDCONbits.BRG16 = 0;
	SPBRG = 25;             // Set baud rate as 19.2K baud with 32 MHz clock
    TXSTAbits.TXEN = 1;     // Enable transmission
    TXSTAbits.TX9 = 0;      // 8-bit transmission
    RCSTAbits.RX9 = 0;        // 8-bit reception
    RCSTAbits.CREN = 1;       // Enable reception
    RCSTAbits.SPEN = 1;       // Enable serial port

	unsigned char c;
	c = eeprom_read(ADDR_SIGN);
	if (c == SIGNATURE) {
		c = eeprom_read(ADDR_CENTRE);
		setConfigCen(toSint(c), 0);
		c = eeprom_read(ADDR_HIGH);
		setConfigHigh(toSint(c), 0);
		c = eeprom_read(ADDR_LOW);
		setConfigLow(toSint(c), 0);
		c = eeprom_read(ADDR_MOTOR);
		setMotorNo(c, 0);
	}
	else {
		eeprom_write(ADDR_SIGN, toUchar(SIGNATURE));
		eeprom_write(ADDR_HIGH, toUchar(getConfigHigh()));
		eeprom_write(ADDR_LOW, toUchar(getConfigLow()));
		eeprom_write(ADDR_CENTRE, toUchar(getConfigCen()));
		eeprom_write(ADDR_MOTOR, getMotorNo());
	}

    //Global interrupt enable
	INTCONbits.PEIE = 1;		// Enable Peripheral interrupts (TMR2, ....)
	INTCONbits.GIE = 1;			// Global interrupt enable

    while(1)
    {
		if (PIR1bits.RCIF != 0) {
			c = RCREG;
			TXREG = c;
			while (PIR1bits.TXIF == 0);
			processChar(c);
			/*
			if (c < 'A' || c > 'V') {
				while (PIR1bits.TXIF == 0);
				TXREG = 'x';
			}
			else {
				setPos((c - 'K') * 10);
				while (PIR1bits.TXIF == 0);
				TXREG = c ^ 0x20;
			}
			*/
		}
    }
}
