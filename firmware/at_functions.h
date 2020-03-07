/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

void confirmMotor(int p1);
void getVersion(int p1);
void moveMotor(int p1, int p2);
void setFuturePosition(int p1, int p2);
void setFutureDuration(int p1, int p2);
void broadcastMoveToFuture();
void getCurrentPosition();
void isSetAddressMode();
void setAddressMode();
void setConfigCentreOffset(int p1, int p2);
void setConfigLimitHigh(int p1, int p2);
void setConfigLimitLow(int p1, int p2);
void atError();

void setOK(void);
void setError(void);
void setVer(void);
void putsSerial(char *buf);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

