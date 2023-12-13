#include "init.h"
#include "zf_common_headfile.h"
#include "My_headfile.h"

void my_init()
{
    /*舵机初始化*/
    pwm_init(servo, 50, 280);                                                           //PWM初始化频率 50KHz 占空比初始为 200

    /*电机初始化*/
    pwm_init(MOTOR1_PWM1, 17000, 0);											        // 左后正转
	pwm_init(MOTOR1_PWM2, 17000, 0);											        // 左后反转
    
    pwm_init(MOTOR2_PWM1, 17000, 0);											        // 右后正转
	pwm_init(MOTOR2_PWM2, 17000, 0);											        // 右后反转
    
    pwm_init(MOTOR3_PWM1, 17000, 0);											        // 右前正转
	pwm_init(MOTOR3_PWM2, 17000, 0);											        // 右前反转
    
    pwm_init(MOTOR4_PWM1, 17000, 0);											        // 左前正转
	pwm_init(MOTOR4_PWM2, 17000, 0);											        // 左前反转

    /*总钻风初始化*/
    mt9v03x_init();

    /*tft初始化*/
    tft180_init();

}
