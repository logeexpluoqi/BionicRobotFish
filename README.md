# WIAS 仿生鱼控制系统底层设计

## 一、系统架构

### 1.1 硬件架构

![image-20210129210424808](README.assets/image-20210129210424808.png)

#### 1.1.1手工焊接初步测试版本引脚设置

- **OLED_0.96IIC**

| SCL  | SDA  |
| :--: | :--: |
| PB13 | PB14 |

- **TJA1050 CAN收发器 (CAN1)**

| CAN_TX | CAN_RX |
| :----: | :----: |
|  PA12  |  PA11  |

- **STM32系统板硬件资源**

| KEY_1 |  D2  |
| :---: | :--: |
|  PA0  | PA1  |

- **NRF24L01(SPI3 Port Remap)**

| IRQ  | MISO | MOSI | SCLK | CSN  |  CE  |
| :--: | :--: | :--: | :--: | :--: | :--: |
| PD0  | PC11 | PC12 | PC10 | PD1  | PA15 |

- **USART (USART1)**

|  TX  |  RX  |
| :--: | :--: |
| PA9  | PA10 |

#### 1.1.2 硬件设备参数

**TMotor 参数范围**

|        | 位置(rad) | 速度(rpm) | 转矩(n*m) |   Kp    |  Kd   |
| :----: | :-------: | :-------: | :-------: | :-----: | :---: |
| AK80-9 |   ±95.5   |    ±45    |    ±18    | 0 - 500 | 0 - 5 |
| AK10-9 |   ±95.5   |    ±45    |    ±54    | 0 - 500 | 0 - 5 |

其中AK10-9电机位置控制时，电机实际输出的角度时输入角度的1/10  (2021/01/24测试发现)，可通过将目标参数扩大10倍作为输入值来解决该问题。

### 1.2软件架构

- **单片机软件目录结构**

![image-20210126202152935](README.assets/image-20210126202152935.png)

软件共有12个一级文件夹，分别存放不同功能代码或文件，其中*"Obj"*, *"Prj"*, *"Doc"*分别存放：keil编译后文件、工程结构文件和软件相关说明。

-- *“System”* 文件夹下包含了有关系统运行的文件，其中*"timer_slice"*中的文件为本系统使用的程序架构---*时间片轮询法*；

-- *"Tasks"*文件夹下包含系统所运行的任务代码文件；

-- *"Communication"*文件夹下包含系统与外界通信的驱动文件，目前包括串口通信、I2C通信、CAN通信以及通信编解码函数；

-- *"Hardware"*文件夹下包含系统外部设备的驱动文件，包括：电机驱动、OLED显示屏驱动、按键驱动以及LED灯驱动；

-- *"Firmware"*文件夹下包含单片机底层文件库，该部分文件一般不能改动；

-- *"Main"*文件夹下包含系统初始化文件、系统模式配置文件、主函数文件和全局类型文件；

-- *"Control"* 文件夹下包含系统控制文件，目前没有添加，后期单片机上的控制方法实现文件都放于该文件夹下；

-- *"Test"*文件夹下包含一些系统设计过程中的一些测试代码文件.

#### 1.2.1 控制系统整体结构

![image-20210126191308565](README.assets/image-20210126191308565.png)

控制系统分为3个部分，分别是顶层由PC端控制，主要是实现复杂的控制算法； 中层由MCU实现控制，直接从机在传感器获得航行器的姿态信息，直接对航行器的姿态进行控制，使航行器处于稳定状态，并向上位机实时发送航行器的姿态以及其它状态信息；底层是电机的控制，使电机能够在规定时间以规定的状态到达规定的位置，目前电机上应用的使FOC控制方法。

上位机与单片机之间的通信采用串口通信，上位机将浮点型数据编码成字符型数据通过串口下发到单片机，单片机通过相应的解码函数进行解压后结合电机返回的信息以及机身传感器返回的信息对电机机型控制，单片机和电机之间采用的CAN总线进行通信同样需要将浮点型数据进行相应的编码后进行发送。

#### 1.2.2 控制器架构

![image-20210126191343094](README.assets/image-20210126191343094.png)

下位机控制器采用STM32F407单片机，系统可分为三个部分：时间片轮询部分、系统显示部分和通信部分，这单个部分单独实现互不影响，只能通过相应的接口进行交流。

- **时间片轮询部分**

时间片轮询使用TIM3定时器及其中断，时间片可自行划分，最小时间片为1ms。

时间片轮询作为系统任务的执行部分，可以根据需求增加相应的时间片任务，目前最基本的任务有：

-- LED闪烁任务，用于判断系统是否正常运行；

-- OLED刷新，上面运行着一个计数器，计数器的值每隔50ms加一并显示，用以判断系统是否卡顿；

-- 按键检测任务，按键检测任务每隔20ms执行一次；

-- 电机控制任务，周期为5ms，该任务只在连续控制时才使用，当使用上位机直接控制时该任务不被创建。

若要创建相应的任务时间片，在下面这个结构体数组中增加相应的函数，并在*"task.h"*中更改相应的*"TASK_NUM"*.

![image-20210126214343911](README.assets/image-20210126214343911.png)

- **系统显示部分**

由于系统对时序要求较为严格，如果直接在时间片轮询中创建显示函数，那么在一些中断中会打断显示函数的执行，导致花屏，故将显示功能独立出来作为系统显示。

系统显示使用软中断实现，显示中断的抢占优先级和串口通信的优先级相同，子优先级低于串口通信优先级，故在串口通信的中断中不能使用系统显示函数。实现方式是将显示参数通过结构体传递，调用系统显示函数时会产生一个软中断，中断函数入口是外部中断3.

相应的接口如下:

![image-20210126214244222](README.assets/image-20210126214244222.png)

- **通信部分**

通信部分主要包括串口和CAN通信部分，以后和传感器之间的通信按时间要求，时间较长的可以创建相应的时间片，时间短的可以使用定时器中断实现，一般不包含在这个部分。

串口通信部分用以接收上位机控制指令以及上传航行器信息，上传通过串口DMA方式减少CPU开销，以提高数据交互速率；数据接收是通过中断接收，规定每次接收一帧30字节的数据，其中包括帧头和帧尾。串口每接收到上位机的一帧数据回向上位机反馈相应的数据，在上位机直接单次控制中每接收一帧电机控制信息，就控制一次电机并反馈电机此时的状态，具体实现见*串口接收中断函数*

串口通信接口如下：

![image-20210126215422763](README.assets/image-20210126215422763.png)

CAN总线通信部分是于电机进行通信，速率为1Mbps,电机的控制信息压缩在一个CAN的数据帧中(8字节)，每发送一帧数据便接收一帧数据。

CAN总线接口如下：

![image-20210126220036469](README.assets/image-20210126220036469.png)

电机控制接口如下：

![image-20210126220653949](README.assets/image-20210126220653949.png)

对外接口为最后4个函数，前面的函数为电机驱动内部调用。

与电机的通信部分有几个主要的全局结构体，如下：

-- 串口信息：

![串口消息](README.assets/image-20210126220225051.png)

-- 电机控制参数：

![电机控制参数](README.assets/image-20210126220332816.png)



## 二、 数据格式

### 2.1 控制信号



**控制信号共28 byte，发送时需要将这28个byte放在{ }中，如：{ab12CRfg45}表示发送的数据是：ab12CRfg45.**

| Name | Info | Size (bit) | Size (byte)|
| - | - | - | - |
| cmd | 控制命令 | 8| 1 |
| ak_id | ak电机ID | 8 | 1 |
| ak_p_dst | ak电机位置 | 16 | 2 |
| ak_v_dst | ak电机速度  | 16 | 2 |
| ak_t_dst | ak电机转矩 | 16 | 2 |
| ak_kp_dst | ak电机参数kp |16 | 2 |
| ak_kd_dst | ak电机参数kd | 16 | 2 |
| step_p_dst | 步进电机位置 | 16 | 2 |
| servo_angle_dst | 舵机角度 x6 | 8 x6 | 6 |
| reserved | 预留字节 | 64| 8 |

- 数据格式

| 0 | 1 | 2 | 3 | 4 | 5 | 6 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| cmd[7:0] | id[7:0] | p[15 :8] | p[7:0] | v[15:8] | v[7:0] |t[15:8]|

| 7 |8 | 9 | 10 | 11 | 12 | 13 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| t[7:0] | kp[15:8] | kp[7:0] | kd[15:8] | kd[7:0] | step_p[15:8] | step_p[7:0] |

| 14 | 15 | 16 | 17 | 18 | 19 | 20 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| servo_0[7:0] | servo_1[7:0] | servo_2[7:0] | servo_3[7:0] | servo_4[7:0] | servo_5[7:0] | reserved[7:0] |

| 21 |22 | 23 | 24 | 25 | 26 | 27 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] |

**cmd[7:0]命令及作用**

| 命令 |   0x00   |   0x01    |    0x02    |    0x03    |    0x04    | 0x05 | 0x06 | 0x07 |
| :--: | :------: | :-------: | :--------: | :--------: | :--------: | :--: | :--: | :--: |
| 含义 | reserved | en_m_mode | set_m_zero | ext_m_mode | ctrl_motor |      |      |      |

| 0x08 | 0x09 | 0x0a | 0x0b | 0x0c | 0x0d | 0x0e | 0x0f |
| :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
|      |      |      |      |      |      |      |      |

|   0xff    |
| :-------: |
| reset_sys |

**上位机信号编码**

上位机上输入的数据为float型，将其扩大100倍后转换为int型然后将int转换为两个字节的数据发送，接收端再进行解码，其发送数据格式如下;

| SOF(1byte) |         data(28byte)         | EOF(1byte) |
| :--------: | :--------------------------: | :--------: |
|    0x0d    | data_0[7:0] ... data_27[7:0] |    0x0a    |



### 2.2 回传信号 

电机的信息是每控制一次电机返回一次电机信息，如果要获取其它类型的信息，则需要发送不同指令来获取。控制一次电机会接收到6个字节的电机状态。

**共112 byte**

| Name | Info | Size (bit) | Size (byte) |
| - | - | - | - |
| ak_id | ak电机ID | 8 | 1 |
| ak_p | ak电机位置 | 16 | 2 |
| ak_v | ak电机速度 | 16 | 2 |
| ak_t | ak电机转矩 | 16 | 2 |
| error_type | 错误信号及其错误类型 | 8 | 1 |
| senser_pressure | 压力传感器信息 | 16 | 2 |
| senser_deep | 深度传感器信息 | 16 | 2 |
| senser_6axis | 6维力传感器 x4 | 16x6 x4 | 48 |
| senser_leak | 漏水传感器 | 8 | 1 |
| senser_step_p | 步进电机位置 | 16 | 2 |
| senser_imu | imu信息 | 16 x9 | 18 |
| senser_servo_p | 舵机角度信息 | 8 x6 | 6 |
| reserved | 预留字节 | 25 x8 | 25 |

- 数据格式

| 0 | 1 | 2 | 3 | 4 | 5 | 6 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| id[7:0] | p[15:8] | p[7:0] | v[15:8] | v[7:0] | t[15:8] | t[7:0] |

| 7 | 8 | 9 | 10 | 11 | 12 |  13  |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| error[7:0] | pressure[15:8] | pressure[7:0] | deep[15:8] | deep[7:0] | leak[7:0] | step_p[15:8] |

| 14 | 15 | 16 | 17 | 18 | 19 | 20 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| step_p[7:0] | imu_ax[15:8] | imu_ax[7:0] | imu_ay[15:8] | imu_ay[7:0] | imu_az[15:8] | imu_az[7:0] |

| 21 | 22 | 23 | 24 | 25 | 26 | 27 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| imu_gx[15:8] | imu_gx[7:0] | imu_gy[15:8] | imu_gy[7:0] | imu_gz[15:8] | imu_gz[7:0] | imu_mx[15:8] |

| 28 | 29 | 30 | 31 | 32 | 33 | 34 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| imu_mx[7:0] | imu_my[15:8] | imu_my[7:0] | imu_mz[15:8] | imu_mz[7:0] | servo_5[7:0] | servo_4[7:0] |

|      35      |      36      |      37      |      38      |       39       |      40       |       41       |
| :----------: | :----------: | :----------: | :----------: | :------------: | :-----------: | :------------: |
| servo_3[7:0] | servo_2[7:0] | servo_1[7:0] | servo_0[7:0] | axis3_fx[15:8] | axis3_fx[7:0] | axis3_fy[15:8] |

|      42       |       43       |      44       |       45       |      46       |       47       |      48       |
| :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: |
| axis3_fy[7:0] | axis3_fz[15:8] | axis3_fz[7:0] | axis3_tx[15:8] | axis3_tx[7:0] | axis3_ty[15:8] | axis3_ty[7:0] |

|       49       |      50       |       51       |      52       |       53       |      54       |       55       |
| :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: |
| axis3_tz[15:8] | axis3_tz[7:0] | axis2_fx[15:8] | axis2_fx[7:0] | axis2_fy[15:8] | axis2_fy[7:0] | axis2_fz[15:8] |

|      56       |       57       |      58       |       59       |      60       |       61       |      62       |
| :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: |
| axis2_fz[7:0] | axis2_tx[15:8] | axis2_tx[7:0] | axis2_ty[15:8] | axis2_ty[7:0] | axis2_tz[15:8] | axis2_tz[7:0] |

|       63       |      64       |       65       |      66       |       67       |      68       |       69       |
| :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: |
| axis1_fx[15:8] | axis1_fx[7:0] | axis1_fy[15:8] | axis1_fy[7:0] | axis1_fz[15:8] | axis1_fz[7:0] | axis1_tx[15:8] |

|      70       |       71       |      72       |       73       |      74       |       75       |      76       |
| :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: |
| axis1_tx[7:0] | axis1_ty[15:8] | axis1_ty[7:0] | axis1_tz[15:8] | axis1_tz[7:0] | axis0_fx[15:8] | axis0_fx[7:0] |

|       77       |      78       |       79       |      80       |       81       |      82       |       83       |
| :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: |
| axis0_fy[15:8] | axis0_fy[7:0] | axis0_fz[15:8] | axis0_fz[7:0] | axis0_tx[15:8] | axis0_tx[7:0] | axis0_ty[15:8] |

|      84       |       85       |      86       |      87       |      88       |      89       |      90       |
| :-----------: | :------------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: |
| axis0_ty[7:0] | axis0_tz[15:8] | axis0_tz[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] |

|      91       |      92       |      93       |      94       |      95       |      96       |      97       |
| :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: |
| reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] |

|      98       |      99       |      100      |      101      |      102      |      103      |      104      |
| :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: |
| reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] |

|      105      |      106      |      107      |      108      |      109      |      110      |      111      |
| :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: |
| reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] |