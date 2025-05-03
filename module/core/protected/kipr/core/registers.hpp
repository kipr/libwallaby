#ifndef KIPR_CORE_REGISTERS_HPP_
#define KIPR_CORE_REGISTERS_HPP_

#define WALLABY_SPI_VERSION 4

// Author: Joshua Southerland (2015)

#ifndef WALLABY_SPI_R1_H_
#define WALLABY_SPI_R1_H_


#define WALLABY_SPI_VERSION 4

// READ Only Registers ---------------------------------------------------------
#define REG_R_START        0

#define REG_R_VERSION_H      1
#define REG_R_VERSION_L      2


// READ/Write Registers --------------------------------------------------------

#define REG_RW_DIG_IN_H    3
#define REG_RW_DIG_IN_L    4
#define REG_RW_DIG_OUT_H   5
#define REG_RW_DIG_OUT_L   6
#define REG_RW_DIG_PE_H    7
#define REG_RW_DIG_PE_L    8
#define REG_RW_DIG_OE_H    9
#define REG_RW_DIG_OE_L    10

#define REG_RW_ADC_0_H     11
#define REG_RW_ADC_0_L     12
#define REG_RW_ADC_1_H     13
#define REG_RW_ADC_1_L     14
#define REG_RW_ADC_2_H     15
#define REG_RW_ADC_2_L     16
#define REG_RW_ADC_3_H     17
#define REG_RW_ADC_3_L     18
#define REG_RW_ADC_4_H     19
#define REG_RW_ADC_4_L     20
#define REG_RW_ADC_5_H     21
#define REG_RW_ADC_5_L     22
#define REG_RW_ADC_PE      23 // low 6 bits used

// Magnetometer (Float Values)
#define REG_RW_MAG_X_0    24
#define REG_RW_MAG_X_1    25
#define REG_RW_MAG_X_2    26
#define REG_RW_MAG_X_3    27

#define REG_RW_MAG_Y_0    28
#define REG_RW_MAG_Y_1    29
#define REG_RW_MAG_Y_2    30
#define REG_RW_MAG_Y_3    31

#define REG_RW_MAG_Z_0    32
#define REG_RW_MAG_Z_1    33
#define REG_RW_MAG_Z_2    34
#define REG_RW_MAG_Z_3    35

// Accelerometer (Float Values)
#define REG_RW_ACCEL_X_0   36
#define REG_RW_ACCEL_X_1   37
#define REG_RW_ACCEL_X_2   38
#define REG_RW_ACCEL_X_3   39

#define REG_RW_ACCEL_Y_0   40
#define REG_RW_ACCEL_Y_1   41
#define REG_RW_ACCEL_Y_2   42
#define REG_RW_ACCEL_Y_3   43

#define REG_RW_ACCEL_Z_0   44
#define REG_RW_ACCEL_Z_1   45
#define REG_RW_ACCEL_Z_2   46
#define REG_RW_ACCEL_Z_3   47

// Gyroscope (Float Values)
#define REG_RW_GYRO_X_0    48
#define REG_RW_GYRO_X_1    49
#define REG_RW_GYRO_X_2    50
#define REG_RW_GYRO_X_3    51

#define REG_RW_GYRO_Y_0    52
#define REG_RW_GYRO_Y_1    53
#define REG_RW_GYRO_Y_2    54
#define REG_RW_GYRO_Y_3    55

#define REG_RW_GYRO_Z_0    56
#define REG_RW_GYRO_Z_1    57
#define REG_RW_GYRO_Z_2    58
#define REG_RW_GYRO_Z_3    59

// Motor 0 position
#define REG_RW_MOT_0_B3    63
#define REG_RW_MOT_0_B2    64
#define REG_RW_MOT_0_B1    65
#define REG_RW_MOT_0_B0    66

// Motor 1 position
#define REG_RW_MOT_1_B3    67
#define REG_RW_MOT_1_B2    68
#define REG_RW_MOT_1_B1    69
#define REG_RW_MOT_1_B0    70

// Motor 2 position
#define REG_RW_MOT_2_B3    71
#define REG_RW_MOT_2_B2    72
#define REG_RW_MOT_2_B1    73
#define REG_RW_MOT_2_B0    74

// Motor 3 position
#define REG_RW_MOT_3_B3    75
#define REG_RW_MOT_3_B2    76
#define REG_RW_MOT_3_B1    77
#define REG_RW_MOT_3_B0    78

#define REG_RW_MOT_MODES       79
#define REG_RW_MOT_DIRS        80
#define REG_RW_MOT_DONE        81
#define REG_RW_MOT_SRV_ALLSTOP 82

// 16-bit signed speed goals
#define REG_RW_MOT_0_SP_H  83
#define REG_RW_MOT_0_SP_L  84
#define REG_RW_MOT_1_SP_H  85
#define REG_RW_MOT_1_SP_L  86
#define REG_RW_MOT_2_SP_H  87
#define REG_RW_MOT_2_SP_L  88
#define REG_RW_MOT_3_SP_H  89
#define REG_RW_MOT_3_SP_L  90

// 16-bit unsigned PWMs (from user or PID controller)
#define REG_RW_MOT_0_PWM_H  91
#define REG_RW_MOT_0_PWM_L  92
#define REG_RW_MOT_1_PWM_H  93
#define REG_RW_MOT_1_PWM_L  94
#define REG_RW_MOT_2_PWM_H  95
#define REG_RW_MOT_2_PWM_L  96
#define REG_RW_MOT_3_PWM_H  97
#define REG_RW_MOT_3_PWM_L  98

// 16-bit unsigned servo goals
#define REG_RW_SERVO_0_H   99
#define REG_RW_SERVO_0_L   100
#define REG_RW_SERVO_1_H   101
#define REG_RW_SERVO_1_L   102
#define REG_RW_SERVO_2_H   103
#define REG_RW_SERVO_2_L   104
#define REG_RW_SERVO_3_H   105
#define REG_RW_SERVO_3_L   106

// 12-bit unsigned ADC result
#define REG_RW_BATT_H      107
#define REG_RW_BATT_L      108

// Virtual button bits
#define REG_RW_BUTTONS     109

#define REG_READABLE_COUNT 110

// WRITE ONLY Registers ---------------------------------------------------------

// Sensitivity Registers
#define REG_W_ACCEL_SENSITIVITY   60
#define REG_W_GYRO_SENSITIVITY    61
#define REG_W_MAG_SENSITIVITY     62

#define REG_W_PID_0_P_H    111
#define REG_W_PID_0_P_L    112
#define REG_W_PID_0_PD_H   113
#define REG_W_PID_0_PD_L   114
#define REG_W_PID_0_I_H    115
#define REG_W_PID_0_I_L    116
#define REG_W_PID_0_ID_H   117
#define REG_W_PID_0_ID_L   118
#define REG_W_PID_0_D_H    119
#define REG_W_PID_0_D_L    120
#define REG_W_PID_0_DD_H   121
#define REG_W_PID_0_DD_L   122

#define REG_W_PID_1_P_H    123
#define REG_W_PID_1_P_L    124
#define REG_W_PID_1_PD_H   125
#define REG_W_PID_1_PD_L   126
#define REG_W_PID_1_I_H    127
#define REG_W_PID_1_I_L    128
#define REG_W_PID_1_ID_H   129
#define REG_W_PID_1_ID_L   130
#define REG_W_PID_1_D_H    131
#define REG_W_PID_1_D_L    132
#define REG_W_PID_1_DD_H   133
#define REG_W_PID_1_DD_L   134

#define REG_W_PID_2_P_H    135
#define REG_W_PID_2_P_L    136
#define REG_W_PID_2_PD_H   137
#define REG_W_PID_2_PD_L   138
#define REG_W_PID_2_I_H    139
#define REG_W_PID_2_I_L    140
#define REG_W_PID_2_ID_H   141
#define REG_W_PID_2_ID_L   142
#define REG_W_PID_2_D_H    143
#define REG_W_PID_2_D_L    144
#define REG_W_PID_2_DD_H   145
#define REG_W_PID_2_DD_L   146

#define REG_W_PID_3_P_H    147
#define REG_W_PID_3_P_L    148
#define REG_W_PID_3_PD_H   149
#define REG_W_PID_3_PD_L   150
#define REG_W_PID_3_I_H    151
#define REG_W_PID_3_I_L    152
#define REG_W_PID_3_ID_H   153
#define REG_W_PID_3_ID_L   154
#define REG_W_PID_3_D_H    155
#define REG_W_PID_3_D_L    156
#define REG_W_PID_3_DD_H   157
#define REG_W_PID_3_DD_L   158

// Motor position goals
#define REG_W_MOT_0_GOAL_B3    159
#define REG_W_MOT_0_GOAL_B2    160
#define REG_W_MOT_0_GOAL_B1    161
#define REG_W_MOT_0_GOAL_B0    162

#define REG_W_MOT_1_GOAL_B3    163
#define REG_W_MOT_1_GOAL_B2    164
#define REG_W_MOT_1_GOAL_B1    165
#define REG_W_MOT_1_GOAL_B0    166

#define REG_W_MOT_2_GOAL_B3    167
#define REG_W_MOT_2_GOAL_B2    168
#define REG_W_MOT_2_GOAL_B1    169
#define REG_W_MOT_2_GOAL_B0    170

#define REG_W_MOT_3_GOAL_B3    171
#define REG_W_MOT_3_GOAL_B2    172
#define REG_W_MOT_3_GOAL_B1    173
#define REG_W_MOT_3_GOAL_B0    174

#define REG_ALL_COUNT      175


#endif // WALLABY_SPI_R1_H_


#endif
