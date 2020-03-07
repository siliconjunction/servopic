#include "parser.h";

// State Machine states
typedef enum _SM_CMD {
	STATE_,	// 
	STATE_A,	// A
	STATE_AT,	// AT
	STATE_ATM,	// ATM
	STATE_ATMP,	// ATMP
	STATE_ATM9,	// ATM9
	STATE_ATM9P,	// ATM9P
	STATE_ATM9P_S,	// ATM9P=
	STATE_ATM9P_Ss,	// ATM9P=+
	STATE_ATM9P_Ss9,	// ATM9P=+9
	STATE_ATM9P_Ss99,	// ATM9P=+99
	STATE_ATM9P_U,	// ATM9P?
	STATE_ATM9C,	// ATM9C
	STATE_ATM9C_S,	// ATM9C=
	STATE_ATM9C_Ss,	// ATM9C=+
	STATE_ATM9C_Ss9,	// ATM9C=+9
	STATE_ATM9C_Ss99,	// ATM9C=+99
	STATE_ATM9C_U,	// ATM9C?
	STATE_ATM9T,	// ATM9T
	STATE_ATM9T_S,	// ATM9T=
	STATE_ATM9T_S9,	// ATM9T=9
	STATE_ATM9T_S99,	// ATM9T=99
	STATE_ATM9V,	// ATM9V
	STATE_ATM9F,	// ATM9F
	STATE_ATM9F_S,	// ATM9F=
	STATE_ATM9F_Ss,	// ATM9F=+
	STATE_ATM9F_Ss9,	// ATM9F=+9
	STATE_ATM9F_Ss99,	// ATM9F=+99
	STATE_ATM9H,	// ATM9H
	STATE_ATM9H_S,	// ATM9H=
	STATE_ATM9H_Ss,	// ATM9H=+
	STATE_ATM9H_Ss9,	// ATM9H=+9
	STATE_ATM9H_Ss99,	// ATM9H=+99
	STATE_ATM9H_U,	// ATM9H?
	STATE_ATM9L,	// ATM9L
	STATE_ATM9L_S,	// ATM9L=
	STATE_ATM9L_Ss,	// ATM9L=+
	STATE_ATM9L_Ss9,	// ATM9L=+9
	STATE_ATM9L_Ss99,	// ATM9L=+99
	STATE_ATM9L_U,	// ATM9L?
	STATE_ATM_S,	// ATM=
	STATE_ATM_S9,	// ATM=9
	STATE_ATM_U,	// ATM?
	STATE_ERROR
};

enum _SM_CMD state;



int p1 = 0;
int p2 = 0;
int mult = 10;
int unit = 1;

void processChar(char ch) {
	switch(state) {
		case STATE_:
			if (0) {
			}
			else if (ch == 'A') {
				state = STATE_A;
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_A:
			if (0) {
			}
			else if (ch == 'T') {
				state = STATE_AT;
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_AT:
			if (0) {
			}
			else if (ch == 'M') {
				state = STATE_ATM;
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM:
			if (0) {
			}
			else if (ch == 'P') {
				state = STATE_ATMP;
			}
			else if (ch >= '0' && ch <= '9') {
				p1 = 0;
				unit = 1;
				state = STATE_ATM9;
				p1 *= mult;
				p1 += unit * (ch - '0');
			}
			else if (ch == '=') {
				state = STATE_ATM_S;
			}
			else if (ch == '?') {
				state = STATE_ATM_U;
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATMP:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				broadcastMoveToFuture();
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9:
			if (0) {
			}
			else if (ch == 'P') {
				state = STATE_ATM9P;
			}
			else if (ch == 'C') {
				state = STATE_ATM9C;
			}
			else if (ch == 'T') {
				state = STATE_ATM9T;
			}
			else if (ch == 'V') {
				state = STATE_ATM9V;
			}
			else if (ch == 'F') {
				state = STATE_ATM9F;
			}
			else if (ch == 'H') {
				state = STATE_ATM9H;
			}
			else if (ch == 'L') {
				state = STATE_ATM9L;
			}
			else if (ch == '\r') {
				state = STATE_;
				confirmMotor(p1);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9P:
			if (0) {
			}
			else if (ch == '=') {
				state = STATE_ATM9P_S;
			}
			else if (ch == '?') {
				state = STATE_ATM9P_U;
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9P_S:
			if (0) {
			}
			else if (ch == '+' || ch == '-') {
				state = STATE_ATM9P_Ss;
				if (ch == '+') {
					unit = 1;
				}
				else if (ch == '-') {
					unit = -1;
				}
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9P_Ss:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				p2 = 0;
				state = STATE_ATM9P_Ss9;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9P_Ss9:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				state = STATE_ATM9P_Ss99;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9P_Ss99:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				moveMotor(p1, p2);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9P_U:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				getCurrentPosition(p1);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9C:
			if (0) {
			}
			else if (ch == '=') {
				state = STATE_ATM9C_S;
			}
			else if (ch == '?') {
				state = STATE_ATM9C_U;
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9C_S:
			if (0) {
			}
			else if (ch == '+' || ch == '-') {
				state = STATE_ATM9C_Ss;
				if (ch == '+') {
					unit = 1;
				}
				else if (ch == '-') {
					unit = -1;
				}
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9C_Ss:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				p2 = 0;
				state = STATE_ATM9C_Ss9;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9C_Ss9:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				state = STATE_ATM9C_Ss99;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9C_Ss99:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				setConfigCentreOffset(p1, p2);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9C_U:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				getConfigCentreOffset(p1);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9T:
			if (0) {
			}
			else if (ch == '=') {
				state = STATE_ATM9T_S;
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9T_S:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				p2 = 0;
				unit = 1;
				state = STATE_ATM9T_S9;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9T_S9:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				state = STATE_ATM9T_S99;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9T_S99:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				setFutureDuration(p1, p2);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9V:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				getVersion(p1);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9F:
			if (0) {
			}
			else if (ch == '=') {
				state = STATE_ATM9F_S;
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9F_S:
			if (0) {
			}
			else if (ch == '+' || ch == '-') {
				state = STATE_ATM9F_Ss;
				if (ch == '+') {
					unit = 1;
				}
				else if (ch == '-') {
					unit = -1;
				}
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9F_Ss:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				p2 = 0;
				state = STATE_ATM9F_Ss9;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9F_Ss9:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				state = STATE_ATM9F_Ss99;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9F_Ss99:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				setFuturePosition(p1, p2);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9H:
			if (0) {
			}
			else if (ch == '=') {
				state = STATE_ATM9H_S;
			}
			else if (ch == '?') {
				state = STATE_ATM9H_U;
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9H_S:
			if (0) {
			}
			else if (ch == '+' || ch == '-') {
				state = STATE_ATM9H_Ss;
				if (ch == '+') {
					unit = 1;
				}
				else if (ch == '-') {
					unit = -1;
				}
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9H_Ss:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				p2 = 0;
				state = STATE_ATM9H_Ss9;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9H_Ss9:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				state = STATE_ATM9H_Ss99;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9H_Ss99:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				setConfigLimitHigh(p1, p2);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9H_U:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				getConfigLimitHigh(p1);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9L:
			if (0) {
			}
			else if (ch == '=') {
				state = STATE_ATM9L_S;
			}
			else if (ch == '?') {
				state = STATE_ATM9L_U;
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9L_S:
			if (0) {
			}
			else if (ch == '+' || ch == '-') {
				state = STATE_ATM9L_Ss;
				if (ch == '+') {
					unit = 1;
				}
				else if (ch == '-') {
					unit = -1;
				}
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9L_Ss:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				p2 = 0;
				state = STATE_ATM9L_Ss9;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9L_Ss9:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				state = STATE_ATM9L_Ss99;
				p2 *= mult;
				p2 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9L_Ss99:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				setConfigLimitLow(p1, p2);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM9L_U:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				getConfigLimitLow(p1);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM_S:
			if (0) {
			}
			else if (ch >= '0' && ch <= '9') {
				p1 = 0;
				unit = 1;
				state = STATE_ATM_S9;
				p1 *= mult;
				p1 += unit * (ch - '0');
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM_S9:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				setAddressMode(p1);
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ATM_U:
			if (0) {
			}
			else if (ch == '\r') {
				state = STATE_;
				isSetAddressMode();
			}
			else {
				state = STATE_ERROR;
			}
			break;
		case STATE_ERROR:
			if (ch == '\r') {
				atError();
				state = STATE_;
			}
			break;
	}
}


