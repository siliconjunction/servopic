/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

void setPos(int pos);
int getPos(void);
void setFuturePos(int pos);
void setFutureDur(int dur);
void moveToFuturePos();
void setConfigCen(int pos, unsigned char flag);
void setConfigHigh(int pos, unsigned char flag);
void setConfigLow(int pos, unsigned char flag);
int getConfigCen();
int getConfigHigh();
int getConfigLow();
void setMotorNo(unsigned char num, unsigned char flag);
unsigned char getMotorNo();
unsigned char getProgMode();

#endif	/* XC_HEADER_TEMPLATE_H */

