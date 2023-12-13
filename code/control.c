#include "control.h"
#include "zf_common_headfile.h"
#include "My_headfile.h"
/*前进*/
void front()
{
    pwm_set_duty (MOTOR1_PWM1, speed1_left1);
    pwm_set_duty (MOTOR2_PWM1, speed2_left1);
    pwm_set_duty (MOTOR3_PWM1, speed3_left1);
    pwm_set_duty (MOTOR4_PWM1, speed4_left1);
}
/*后退*/
void back()
{
    pwm_set_duty (MOTOR1_PWM2, speed1_left2);
    pwm_set_duty (MOTOR2_PWM2, speed2_left2);
    pwm_set_duty (MOTOR3_PWM2, speed3_left2);
    pwm_set_duty (MOTOR4_PWM2, speed4_left2);
}
/*右平移*/
void right_shift()
{
    pwm_set_duty (MOTOR1_PWM1, speed1_left1);
    pwm_set_duty (MOTOR2_PWM2, speed2_left2);
    pwm_set_duty (MOTOR3_PWM2, speed3_left2);
    pwm_set_duty (MOTOR4_PWM1, speed4_left1);
}
/*左平移*/
void left_shift()
{
    pwm_set_duty (MOTOR1_PWM2, speed1_left2);
    pwm_set_duty (MOTOR2_PWM1, speed2_left1);
    pwm_set_duty (MOTOR3_PWM1, speed3_left1);
    pwm_set_duty (MOTOR4_PWM2, speed4_left2);
}
/*右前进*/
void right_front()
{
    pwm_set_duty (MOTOR1_PWM1, speed1_left1);
    pwm_set_duty (MOTOR4_PWM1, speed3_left1);
}
/*左前进*/
void left_front()
{
    pwm_set_duty (MOTOR2_PWM1, speed1_left1);
    pwm_set_duty (MOTOR3_PWM1, speed3_left1);
}
/*右后退*/
void right_back()
{
    pwm_set_duty (MOTOR1_PWM2, speed1_left2);
    pwm_set_duty (MOTOR4_PWM2, speed3_left2);
}
/*左后退*/
void left_back()
{
    pwm_set_duty (MOTOR2_PWM2, speed1_left2);
    pwm_set_duty (MOTOR3_PWM2, speed3_left2);
}
/*顺时针*/
void front_clock_wise()
{
    pwm_set_duty (MOTOR1_PWM1, speed1_left1);
    pwm_set_duty (MOTOR2_PWM2, speed2_left2);
    pwm_set_duty (MOTOR3_PWM1, speed3_left1);
    pwm_set_duty (MOTOR4_PWM2, speed4_left2);
}
/*逆时针*/
void back_clock_wise()
{
    pwm_set_duty (MOTOR1_PWM1, speed1_left1);
    pwm_set_duty (MOTOR2_PWM2, speed2_left2);
    pwm_set_duty (MOTOR3_PWM1, speed3_left1);
    pwm_set_duty (MOTOR4_PWM2, speed4_left2);
}