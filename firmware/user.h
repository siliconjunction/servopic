/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#define O_LED       LATAbits.LATA4	// Pin 3 RA4 - LED
#define O_SERVO		LATAbits.LATA5	// Pin 2 RA5 - Servo
#define I_PROG		PORTAbits.RA3	// Pin 4 RA3 - Prog input

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
