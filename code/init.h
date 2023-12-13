#ifndef _init_h
#define _init_h

extern void my_init();

#define servo                      (PWM4_MODULE2_CHA_C30)               //舵机

#define MOTOR1_PWM1                 (PWM2_MODULE2_CHA_C10)              // 左前正转
#define MOTOR1_PWM2                 (PWM2_MODULE3_CHA_D2)               // 左前反转

#define MOTOR2_PWM1                 (PWM2_MODULE2_CHB_C11)              // 右前正转
#define MOTOR2_PWM2                 (PWM2_MODULE3_CHB_D3)               // 右前反转

#define MOTOR3_PWM1                 (PWM2_MODULE1_CHB_C9)               // 左后正转
#define MOTOR3_PWM2                 (PWM2_MODULE0_CHB_C7)               // 左后反转

#define MOTOR4_PWM1                 (PWM2_MODULE1_CHA_C8)               // 右后正转
#define MOTOR4_PWM2                 (PWM2_MODULE0_CHA_C6)               // 右后反转

#endif
