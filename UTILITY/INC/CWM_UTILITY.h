
#ifndef __CWM_UTILITY_H
#define __CWM_UTILITY_H

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
/* Compiler includes. */
#include <intrinsics.h>

/*********************************************************/
/*********************************************************/
/************//*TOP Config file*/
/*********************************************************/
#include "CWM_CONFIG.h"
/*********************************************************/
/************//*TOP Config file END*/
/*********************************************************/
/*********************************************************/

/*FreeRTOS include*/
#include "cmsis_os.h"

/*********************************************************/
/*********************************************************/
/************//*Standard UTILITY/STRUCTURE/ERROR CODE */
/*********************************************************/
#include "CWM_CMD.h"
#include "CWM_STRUCTURE.h"
#include "CWM_ERROR_CODE.h"
#include "CWM_INTERNAL_CMD.h"

/*system periphreal message control center*/
#include "CWM_MSG_QUEUE.h"
/*********************************************************/
/************//*Standard UTILITY/STRUCTURE/ERROR CODE */
/*********************************************************/
/*********************************************************/

extern int64_t gTimestamp;

/*********************************************************/
/*********************************************************/
/************//*MODULE*/
/*********************************************************/
/*Double Link List Module*/
#include "CWM_LINK_LIST_TEMPLATE.h"
/*Log String Paser Module*/
#include "CWM_LOG_STRING_PASER_TEMPLATE.h"
/*Circular Queue*/
#include "CwmQueue.h"
/*Package Queue*/
#include "CWM_PACKAGE_QUEUE.h"

/*Screen fonts*/
#include "tm_stm32f4_fonts.h"
/*OLED screen driver SH1106*/
#include "tm_stm32f4_SH1106.h"
/*OLED screen driver SSD1306*/
#include "tm_stm32f4_ssd1306.h"
/*BLE HC-06 Module for slave*/
#include "MODULE_BT_HC_06_SLAVE.h"
/*GPS Module*/
#include "CWM_GPS_PASSER.h"
/*StepperMotor Module*/
#include "CWM_STEPPER_MOTOR.h"
/*********************************************************/
/************//*MODULE END*/
/*********************************************************/
/*********************************************************/

/*********************************************************/
/*********************************************************/
/************//*Exported macro*/
/*********************************************************/
#define Error_Handler() while(1);
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/*********************************************************/
/************//*Exported macro END*/
/*********************************************************/
/*********************************************************/
#endif /* __CWM_UTILITY_H */
