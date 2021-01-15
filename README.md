# WIAS 仿生鱼控制系统底层设计

## 一、系统架构

### 1.1 硬件架构

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
| AK80-9 |   ±12.5   |    ±45    |    ±18    | 0 - 500 | 0 - 5 |
| AK10-9 |   ±12.5   |    ±45    |    ±54    | 0 - 500 | 0 - 5 |



### 1.2软件架构



## 二、 数据格式

### 2.1 控制信号



**控制信号共28 byte，发送时需要将这28个byte放在{ }中，如：{B0 B1 ... B27}**

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

| 命令 |   0x00   |   0x01    |    0x02    |    0x03    | 0x04 | 0x05 | 0x06 | 0x07 |
| :--: | :------: | :-------: | :--------: | :--------: | :--: | :--: | :--: | :--: |
| 含义 | reserved | en_m_mode | set_m_zero | ext_m_mode |      |      |      |      |

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