/*
 * wallaby_regs_p.hpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

namespace Private
{

#ifndef SRC_WALLABY_REGS_P_HPP_
#define SRC_WALLABY_REGS_P_HPP_



#define WALLABY_SPI_VERSION 1

// READ Only Registers ---------------------------------------------------------
#define REG_R_START        0

// READ/Write Registers --------------------------------------------------------

#define REG_RW_DIG_IN_H    1
#define REG_RW_DIG_IN_L    2
#define REG_RW_DIG_OUT_H   3
#define REG_RW_DIG_OUT_L   4
#define REG_RW_DIG_PE_H    5
#define REG_RW_DIG_PE_L    6
#define REG_RW_DIG_OE_H    7
#define REG_RW_DIG_OE_L    8

#define REG_RW_ADC_0_H     9
#define REG_RW_ADC_0_L     10
#define REG_RW_ADC_1_H     11
#define REG_RW_ADC_1_L     12
#define REG_RW_ADC_2_H     13
#define REG_RW_ADC_2_L     14
#define REG_RW_ADC_3_H     15
#define REG_RW_ADC_3_L     16
#define REG_RW_ADC_4_H     17
#define REG_RW_ADC_4_L     18
#define REG_RW_ADC_5_H     19
#define REG_RW_ADC_5_L     20
#define REG_RW_ADC_PE      21 // low 6 bits used

#define REG_RW_MAG_X_H     22
#define REG_RW_MAG_X_L     23
#define REG_RW_MAG_Y_H     24
#define REG_RW_MAG_Y_L     25
#define REG_RW_MAG_Z_H     26
#define REG_RW_MAG_Z_L     27

#define REG_RW_ACCEL_X_H   28
#define REG_RW_ACCEL_X_L   29
#define REG_RW_ACCEL_Y_H   30
#define REG_RW_ACCEL_Y_L   31
#define REG_RW_ACCEL_Z_H   32
#define REG_RW_ACCEL_Z_L   33

#define REG_RW_GYRO_X_H    34
#define REG_RW_GYRO_X_L    35
#define REG_RW_GYRO_Y_H    36
#define REG_RW_GYRO_Y_L    37
#define REG_RW_GYRO_Z_H    38
#define REG_RW_GYRO_Z_L    39

// Motor 0 position
#define REG_RW_MOT_0_B3    40
#define REG_RW_MOT_0_B2    41
#define REG_RW_MOT_0_B1    42
#define REG_RW_MOT_0_B0    43

// Motor 1 position
#define REG_RW_MOT_1_B3    44
#define REG_Rw_MOT_1_B2    45
#define REG_Rw_MOT_1_B1    46
#define REG_RW_MOT_1_B0    47

// Motor 2 position
#define REG_RW_MOT_2_B3    48
#define REG_RW_MOT_2_B2    49
#define REG_RW_MOT_2_B1    50
#define REG_RW_MOT_2_B0    51

// Motor 3 position
#define REG_RW_MOT_3_B3    52
#define REG_RW_MOT_3_B2    53
#define REG_RW_MOT_3_B1    54
#define REG_RW_MOT_3_B0    55

#define REG_RW_MOT_MODES   56  //   Normal PWM, MTP, MAV, MRP, 2 bits per motor
#define REG_RW_MOT_DIRS    57  //   IDLE, FORWARD, REVERSE, BREAK, 2 bits per motor
#define REG_RW_MOT_DONE    58  //   4 lowest bit used:   0000 (chan0) (chan1) (chan2) (chan3)
#define REG_RW_MOT_SRV_ALLSTOP 59 //  2nd lowest bit is motor all stop, lowest bit is servo allstop

// 16 bit signed speed goals
#define REG_RW_MOT_0_SP_H  60
#define REG_RW_MOT_0_SP_L  61
#define REG_RW_MOT_1_SP_H  62
#define REG_RW_MOT_1_SP_L  63
#define REG_RW_MOT_2_SP_H  64
#define REG_RW_MOT_2_SP_L  65
#define REG_RW_MOT_3_SP_H  66
#define REG_RW_MOT_3_SP_L  67

// 16 bit unsigned pwms, from the user or PID controller
#define REG_RW_MOT_0_PWM_H  68
#define REG_RW_MOT_0_PWM_L  69
#define REG_RW_MOT_1_PWM_H  70
#define REG_RW_MOT_1_PWM_L  71
#define REG_RW_MOT_2_PWM_H  72
#define REG_RW_MOT_2_PWM_L  73
#define REG_RW_MOT_3_PWM_H  74
#define REG_RW_MOT_3_PWM_L  75

// 16 bit unsigned servo goals
// microsecond servo pulse, where 1500 is neutral
// +/- about 10 per degree from neutral
#define REG_RW_SERVO_0_H   76
#define REG_RW_SERVO_0_L   77
#define REG_RW_SERVO_1_H   78
#define REG_RW_SERVO_1_L   79
#define REG_RW_SERVO_2_H   80
#define REG_RW_SERVO_2_L   81
#define REG_RW_SERVO_3_H   82
#define REG_RW_SERVO_3_L   83

// 12 bit unsigned int adc result
#define REG_RW_BATT_H      84
#define REG_RW_BATT_L      85

#define REG_READABLE_COUNT 86



// WRITE ONLY Registers---------------------------------------------------------
#define REG_W_PID_0_P_H    86
#define REG_W_PID_0_P_L    87
#define REG_W_PID_0_PD_H   88
#define REG_W_PID_0_PD_L   89
#define REG_W_PID_0_I_H    90
#define REG_W_PID_0_I_L    91
#define REG_W_PID_0_ID_H   92
#define REG_W_PID_0_ID_L   93
#define REG_W_PID_0_D_H    94
#define REG_W_PID_0_D_L    95
#define REG_W_PID_0_DD_H   96
#define REG_W_PID_0_DD_L   97
#define REG_W_PID_1_P_H    98
#define REG_W_PID_1_P_L    99
#define REG_W_PID_1_PD_H   100
#define REG_W_PID_1_PD_L   101
#define REG_W_PID_1_I_H    102
#define REG_W_PID_1_I_L    104
#define REG_W_PID_1_ID_H   103
#define REG_W_PID_1_ID_L   104
#define REG_W_PID_1_D_H    105
#define REG_W_PID_1_D_L    106
#define REG_W_PID_1_DD_H   107
#define REG_W_PID_1_DD_L   108
#define REG_W_PID_2_P_H    109
#define REG_W_PID_2_P_L    110
#define REG_W_PID_2_PD_H   111
#define REG_W_PID_2_PD_L   112
#define REG_W_PID_2_I_H    113
#define REG_W_PID_2_I_L    114
#define REG_W_PID_2_ID_H   115
#define REG_W_PID_2_ID_L   116
#define REG_W_PID_2_D_H    117
#define REG_W_PID_2_D_L    118
#define REG_W_PID_2_DD_H   119
#define REG_W_PID_2_DD_L   120

#define REG_W_PID_3_P_H    121
#define REG_W_PID_3_P_L    122
#define REG_W_PID_3_PD_H   123
#define REG_W_PID_3_PD_L   124
#define REG_W_PID_3_I_H    125
#define REG_W_PID_3_I_L    126
#define REG_W_PID_3_ID_H   127
#define REG_W_PID_3_ID_L   128
#define REG_W_PID_3_D_H    129
#define REG_W_PID_3_D_L    130
#define REG_W_PID_3_DD_H   131
#define REG_W_PID_3_DD_L   132


// Motor 0 position goal
#define REG_W_MOT_0_GOAL_B3    133
#define REG_W_MOT_0_GOAL_B2    134
#define REG_W_MOT_0_GOAL_B1    135
#define REG_W_MOT_0_GOAL_B0    136

// Motor 1 position goal
#define REG_W_MOT_1_GOAL_B3    137
#define REG_w_MOT_1_GOAL_B2    138
#define REG_w_MOT_1_GOAL_B1    139
#define REG_W_MOT_1_GOAL_B0    140

// Motor 2 position goal
#define REG_W_MOT_2_GOAL_B3    141
#define REG_W_MOT_2_GOAL_B2    142
#define REG_W_MOT_2_GOAL_B1    143
#define REG_W_MOT_2_GOAL_B0    144

// Motor 3 position goal
#define REG_W_MOT_3_GOAL_B3    145
#define REG_W_MOT_3_GOAL_B2    146
#define REG_W_MOT_3_GOAL_B1    147
#define REG_W_MOT_3_GOAL_B0    148

#define REG_ALL_COUNT      149

}

#endif /* SRC_WALLABY_REGS_P_HPP_ */
