	
#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdlib.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "parser.h"
#include "set_functions.h"


#define true	((unsigned char)1)


void setOK(void);
void setError(void);
void setVer(void);
void setInt(int i);
void putsSerial(char *buf);


char outBuf[10];


void atError() {
	setError();
	putsSerial(outBuf);
}

void confirmMotor(int p1) {
	if (p1 == getMotorNo()) {
		setOK();
		putsSerial(outBuf);
	}
}

void getVersion(int p1) {
	if (p1 == getMotorNo()) {
		setVer();
		putsSerial(outBuf);
	}
}

void moveMotor(int p1, int p2) {
	if (p1 == getMotorNo()) {
		setPos(p2);
		setOK();
		putsSerial(outBuf);
	}
}

void setFuturePosition(int p1, int p2) {
	if (p1 == getMotorNo()) {
		setFuturePos(p2);
		setOK();
		putsSerial(outBuf);
	}
}

void setFutureDuration(int p1, int p2) {
	if (p1 == getMotorNo()) {
		setFutureDur(p2);
		setOK();
		putsSerial(outBuf);
	}
}

void broadcastMoveToFuture() {
	moveToFuturePos();
}

void getCurrentPosition(int p1) {
	if (p1 == getMotorNo()) {
		setInt(getPos());
		putsSerial(outBuf);
	}
}

void isSetAddressMode() {
	if (getProgMode()) {
		setOK();
		putsSerial(outBuf);
	}
}

void setAddressMode(int p1) {
	if (getProgMode()) {
		setMotorNo((unsigned char)p1, true);
		setOK();
		putsSerial(outBuf);
	}
}

void setConfigCentreOffset(int p1, int p2) {
	if (p1 == getMotorNo()) {
		setConfigCen(p2, true);
		setOK();
		putsSerial(outBuf);
	}
}

void setConfigLimitHigh(int p1, int p2) {
	if (p1 == getMotorNo()) {
		setConfigHigh(p2, true);
		setOK();
		putsSerial(outBuf);
	}
}

void setConfigLimitLow(int p1, int p2) {
	if (p1 == getMotorNo()) {
		setConfigLow(p2, true);
		setOK();
		putsSerial(outBuf);
	}
}

int getConfigCentreOffset(int p1) {
	if (p1 == getMotorNo()) {
		setInt(getConfigCen());
		putsSerial(outBuf);
	}
}

int getConfigLimitHigh(int p1) {
	if (p1 == getMotorNo()) {
		setInt(getConfigHigh());
		putsSerial(outBuf);
	}
}

int getConfigLimitLow(int p1) {
	if (p1 == getMotorNo()) {
		setInt(getConfigLow());
		putsSerial(outBuf);
	}
}


void setOK(void) {
	outBuf[0] = '\n';
	outBuf[1] = 'O';
	outBuf[2] = 'K';
	outBuf[3] = '\r';
	outBuf[4] = '\n';
	outBuf[5] = '\0';
}

void setError(void) {
	outBuf[0] = '\n';
	outBuf[1] = 'E';
	outBuf[2] = 'R';
	outBuf[3] = 'R';
	outBuf[4] = '\r';
	outBuf[5] = '\n';
	outBuf[6] = '\0';
}

void setVer(void) {
	outBuf[0] = '\n';
	outBuf[1] = 'V';
	outBuf[2] = ' ';
	outBuf[3] = '1';
	outBuf[4] = '.';
	outBuf[5] = '0';
	outBuf[6] = '\r';
	outBuf[7] = '\n';
	outBuf[8] = '\0';
}

void setInt(int i) {
	char *buf = outBuf;
	*(buf++) = '\n';
	if (i < 0) {
		*(buf++) = '-';
		i = -i;
	}
	if (i < 10) {
		*(buf++) = '0';
	}
	else {
		*(buf++) = '0' + (unsigned char)(i / 10);
	}
	*(buf++) = '0' + (unsigned char)(i % 10);
	*(buf++) = '\r';
	*(buf++) = '\n';
	*(buf++) = '\0';
}

void putsSerial(char *buf) {
	// TODO: Enable and disable Tx during transmission
	TRISAbits.TRISA0 = 0;

	while (*buf) {
		while (PIR1bits.TXIF == 0);
		TXREG = *buf;
		buf++;
	}
	while (TXSTAbits.TRMT == 0);
	TRISAbits.TRISA0 = 1;
}
