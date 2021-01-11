# WIAS 仿生鱼控制系统底层设计

## 一、系统架构

### 1.1 硬件架构

#### 1.1.1手工焊接初步测试版本引脚设置

- **OLED_0.96IIC**

| SCL  | SDA  |
| :--: | :--: |
| PB13 | PB14 |

- **TJA1050 CAN收发器 (CAN1)**

| CAN_TX | CCAN_RX |
| :----: | :-----: |
|  PA12  |  PA11   |

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



### 1.2软件架构



## 二、 数据格式

### 2.1 控制信号

**共28 byte**

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

| 27 | 26 | 25 | 24 | 23 | 22 | 21 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| cmd[7:0] | id[7:0] | p[15 :8] | p[7:0] | v[15:8] | v[7:0] |t[15:8]|

| 20 |19 | 18 | 17 | 16 | 15 | 14 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| t[7:0] | kp[15:8] | kp[7:0] | kd[15:8] | kd[7:0] | step_p[15:8] | step_p[7:0] |

| 13 | 12 | 11 | 10 | 9 | 8 | 7 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| servo_0[7:0] | servo_1[7:0] | servo_2[7:0] | servo_3[7:0] | servo_4[7:0] | servo_5[7:0] | reserved[7:0] |

| 6 |5 | 4 | 3 | 2 | 1 | 0 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] |



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

| 111 | 110 | 109 | 108 | 107 | 106 | 105 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| id[7:0] | p[15:8] | p[7:0] | v[15:8] | v[7:0] | t[15:8] | t[7:0] |

| 104 | 103 | 102 | 101 | 100 | 99 |  98  |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| error[7:0] | pressure[15:8] | pressure[7:0] | deep[15:8] | deep[7:0] | leak[7:0] | step_p[15:8] |

| 97 | 96 | 95 | 94 | 93 | 92 | 91 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| step_p[7:0] | imu_ax[15:8] | imu_ax[7:0] | imu_ay[15:8] | imu_ay[7:0] | imu_az[15:8] | imu_az[7:0] |

| 90 | 89 | 88 | 87 | 86 | 85 | 84 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| imu_gx[15:8] | imu_gx[7:0] | imu_gy[15:8] | imu_gy[7:0] | imu_gz[15:8] | imu_gz[7:0] | imu_mx[15:8] |

| 83 | 82 | 81 | 80 | 79 | 78 | 77 |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| imu_mx[7:0] | imu_my[15:8] | imu_my[7:0] | imu_mz[15:8] | imu_mz[7:0] | servo_5[7:0] | servo_4[7:0] |

|      76      |      75      |      74      |      73      |       72       |      71       |       70       |
| :----------: | :----------: | :----------: | :----------: | :------------: | :-----------: | :------------: |
| servo_3[7:0] | servo_2[7:0] | servo_1[7:0] | servo_0[7:0] | axis3_fx[15:8] | axis3_fx[7:0] | axis3_fy[15:8] |

|      69       |       68       |      67       |       66       |      65       |       64       |      63       |
| :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: |
| axis3_fy[7:0] | axis3_fz[15:8] | axis3_fz[7:0] | axis3_tx[15:8] | axis3_tx[7:0] | axis3_ty[15:8] | axis3_ty[7:0] |

|       62       |      61       |       60       |      59       |       58       |      57       |       56       |
| :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: |
| axis3_tz[15:8] | axis3_tz[7:0] | axis2_fx[15:8] | axis2_fx[7:0] | axis2_fy[15:8] | axis2_fy[7:0] | axis2_fz[15:8] |

|      55       |       54       |      53       |       52       |      51       |       50       |      49       |
| :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: |
| axis2_fz[7:0] | axis2_tx[15:8] | axis2_tx[7:0] | axis2_ty[15:8] | axis2_ty[7:0] | axis2_tz[15:8] | axis2_tz[7:0] |

|       48       |      47       |       46       |      45       |       44       |      43       |       42       |
| :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: |
| axis1_fx[15:8] | axis1_fx[7:0] | axis1_fy[15:8] | axis1_fy[7:0] | axis1_fz[15:8] | axis1_fz[7:0] | axis1_tx[15:8] |

|      41       |       40       |      39       |       38       |      37       |       36       |      35       |
| :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: |
| axis1_tx[7:0] | axis1_ty[15:8] | axis1_ty[7:0] | axis1_tz[15:8] | axis1_tz[7:0] | axis0_fx[15:8] | axis0_fx[7:0] |

|       34       |      33       |       32       |      31       |       30       |      29       |       28       |
| :------------: | :-----------: | :------------: | :-----------: | :------------: | :-----------: | :------------: |
| axis0_fy[15:8] | axis0_fy[7:0] | axis0_fz[15:8] | axis0_fz[7:0] | axis0_tx[15:8] | axis0_tx[7:0] | axis0_ty[15:8] |

|      27       |       26       |      25       |      24       |      23       |      22       |      21       |
| :-----------: | :------------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: |
| axis0_ty[7:0] | axis0_tz[15:8] | axis0_tz[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] |

|      20       |      19       |      18       |      17       |      16       |      15       |      14       |
| :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: |
| reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] |

|      13       |      12       |      11       |      10       |       9       |       8       |       7       |
| :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: |
| reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] |

|       6       |       5       |       4       |       3       |       2       |       1       |       0       |
| :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: |
| reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] | reserved[7:0] |