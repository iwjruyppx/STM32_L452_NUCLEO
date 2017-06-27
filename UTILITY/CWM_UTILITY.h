
#ifndef __CWM_UTILITY_H
#define __CWM_UTILITY_H

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
/* Compiler includes. */
#include <intrinsics.h>

/*********************************************************/
/************//*TOP Config file*/
/*********************************************************/
#include "CWM_CONFIG.h"
/*********************************************************/
/************//*TOP Config file END*/
/*********************************************************/

/*FreeRTOS include*/
#include "cmsis_os.h"

/*********************************************************/
/************//*Standard UTILITY/STRUCTURE/ERROR CODE */
/*********************************************************/
#include "CWM_CMD.h"
#include "CWM_STRUCTURE.h"
#include "CWM_ERROR_CODE.h"
/*********************************************************/
/************//*Standard UTILITY/STRUCTURE/ERROR CODE */
/*********************************************************/

/*********************************************************/
/************//*MODULE*/
/*********************************************************/

/*Double Link List Module*/
#include "CWM_LINK_LIST_TEMPLATE.h"

/*GPS Module*/
#include "CWM_GPS_PASSER.h"

/*OLED screen driver SSD1306*/
#include "tm_stm32f4_ssd1306.h"

/*Screen fonts*/
#include "tm_stm32f4_fonts.h"

/*Circular Queue*/
#include "CwmQueue.h"
/*********************************************************/
/************//*MODULE END*/
/*********************************************************/

/*********************************************************/
/************//*Exported macro*/
/*********************************************************/
#define Error_Handler() while(1);
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/*********************************************************/
/************//*Exported macro END*/
/*********************************************************/
#endif /* __CWM_UTILITY_H */
