#ifndef _init_h
#define _init_h

extern void my_init();

#define servo                      (PWM4_MODULE2_CHA_C30)               //���

#define MOTOR1_PWM1                 (PWM2_MODULE2_CHA_C10)              // ��ǰ��ת
#define MOTOR1_PWM2                 (PWM2_MODULE3_CHA_D2)               // ��ǰ��ת

#define MOTOR2_PWM1                 (PWM2_MODULE2_CHB_C11)              // ��ǰ��ת
#define MOTOR2_PWM2                 (PWM2_MODULE3_CHB_D3)               // ��ǰ��ת

#define MOTOR3_PWM1                 (PWM2_MODULE1_CHB_C9)               // �����ת
#define MOTOR3_PWM2                 (PWM2_MODULE0_CHB_C7)               // ���ת

#define MOTOR4_PWM1                 (PWM2_MODULE1_CHA_C8)               // �Һ���ת
#define MOTOR4_PWM2                 (PWM2_MODULE0_CHA_C6)               // �Һ�ת

#endif
