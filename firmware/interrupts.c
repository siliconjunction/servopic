/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "set_functions.h"
#include "config.h"


#define LED_ON		15
#define LED_OFF		35


/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* Baseline devices don't have interrupts. Unfortunately the baseline detection 
 * macro is named _PIC12 */

#ifndef _PIC12

void delay_us(volatile unsigned char us_count);
void delay_256us(unsigned char count);
void setRgbLed();

extern char ledR, ledG, ledB;

int MAX_SPEED = 4;
int STEPS_IN_RAMP = 8;
int RAMP_STEP_SIZE = 1;

int ledTick = 0;
int startPos = 0;
int endPos = 0;
int currPos = 0;
char fwdFlag = 1;
int futurePos = 0;
int futureDuration = 0;

int confCentre = 0;
int confHigh = 99;
int confLow = -99;

unsigned char motorNo = 2;
unsigned char progMode = 0;

unsigned int usTotalDelay;
unsigned char usDelay;
unsigned char us256Delay;

void interrupt isr(void)
{
    if (PIE1bits.TMR2IE && PIR1bits.TMR2IF) {   
		PIR1bits.TMR2IF = 0;
		if (currPos != endPos) {
			if (fwdFlag) {
				if (currPos - startPos < STEPS_IN_RAMP) {
					currPos += RAMP_STEP_SIZE;
				}
				else if (endPos - currPos < STEPS_IN_RAMP) {
					currPos += RAMP_STEP_SIZE;
				}
				else {
					currPos += MAX_SPEED;
				}
				if (currPos > endPos) {
					currPos = endPos;
				}
			}
			else {
				if (-currPos + startPos < STEPS_IN_RAMP) {
					currPos -= RAMP_STEP_SIZE;
				}
				else if (-endPos + currPos < STEPS_IN_RAMP) {
					currPos -= RAMP_STEP_SIZE;
				}
				else {
					currPos -= MAX_SPEED;
				}
				if (currPos < endPos) {
					currPos = endPos;
				}
			}
		}
		usTotalDelay = (unsigned int)(10 * (currPos + 100) + 500);
		usDelay = usTotalDelay % 256;
		us256Delay = usTotalDelay / 256;
		O_SERVO = 1;
		/*
		 * For a pulse from 0.5ms to 2.5ms, values of 18 for initial delay and 
		 * 6 x NOPs for the loop work best. Centering is a bit off, implying some 
		 * tuning parameters may be required for centre and extreme limits.
		 */
		delay_us(usDelay);
		delay_256us(us256Delay);
		O_SERVO = 0;

		if (++ledTick > LED_ON + LED_OFF) {
			ledTick = 0;
		}
		//O_LED = (ledTick < LED_ON);

		if (ledTick < LED_ON) {
			ledR = 0;
			ledG = 1;
			ledB = 2;
		}
		else {
			ledR = (progMode ? 3 : 0);
			ledG = 0;
			ledB = 4;
		}
		setRgbLed();
		progMode = (I_PROG == 0);
	}
}

void setPos(int pos) {
	startPos = endPos;
	currPos = startPos;
	endPos = pos + confCentre;
	if (endPos > confHigh + confCentre) {
		endPos = confHigh + confCentre;
	}
	if (endPos < confLow + confCentre) {
		endPos = confLow + confCentre;
	}
	fwdFlag = (unsigned char)(endPos > startPos);
}

int getPos() {
	return currPos - confCentre;
}

void setFuturePos(int pos) {
	futurePos = pos;
}

void setFutureDur(int dur) {
	futureDuration = dur;
}

void moveToFuturePos() {
	setPos(futurePos);
}

void setConfigCen(int pos, unsigned char flag) {
	confCentre = pos;
	if (flag) {
		eeprom_write(ADDR_CENTRE, toUchar(confCentre));
	}
}

void setConfigHigh(int pos, unsigned char flag) {
	confHigh = pos;
	if (flag) {
		eeprom_write(ADDR_HIGH, toUchar(confHigh));
	}
}

void setConfigLow(int pos, unsigned char flag) {
	confLow = pos;
	if (flag) {
		eeprom_write(ADDR_LOW, toUchar(confLow));
	}
}

int getConfigCen() {
	return confCentre;
}

int getConfigHigh() {
	return confHigh;
}

int getConfigLow() {
	return confLow;
}

void setMotorNo(unsigned char num, unsigned char flag) {
	motorNo = num;
	if (flag) {
		eeprom_write(ADDR_MOTOR, toUchar(motorNo));
	}
}

unsigned char getMotorNo() {
	return motorNo;
}

unsigned char getProgMode() {
	return progMode;
}

unsigned char toUchar(int pos) {
	if (pos < 0) {
		return (unsigned char)(0x80 | (0x7f & -pos));
	}
	else {
		return (unsigned char)(0x7f & pos);
	}
}

void delay_256us(unsigned char count) {
	while (count-- > 0) {
		delay_us(255);
	}
}

void delay_us(volatile unsigned char us_count)
{   //input value will be in W. "Volatile" attribute prevents an "unused" warning.
#asm
dlyloop:
        addlw   0xFF            ;1 clk  -1
		nop
		nop
		nop
		nop
        btfsc   STATUS,0        ;1 clk   will be clear if W was smaller than 2
        goto    dlyloop         ;2 clks
#endasm
}


void setRgbLed() {
	FSR0L = ledG;
	#asm
		MOVLB	0x2
		BSF		PORTA, 4
		BTFSS	FSR0L, 7
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 6
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 5
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 4
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 3
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 2
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 1
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 0
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
	#endasm
	FSR0L = ledR;
	#asm
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 7
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 6
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 5
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 4
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 3
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 2
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 1
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 0
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
	#endasm
	FSR0L = ledB;
	#asm
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 7
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 6
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 5
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 4
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 3
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 2
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 1
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
		BSF		PORTA, 4
		BTFSS	FSR0L, 0
		BCF		PORTA, 4
		NOP
		NOP
		BCF		PORTA, 4
		BCF		PORTA, 4
		;
	#endasm
	#asm
		;
		movlw	100
		dlyloop2:
        addlw   0xFF            ;1 clk  -1
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
        btfsc   STATUS,0        ;1 clk   will be clear if W was smaller than 2
        goto    dlyloop2         ;2 clks

	#endasm
}
#endif


