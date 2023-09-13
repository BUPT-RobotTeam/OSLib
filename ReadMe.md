# OSLIB

---

### 1.代码结构

```
├─can/
	├─oslib_can				// CAN发送接收管理
	├─oslib_can_dispatch	// CAN接收分发器
	├─oslib_can_example		// CAN接收任务范例
├─uart/
	├─oslib_uart			// UART发送接收管理
	├─oslib_uart_cmd		// UART命令行
├─utils/
	├─oslib_hash			// 通用哈希表
├─debug/
	├─oslib_debug			// 按调试等级区分输出内容
	├─SEGGER_RTT			// RTT调试
├─app/
	├─cmd_func				// [可修改]指定命令以及回调函数
	├─can_func				// [可修改]can消息接收处理
├─oslib						// OSLIB初始化相关
└─oslib_config				// [可修改]OSLIB功能裁剪
```

### 2.配置说明

##### 2.1 使用CubeMX进行配置

> OSLIB使用了HAL库, 推荐使用CubeMX进行配置。以下参数仅供参考。

- 基本配置
  - *System Core\RCC\Mode*
    - `HSE`: 自定
    - `LSE`: 自定
  - *System Core\SYS\Mode*: 
    - `Debug`: Serial Wire
    - `Timebase Source`: **TIM1**(这里可以选择任意一个定时器)
  - *..\Clock Configuration*: 
    - `HCLK(MHz)`: 自定
- FreeRTOS配置
  - *Middleware\FREERTOS\Mode*:
    - `Interface`: **CMSIS_V2**
  - *Middleware\FREERTOS\Configuration\Config parameters*:
    - `USE_STATS_FORMATTING_FUNCTIONS`: Enabled
- UART配置
  - *Connectivity\USART1\Mode*:
    - `Mode`: Asynchronous
  - *Connectivity\USART1\Configuration\NVIC Settings*:
    - `USART1 global interrupt`: ✔
  - *Connectivity\USART1\Configuration\DMA Settings*:
    - 点击`Add`添加 USART1_RX
      - `Mode`: **Circular**
    - 点击`Add`添加 USART1_TX
- CAN配置
  - *Connectivity\CAN1\Mode*:
    - `Master Mode`: ✔
  - *Connectivity\CAN1\Configuration\Parameter Settings*:
    - `Time Quanta in Bit Segment 2`: 4 Times
      - (如果使用f103此处为2 Times)
    - `Time Quanta in Bit Segment 1`: 9 Times
    - `Prescaler(for Time Quantum)`: 3
    - `Automatic Bus-Off Management`: Enable
    - `Transmit Fifo Priority`: Enable
  - *Connectivity\CAN1\Configuration\NVIC Settings*:
    - `CAN1 TX interrupts`: ✔
    - `CAN1 RX0 interrupts`: ✔
    - `CAN1 RX1 interrupts`: ✔

##### 2.2 使用前的准备

- 将所需要的OSLIB代码和头文件添加到工程中

  - 一个目录即为一个模块，其中头文件只需要添加oslib根目录即可
  - 需要注意的是: 
    - 使用串口命令行模块, 需要额外添加app目录下的cmd_func系列文件和utils目录下的oslib_hash系列文件
    - 使用CAN接收处理模块, 需要额外添加app目录下的can_func系列文件和utils目录下的oslib_hash系列文件

- 添加部分代码

  - 文件*stm32f4xx_it.c*t添加: (上下注释用来定位)

    ```c
    /* USER CODE BEGIN Includes */
    #include "oslib.h"
    /* USER CODE END Includes */
    ```

    函数`void USART1_IRQHandler(void)`中添加: (上下两行注释用来定位)

    ```c
      /* USER CODE BEGIN USART1_IRQn 0 */
      if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
        OSLIB_UART_RxIdleCallback(&huart1);
      else
      /* USER CODE END USART1_IRQn 0 */
    ```

  - 文件*freertos.c*添加: (上下注释用来定位)

    ```c
    /* Private includes ----------------------------------------------------------*/
    /* USER CODE BEGIN Includes */     
    #include "oslib.h"
    /* USER CODE END Includes */
    
    ```

    函数 `void MX_FREERTOS_Init(void)`中添加: (上下两行注释用来定位)

    ```c
      /* USER CODE BEGIN Init */
      OSLIB_Init();
      /* USER CODE END Init */
    ```
    
    > 注意: CubeMX版本不同生成的代码可能有差别, 如果MX_FREERTOS_Init()函数内调用了osKernelInitialize()函数, 请把OSLIB_Init()放到osKernelInitialize()函数之后.
  
- 在oslib_config.h中进行配置

  - OSLIB主要通过宏进行功能的选择, 宏的类型及其基本含义如下:

    - `xxx_ENABLED`: 允许使用某模块/外设. 只需要将该宏注释掉即可表示不使用该模块.
    - `USE_xxx`: 使用某功能. 主要针对OSLIB中各模块的子功能. 0或1表示是否使用功能.
    - `SELECT_xxx`: 选择. 要求从允许的多个值中选择一个值作为该宏的值.

  - 范例: 假设使用了CAN1, UART1, UART6. 将UART1作为命令行串口, 配置如下:

    ```c
    #define OSLIB_DEBUG_MODULE_ENABLED  // 调试输出功能
    #define OSLIB_UART_MODULE_ENABLED   // 串口功能
    #define OSLIB_CAN_MODULE_ENABLED    // CAN功能
    /*------------------------------------------------------*/
    #define UART1_ENABLED		// 使用UART1
    // #define UART2_ENABLED
    // #define UART3_ENABLED
    // #define UART4_ENABLED
    // #define UART5_ENABLED
    #define UART6_ENABLED
    
    #define CAN1_ENABLED		// 使用CAN1
    // #define CAN2_ENABLED
    /*------------------------------------------------------*/
    #define SELECT_DEBUG_LEVEL DEBUG     // 调试输出等级为DEBUG
    
    #define USE_OSLIB_UART_CLI 1        // 使用串口命令行
    #define SELECT_MAJOR_UART 1         // 主串口为1, 即命令行的串口为UART1
    
    #define USE_OSLIB_CAN_CALLBACK 1    // 允许使用回调方式处理CAN报文
    
    #define USE_OSLIB_CAN_EXAMPLE 1     // 使用CAN范例任务处理CAN报文
    ```
  
  > 注意: oslib_config.h中有上下两个保留区域, 其中的内容不允许被修改.

##### 2.3 特别注意

- 禁止在中断中调用信号量等待函数(比如`osSemaphoreAcquire()`)，以及调用该函数的函数(比如`OSLIB_Uart_Printf()`和`OSLIB_CAN_SendMessage()`)。
- 使用CubeMX生成的代码可能会颠倒DMA初始化`MX_DMA_Init()`和USART1初始化`MX_USART1_UART_Init()`, 需要手动调整。
- 使用较高版本(比如6.0.0)的CubeMX生成代码可能出现中断优先级默认为0的情况, 其他版本通常为5, 这时需要在NVIC配置中调低部分中断优先级。
- 当使用SRAM容量较小的芯片时, 可能出现默认的堆空间不足的情况, 需要在CubeMX里将FREERTOS选项中的堆空间调大。

### 3. 模块说明

##### 3.1 UART管理模块

- 模块目录: .\uart\

- 依赖模块: .\utils\oslib_hash

- 基本功能: ①多任务环境下避免串口发送冲突 ②缓存串口接收到的数据, 便于使用任务处理

  - 发送接口: (\*表示衍生的宏, 本质上依赖于其他接口函数)
    - `void OSLIB_Uart_Printf(huart, fmt, ...)` - **函数**, 通过指定串口发送格式化字符串
    - `void OSLIB_Uart_SendData(huart, msg, size)` - **函数**, 通过指定串口发送任意数据
    - \* `void uprintf(fmt, ...)` - **宏**, 通过主串口发送格式化字符串
    - \* `void uprintf_to(huart, fmt, ...)` - **宏**, 通过指定串口发送格式化字符串
  - DMA接收接口: (要求oslib_config.h中的宏`SELECT_UARTx_RXMODE`为0)
    - `osSemaphoreId_t UARTx_RxSema` - **信号量**, 对应串口x接收情况
    - `char UARTx_TaskBuffer[RxBufferSize]` - **缓冲区**, 得到信号量之后, 可以从缓冲区中取出数据进行处理
    - `int UARTx_TaskBufferSize` - **接收数据的长度**, 得到信号量之后，可以通过该变量得到此次接收数据的长度
  - IT接收接口: (要求oslib_config.h中的宏`SELECT_UARTx_RXMODE`为1)
    - `osMessageQueueId_t UARTx_RxQueue` - **消息队列**, 存储串口x收到的字符

- 子功能: 创建命令行处理任务, 解析并执行主串口的接收到的命令.

  - 功能裁剪: 宏`USE_OSLIB_UART_CLI`决定是否启用这一子功能, 宏`SELECT_MAJOR_UART`用于选择主串口, 即命令行所使用的串口. 
  - 命令格式: `<命令1> <参数表1>;<命令2> <参数表2>;<命令3> <参数表3>`
    - 参数表可以为空. 如果不为空, 各参数之间用空格分隔, 一条命令的参数不能超过10个.
    - 输入多条命令时, 两命令间用分号分割, 多条命令将会被依次执行. 一次输入的命令数不能超过5个, 第5条之后的命令不会被执行.

  - 基本命令:

    | 命令    | 说明                     | 命令格式示例       |
    | ------- | ------------------------ | ------------------ |
    | help    | 打印命令列表             | help               |
    | hello   | 打印"Hello SimpleLib"    | hello              |
    | echo    | 打印echo命令后面的参数   | echo param1 param2 |
    | cansend | 通过CAN1发送特定ID的报文 | cansend std 1FF    |
    | tasks   | 打印当前各任务的状态     | tasks              |

  - 自定义命令: .\app\cmd_func.c中记录了所有的命令及其回调函数, 用户可以自行增删命令.

    - 在Command_Init()中添加对OSLIB_UartCmd_AddCommand()函数的调用, 参数如下:
      - 命令: 字符串, 如"help"
      - 帮助: 字符串, 对命令进行一定的描述
      - 回调函数: 函数指针, 函数原型为`void (*)(int argc, char *argv[])`
        - `argc`的值为参数个数+1
        - `argv[]`为各参数的字符串 (其中`argv[0]`为命令的字符串)

    > 注意: 一条指令的执行时间不宜过长, 由于命令行是最高优先级任务, 执行时间过长会阻塞整个单片机的运行.

    > 提醒：添加命令的方式很可能会在后续版本中修改为其他形式

##### 3.2 CAN管理模块

- 模块目录: .\can\

- 依赖模块: .\utils\oslib_hash

- 基本功能: ①多任务环境下避免CAN发送冲突 ②过滤指定ID的报文, 并分流到不同任务进行处理

  - 发送接口: (*表示衍生出的宏, 本质上还是依赖于其他接口函数)

    - `void OSLIB_CAN_SendMessage(hcan, idtype, id, msg) ` - **函数**, 通过指定的CAN控制器发送指定ID的报文
    - \* `void can_send_msg(id, msg)` - **宏**, 通过CAN1发送指定标准帧ID的报文
    - \* `void can_ext_send_msg(id, msg)` - **宏**, 通过CAN1发送指定标准帧ID的报文

  - 接收接口: 当CAN控制器接收到报文时, 会根据用户配置的ID, 找到对应的消息队列, 将报文发送出去, 完成"派发"的工作。报文将会被消息队列对应的任务处理。

    - .\app\can_func.c文件中为每个可能使用到的CAN定义了数组CANx_NormList, 用户可以通过在数组中添加表项对CANx的派发逻辑进行设置.
      - 表项的格式为: `{<ID>, <ID类型>, <消息队列地址>, <回调函数>}`
        - `<ID>`是期望接收的报文ID, 理论上除了用户在两个数组内设置的ID以外的ID都会被筛选器筛选掉.
        - `<ID类型>`是对应的ID类型, 取值可以为`CAN_IDTYPE_STD`和`CAN_IDTYPE_EXT`
        - `<消息队列地址>`指向该ID对应的报文在派发过程中的目标消息队列
        - `<回调函数>`指向该ID对应的处理函数, 没有则为NULL, 详见下文"子功能"一节
      - 也可以利用`CANx_NormItem_Callback(id, idtype, callback)`和`CANx_NormItem_Queue(id, idtype, queue)`**宏**代替表项, 两种方式完全等价
    - 为兼容本杰明电调, .\app\can_func.c文件中还增加一个数组CANx_VescList, 其中ID应填入本杰明电调ID. 接收时, 派发器能够将ID中的5位状态码屏蔽掉再进行派发. 
    
    > 注意: 这里有个设计上的瑕疵, 对于非本杰明电调发出的**扩展帧ID**, 也会出现屏蔽掉5位状态码再派发的情况(比如: 0x1FF和0xFF可能都会派发给0xFF的队列). 在设计ID时需要考虑这一点.

- 子功能: 基于以上的接收逻辑提供一个使用回调方式处理CAN报文的子功能. 针对每个CAN都创建一个新任务, 任务中根据ID调用对应回调函数进行处理. 

  - 功能裁剪: 宏`USE_OSLIB_CAN_CALLBACK`决定是否启用这一子功能
  - 使用方式: 数组CANx_NormList中将`<消息队列地址>`的值取为NULL, 则派发器会自动将该ID关联到回调处理任务的队列上, 此时`<回调函数>`不允许为NULL. 
  - 回调函数: 回调函数的函数原型为`void (*)(CAN_ConnMessage *msg)`.
    - `msg`包含接收到的CAN报文ID以及数据帧

  > 注意: 本杰明电调发出的报文(记录在VescList中的ID)不支持该方式

- 附加功能: CAN范例任务, 将接收到的CAN报文ID直接打印出来, 通常只用于测试时使用

  - 功能裁剪: 宏`USE_OSLIB_CAN_EXAMPLE`决定是否启用这一附加功能
  - 接口: `ExampleCanTaskQueue` - **消息队列**, 与任务绑定的消息队列.

##### 3.3 调试输出模块

- 模块目录: .\debug\

- 基本功能: 采用Jlink提供的RTT功能进行分级调试输出

- RTT介绍: RTT方式一定程度上优于串口, 传输速度较快, 而且可以在中断中使用. 缺点是对下载器有要求, 必须采用Jlink下载器, 无线调试时不能使用, 而且必须使用SEGGER官方提供的RTT Viewer等工具显示调试信息.

- 基本功能: 

  - 功能配置: 宏`SELECT_DEBUG_LEVEL`用于选择调试输出等级, 调试输出等级由低到高依次有`OFF FATAL ERROR WARN INFO DEBUG ALL`  , 对应的输出内容由少到多.
  - 输出接口: 提供6种类型的调试输出
    - `void Fatal(fmt, ...)` - **宏**, 打印不可修复错误的相关信息, 此时系统不能继续运行
    - `void Error(fmt, ...)` - **宏**, 打印系统错误的错误信息, 此时通常不能完成目标动作
    - `void Warn(fmt, ...)` - **宏**, 打印轻微错误的错误信息, 此时不会影响目标动作的完成
    - `void Info(fmt, ...)` - **宏**, 打印需要注意的信息, 此时通常没有发生错误
    - `void Debug(fmt, ...)` - **宏**, 打印调试时需要打印的相关信息, 此时也未发生错误
    - `void UDebug(fmt, ...)`- 宏, 打印用户自定义内容, 便于调试。`UDebug`输出等级同`Fatal()`宏, 因此使用结束后需要删除, 以免对其他信息的查看造成干扰

  > 提醒: 这里以后*可能*会添加使用串口打印调试信息的方式.

### 4. 更新记录

| 版本   | 时间       | 内容                                           |
| ------ | ---------- | ---------------------------------------------- |
| v0.2.7 | 2021/11/16 | 串口DMA接收增加接收数据长度信息                |
| v0.2.6 | 2021/11/15 | 针对Keil的警告进行调整, 调整#include为相对路径 |
| v0.2.5 | 2021/07/22 | CAN1和CAN2采用不同的宏进行功能裁剪             |
| v0.2.4 | 2021/05/05 | 修复串口同时收发卡住的问题, 调试增加了UDebug宏 |
| v0.2.3 | 2021/04/08 | 支持中断方式的串口接收                         |
| v0.2.2 | 2021/03/26 | 完善了CAN的功能裁剪                            |
| v0.2.1 | 2021/01/07 | 串口增加了发送通用数据的函数                   |
| v0.2.0 | 2021/01/04 | CAN的管理方式有较大改动                        |
| ...    | ...        | ...                                            |
| v0.1.0 | 2020/11/29 | 初始项目, 包含UART和CAN的基本管理功能          |

