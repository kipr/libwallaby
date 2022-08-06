#ifndef _KIPR_CORE_REGISTERS_HPP_
#define _KIPR_CORE_REGISTERS_HPP_

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

#define REG_RW_MAG_X_H     24
#define REG_RW_MAG_X_L     25
#define REG_RW_MAG_Y_H     26
#define REG_RW_MAG_Y_L     27
#define REG_RW_MAG_Z_H     28
#define REG_RW_MAG_Z_L     29

#define REG_RW_ACCEL_X_H   30
#define REG_RW_ACCEL_X_L   31
#define REG_RW_ACCEL_Y_H   32
#define REG_RW_ACCEL_Y_L   33
#define REG_RW_ACCEL_Z_H   34
#define REG_RW_ACCEL_Z_L   35

#define REG_RW_GYRO_X_H    36
#define REG_RW_GYRO_X_L    37
#define REG_RW_GYRO_Y_H    38
#define REG_RW_GYRO_Y_L    39
#define REG_RW_GYRO_Z_H    40
#define REG_RW_GYRO_Z_L    41

// Motor 0 position
#define REG_RW_MOT_0_B3    42
#define REG_RW_MOT_0_B2    43
#define REG_RW_MOT_0_B1    44
#define REG_RW_MOT_0_B0    45

// Motor 1 position
#define REG_RW_MOT_1_B3    46
#define REG_Rw_MOT_1_B2    47
#define REG_Rw_MOT_1_B1    48
#define REG_RW_MOT_1_B0    49

// Motor 2 position
#define REG_RW_MOT_2_B3    50
#define REG_RW_MOT_2_B2    51
#define REG_RW_MOT_2_B1    52
#define REG_RW_MOT_2_B0    53

// Motor 3 position
#define REG_RW_MOT_3_B3    54
#define REG_RW_MOT_3_B2    55
#define REG_RW_MOT_3_B1    56
#define REG_RW_MOT_3_B0    57

#define REG_RW_MOT_MODES   58  //   Normal PWM, MTP, MAV, MRP, 2 bits per motor
#define REG_RW_MOT_DIRS    59  //   IDLE, FORWARD, REVERSE, BREAK, 2 bits per motor
#define REG_RW_MOT_DONE    60  //   4 lowest bit used:   0000 (chan0) (chan1) (chan2) (chan3)
#define REG_RW_MOT_SRV_ALLSTOP 61 //  2nd lowest bit is motor all stop, lowest bit is servo allstop

// 16 bit signed speed goals
#define REG_RW_MOT_0_SP_H  62
#define REG_RW_MOT_0_SP_L  63
#define REG_RW_MOT_1_SP_H  64
#define REG_RW_MOT_1_SP_L  65
#define REG_RW_MOT_2_SP_H  66
#define REG_RW_MOT_2_SP_L  67
#define REG_RW_MOT_3_SP_H  68
#define REG_RW_MOT_3_SP_L  69

// 16 bit unsigned pwms, from the user or PID controller
#define REG_RW_MOT_0_PWM_H  70
#define REG_RW_MOT_0_PWM_L  71
#define REG_RW_MOT_1_PWM_H  72
#define REG_RW_MOT_1_PWM_L  73
#define REG_RW_MOT_2_PWM_H  74
#define REG_RW_MOT_2_PWM_L  75
#define REG_RW_MOT_3_PWM_H  76
#define REG_RW_MOT_3_PWM_L  77

// 16 bit unsigned servo goals
// microsecond servo pulse, where 1500 is neutral
// +/- about 10 per degree from neutral
#define REG_RW_SERVO_0_H   78
#define REG_RW_SERVO_0_L   79
#define REG_RW_SERVO_1_H   80
#define REG_RW_SERVO_1_L   81
#define REG_RW_SERVO_2_H   82
#define REG_RW_SERVO_2_L   83
#define REG_RW_SERVO_3_H   84
#define REG_RW_SERVO_3_L   85

// 12 bit unsigned int adc result
#define REG_RW_BATT_H      86
#define REG_RW_BATT_L      87

// msb is "extra show", then a non used bit
// then 6 virtual button bits
// E x 5 4 3 2 1 0
#define REG_RW_BUTTONS 88

#define REG_READABLE_COUNT 89



// WRITE ONLY Registers---------------------------------------------------------
#define REG_W_PID_0_P_H    89
#define REG_W_PID_0_P_L    90
#define REG_W_PID_0_PD_H   91
#define REG_W_PID_0_PD_L   92
#define REG_W_PID_0_I_H    93
#define REG_W_PID_0_I_L    94
#define REG_W_PID_0_ID_H   95
#define REG_W_PID_0_ID_L   96
#define REG_W_PID_0_D_H    97
#define REG_W_PID_0_D_L    98
#define REG_W_PID_0_DD_H   99
#define REG_W_PID_0_DD_L   100
#define REG_W_PID_1_P_H    101
#define REG_W_PID_1_P_L    102
#define REG_W_PID_1_PD_H   103
#define REG_W_PID_1_PD_L   104
#define REG_W_PID_1_I_H    105
#define REG_W_PID_1_I_L    106
#define REG_W_PID_1_ID_H   107
#define REG_W_PID_1_ID_L   108
#define REG_W_PID_1_D_H    119
#define REG_W_PID_1_D_L    110
#define REG_W_PID_1_DD_H   111
#define REG_W_PID_1_DD_L   112
#define REG_W_PID_2_P_H    113
#define REG_W_PID_2_P_L    114
#define REG_W_PID_2_PD_H   115
#define REG_W_PID_2_PD_L   116
#define REG_W_PID_2_I_H    117
#define REG_W_PID_2_I_L    118
#define REG_W_PID_2_ID_H   119
#define REG_W_PID_2_ID_L   120
#define REG_W_PID_2_D_H    121
#define REG_W_PID_2_D_L    122
#define REG_W_PID_2_DD_H   123
#define REG_W_PID_2_DD_L   124

#define REG_W_PID_3_P_H    125
#define REG_W_PID_3_P_L    126
#define REG_W_PID_3_PD_H   127
#define REG_W_PID_3_PD_L   128
#define REG_W_PID_3_I_H    129
#define REG_W_PID_3_I_L    130
#define REG_W_PID_3_ID_H   131
#define REG_W_PID_3_ID_L   132
#define REG_W_PID_3_D_H    133
#define REG_W_PID_3_D_L    134
#define REG_W_PID_3_DD_H   135
#define REG_W_PID_3_DD_L   136

// Motor 0 position goal
#define REG_W_MOT_0_GOAL_B3    137
#define REG_W_MOT_0_GOAL_B2    138
#define REG_W_MOT_0_GOAL_B1    139
#define REG_W_MOT_0_GOAL_B0    140

// Motor 1 position goal
#define REG_W_MOT_1_GOAL_B3    141
#define REG_w_MOT_1_GOAL_B2    142
#define REG_w_MOT_1_GOAL_B1    143
#define REG_W_MOT_1_GOAL_B0    144

// Motor 2 position goal
#define REG_W_MOT_2_GOAL_B3    145
#define REG_W_MOT_2_GOAL_B2    146
#define REG_W_MOT_2_GOAL_B1    147
#define REG_W_MOT_2_GOAL_B0    148

// Motor 3 position goal
#define REG_W_MOT_3_GOAL_B3    149
#define REG_W_MOT_3_GOAL_B2    150
#define REG_W_MOT_3_GOAL_B1    151
#define REG_W_MOT_3_GOAL_B0    152

#define REG_ALL_COUNT      153

#endif