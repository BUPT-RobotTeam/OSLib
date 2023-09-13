/*!
 * @author LiNY2 
 * @date 2023/03/19 0019
 */


#ifndef ELOG_PORT_H_
#define ELOG_PORT_H_

#include "components/easylogger/inc/elog.h"
#include "cmsis_os2.h"



void OSLIB_Elog_Init(void);

void StartElogTask(void *argument);


#endif//ELOG_PORT_H_
