cmake_minimum_required(VERSION 3.24)

option(USE_f407Port "使用F407移植" OFF)
option(USE_Legacy "使用遗留移植" OFF)
option(USE_easylogger "使用easylogger" OFF)
option(USE_vofa "使用vofa" OFF)

set(USE_f407Port ON)
set(USE_easylogger ON)
set(USE_vofa ON)
#set(USE_Legacy ON)
set(
    OSLib_Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/
)
# root dir
set(
    OSLib_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/oslib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/oslib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/oslib_config.h

)
message(DEBUG "OSLibSRC: ${OSLib_Src}")

# port
set(
    OSLib_port
)
if (USE_f407Port)
    set(
        OSLib_port
        ${OSLib_port}
        ${CMAKE_CURRENT_SOURCE_DIR}/port/stm32f407zg/oslib_can_port.c
        ${CMAKE_CURRENT_SOURCE_DIR}/port/stm32f407zg/oslib_config_port.h
        ${CMAKE_CURRENT_SOURCE_DIR}/port/stm32f407zg/oslib_uart_port.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/app/vofaDataDriver.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/app/vofaDataDriver.h
    )
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/port/stm32f407zg/)
    set(
        OSLib_Inc
        ${OSLib_Inc}
        ${CMAKE_CURRENT_SOURCE_DIR}/port/stm32f407zg/
    )
endif ()

if (USE_Legacy)
    set(
        OSLib_port
        ${OSLib_port}
        ${CMAKE_CURRENT_SOURCE_DIR}/port/legacy/can_func.c
        ${CMAKE_CURRENT_SOURCE_DIR}/port/legacy/cmd_func.c
        ${CMAKE_CURRENT_SOURCE_DIR}/port/legacy/oslib_can_port.c
        ${CMAKE_CURRENT_SOURCE_DIR}/port/legacy/oslib_config_port.h.h
        ${CMAKE_CURRENT_SOURCE_DIR}/port/legacy/oslib_uart_port.c
    )
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/port/legacy/ )
endif ()

message(DEBUG "OSLib_port: ${OSLib_port}")

# can
set(
    OSLib_can
    ${CMAKE_CURRENT_SOURCE_DIR}/can/oslib_can.c
    ${CMAKE_CURRENT_SOURCE_DIR}/can/oslib_can.h
    ${CMAKE_CURRENT_SOURCE_DIR}/can/oslib_can_dispatch.c
    ${CMAKE_CURRENT_SOURCE_DIR}/can/oslib_can_dispatch.h
#   ${CMAKE_CURRENT_SOURCE_DIR}/can/oslib_can_example.c
#   ${CMAKE_CURRENT_SOURCE_DIR}/can/oslib_can_example.h

)
set(
    OSLib_Inc
    ${OSLib_Inc}
    ${CMAKE_CURRENT_SOURCE_DIR}/can/
)
message(DEBUG "OSLib_can: ${OSLib_can}")

# debug
set(
    OSLib_Debug
    ${CMAKE_CURRENT_SOURCE_DIR}/debug/oslib_debug.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debug/SEGGER_RTT.c
    ${CMAKE_CURRENT_SOURCE_DIR}/debug/SEGGER_RTT.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debug/SEGGER_RTT_ASM_ARMv7M.S
    ${CMAKE_CURRENT_SOURCE_DIR}/debug/SEGGER_RTT_Conf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debug/SEGGER_RTT_printf.c
)
set(
    OSLib_Inc
    ${OSLib_Inc}
    ${CMAKE_CURRENT_SOURCE_DIR}/debug/
)
message(DEBUG "OSLib_Debug: ${OSLib_Debug}")

# uart
set(
    OSLib_uart
    ${CMAKE_CURRENT_SOURCE_DIR}/uart/oslib_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/uart/oslib_uart.h
    ${CMAKE_CURRENT_SOURCE_DIR}/uart/oslib_uart_cmd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/uart/oslib_uart_cmd.h

)
set(
    OSLib_Inc
    ${OSLib_Inc}
    ${CMAKE_CURRENT_SOURCE_DIR}/uart/
)
# utils
message(DEBUG "OSLib_uart: ${OSLib_uart}")
set(
    OSLib_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/oslib_hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/oslib_hash.h

)
set(
    OSLib_Inc
    ${OSLib_Inc}
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/
)
message(DEBUG "OSLib_utils: ${OSLib_utils} ")

#components
set(OSLib_components)

if (USE_easylogger)
    set(
        OSLib_components
        ${OSLib_components}
        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/inc/elog.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/inc/elog_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/port/elog_port.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/port/elog_port.c
        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/port/elog_cmd.c
        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/port/elog_cmd.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/src/elog.c
        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/src/elog_async.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/src/elog_buf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/src/elog_utils.c
    )
    set(
        OSLib_Inc
        ${OSLib_Inc}
        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/
        ${CMAKE_CURRENT_SOURCE_DIR}/components/easylogger/inc/
    )

endif ()

if (USE_vofa)
    set(
        OSLib_components
        ${OSLib_components}
        ${CMAKE_CURRENT_SOURCE_DIR}/components/vofaDataDriver/vofaDataDriver.c
        ${CMAKE_CURRENT_SOURCE_DIR}/components/vofaDataDriver/vofaDataDriver.h

    )
    set(
        OSLib_Inc
        ${OSLib_Inc}
        ${CMAKE_CURRENT_SOURCE_DIR}/components/vofaDataDriver/
    )

endif ()


message(DEBUG "OSLib_components: ${OSLib_components} ")

set(
    OSLib_example
    ${CMAKE_CURRENT_SOURCE_DIR}/example/can_func.c
    ${CMAKE_CURRENT_SOURCE_DIR}/example/cmd_func.c

)
set(
    OSLib_Inc
    ${OSLib_Inc}
    ${CMAKE_CURRENT_SOURCE_DIR}/example/
)
message(DEBUG "OSLib_utils: ${OSLib_utils} ")
list(APPEND OSLib_Src
    ${OSLib_port} ${OSLib_can} ${OSLib_uart}
    ${OSLib_utils} ${OSLib_Debug} ${OSLib_components} ${OSLib_example})
set(OSLib_IncDir
    ${OSLib_Inc}
    PARENT_SCOPE
)
include_directories(
    ${OSLib_IncDir}
)
set(OSLib ${OSLib_Src} PARENT_SCOPE)