/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                            EXAMPLE CODE
*
* Filename : app_cfg.h
*********************************************************************************************************
*/

#ifndef  _APP_CFG_H_
#define  _APP_CFG_H_


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>


/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_CFG_STARTUP_TASK_PRIO          3u

#define  OS_TASK_TMR_PRIO                  (OS_LOWEST_PRIO - 2u)


/*
*********************************************************************************************************
*                                          TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_CFG_STARTUP_TASK_STK_SIZE    128u


/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                    0u
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                   1u
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                    2u
#endif

#define  APP_TRACE_LEVEL                   TRACE_LEVEL_OFF
#define  APP_TRACE                         printf

#define  APP_TRACE_INFO(x)    ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)     ((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_TRACE x) : (void)0)


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif        
                                                  /* End of module include. */

// 不受内核管理的中断等级，使用 BOUNDARY 寄存器设置任务切换时的中断屏蔽
#ifndef CPU_CFG_KA_IPL_BOUNDARY
#define CPU_CFG_KA_IPL_BOUNDARY 4u
#endif

// 中断优先级寄存器可用位数，ARM 官方为 8bit 由厂家裁剪
#ifndef CPU_CFG_NVIC_PRIO_BITS
#define CPU_CFG_NVIC_PRIO_BITS 4u
#endif
