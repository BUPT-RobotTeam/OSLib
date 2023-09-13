/*!
 * @author LiNY2 
 * @date 2023/04/07 0007
 */


#ifndef ELOG_APP_H
#define ELOG_APP_H

#if defined(OSLIB_UART_MODULE_ENABLED) &&  defined(OSLIB_LOG_MODULE_ENABLED) && defined(USE_OSLIB_UART_CLI)
void OSLIB_UART_CLI_AddElogCMD(OSLIB_UART_Handle_t * uart_handle);
#endif

#endif//ELOG_APP_H
