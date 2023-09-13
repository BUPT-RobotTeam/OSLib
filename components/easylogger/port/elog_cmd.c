/*!
 * @author LiNY2 
 * @date 2023/04/07 0007
 */



#include "oslib_config.h"
#if defined(OSLIB_UART_MODULE_ENABLED) &&  defined(OSLIB_LOG_MODULE_ENABLED) && defined(USE_OSLIB_UART_CLI)
#include "oslib_uart.h"
#include "oslib_uart_cmd.h"
#include "stdlib.h"
#include "string.h"
#include "elog_cmd.h"
#include "ctype.h"
#include "components/easylogger/inc/elog.h"
/**
 * 使能日志输出
 * @param uart_handle
 * @param argc
 * @param argv
 * @note log_en argv[1]:使能开关,输入 0/f(alse)/F(alse) 关闭，输入 1/t(rue)/T(rue)
 */
static void Command_Log_OutPutEnable(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[])
{
    if(argv[1][0] == '0' || argv[1][0] == 'f'||argv[1][0] == 'F')
    {
        elog_set_output_enabled(false);
    }
    else if(argv[1][0] == '1' || argv[1][0] == 't'||argv[1][0] == 'T')
    {
        elog_set_output_enabled(true);
    }
    else{
        elog_error("CMDLogOutPutEn","Please set 0/false/1/true");
    }
}
/**
 * 使能日志彩色字符
 * @param uart_handle
 * @param argc
 * @param argv
 * @note  log_colen argv[1]:使能开关,输入 0/f(alse)/F(alse) 关闭，输入 1/t(rue)/T(rue)
 */
static void Command_Log_ColorEnable(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[])
{
    if(argv[1][0] == '0' || argv[1][0] == 'f'||argv[1][0] == 'F')
    {
        elog_set_text_color_enabled(false);
    }
    else if(argv[1][0] == '1' || argv[1][0] == 't'||argv[1][0] == 'T')
    {
        elog_set_text_color_enabled(true);
    }
    else{
        elog_error("CMDLogColorEn","Please set 0/false/1/true");
    }
}

/**
 * 设置全局日志过滤器级别
 * @param uart_handle
 * @param argc
 * @param argv
 * @note log_filt argv[1]:LVL;LVL是0~5数字，或者是a(ssert)/e(rror)/w(arn)/i(nfo)/d(ebug)/v(erbose)/s(lient)，不区分大小写。
 */
static void Command_Log_SetFiltLVL(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[])
{
    if(isdigit(argv[1][0]))
    {
        uint8_t lvl = atoi(argv[1]);
        if( lvl < ELOG_LVL_TOTAL_NUM )
        {
            elog_set_filter_lvl(lvl);
        }
        else
        {
            elog_error("CMDLogSetFiltLVL","No LVL Named:%s",argv[1]);
        }
        return ;
    }

    switch (tolower(argv[1][0]))
    {
        case 'a':
        {elog_set_filter_lvl(ELOG_LVL_ASSERT);break ;}
        case 'e':
        {elog_set_filter_lvl(ELOG_LVL_ERROR);break ;}
        case 'w':
        {elog_set_filter_lvl(ELOG_LVL_WARN);break ;}
        case 'i':
        {elog_set_filter_lvl(ELOG_LVL_INFO);break ;}
        case 'd':
        {elog_set_filter_lvl(ELOG_LVL_DEBUG);break ;}
        case 'v':
        {elog_set_filter_lvl(ELOG_LVL_VERBOSE);break ;}
        case 's':
        {elog_set_filter_lvl(ELOG_FILTER_LVL_SILENT);break ;}
        default:{
            elog_error("CMDLogSetFiltLVL","No LVL Named:%s",argv[1]);
        }
    }
}

/**
 * 设置模块过滤器级别
 * @param uart_handle
 * @param argc
 * @param argv
 * @note log_filt argv[1]:tag(会区分大小写) argv[2]:LVL;LVL是0~5数字，或者是a(ssert)/e(rror)/w(arn)/i(nfo)/d(ebug)/v(erbose)/s(lient)，不区分大小写。
 */
static void Command_Log_SetTagFiltLVL(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[])
{
    if(isdigit(argv[2][0]))
    {
        uint8_t lvl = atoi(argv[2]);
        if( lvl < ELOG_LVL_TOTAL_NUM )
        {
            elog_set_filter_tag_lvl(argv[1],lvl);
        }
        else
        {
            elog_error("CMDLogSetTagFiltLVL","No LVL Named:%s",argv[2]);
        }
        return ;
    }

    switch (tolower(argv[2][0]))
    {
        case 'a':
        { elog_set_filter_tag_lvl(argv[1],ELOG_LVL_ASSERT);break ;}
        case 'e':
        { elog_set_filter_tag_lvl(argv[1],ELOG_LVL_ERROR);break ;}
        case 'w':
        { elog_set_filter_tag_lvl(argv[1],ELOG_LVL_WARN);break ;}
        case 'i':
        { elog_set_filter_tag_lvl(argv[1],ELOG_LVL_INFO);break ;}
        case 'd':
        { elog_set_filter_tag_lvl(argv[1],ELOG_LVL_DEBUG);break ;}
        case 'v':
        { elog_set_filter_tag_lvl(argv[1],ELOG_LVL_VERBOSE);break ;}
        case 's':
        { elog_set_filter_tag_lvl(argv[1],ELOG_FILTER_LVL_SILENT);break ;}
        default:{
            elog_error("CMDLogSetTagFiltLVL","No LVL Named:%s",argv[2]);break;
        }
    }
}
/**
 * OSLIB_UART_CLI_Init中被调用。由OSLIB_LOG_MODULE_ENABLED宏控制是否启用。
 * @param uart_handle 注册命令的串口
 */
void OSLIB_UART_CLI_AddElogCMD(OSLIB_UART_Handle_t * uart_handle)
{
    OSLIB_UART_CLI_AddCommand(uart_handle, "log_en",
                              "Enable Elog Output,log_enable 0/f(alse)/F(alse)/1/t(rue)/T(rue)",
                              Command_Log_OutPutEnable);
    OSLIB_UART_CLI_AddCommand(uart_handle, "log_colen",
                              "Enable Elog color Output,log_colen 0/f(alse)/F(alse)/1/t(rue)/T(rue)",
                              Command_Log_ColorEnable);
    OSLIB_UART_CLI_AddCommand(uart_handle, "log_filt",
                              "Set Elog global LVL filter,log_filt LVL(number0~5/a/e/w/i/d/v/s)",
                              Command_Log_SetFiltLVL);
    OSLIB_UART_CLI_AddCommand(uart_handle, "log_tagfilt",
                              "Set Elog tag LVL filter,log_tagfilt tag LVL(number0~5/a/e/w/i/d/v/s)",
                              Command_Log_SetTagFiltLVL);
}

#endif