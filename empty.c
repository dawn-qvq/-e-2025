#include "board.h"
#include "stdio.h"


#include "motor.h"

#include "led.h"


int32_t Get_Encoder_countA = 0, encoderA_cnt, PWMA, Get_Encoder_countB = 0, encoderB_cnt, PWMB;

int flag = 0;
volatile int i = 0, key = 0;
int ang, km;
float error  = 0, HD_Last = 1;
int k = 10;
uint8_t Num1, Flag, LED_NUM, LED_flag = 0;

float pitch = 0, roll = 0, yaw = 0;
float initial_angle = 0;

uint8_t Quanshu = 0;

extern enum Mode mode;
extern Motor motor;
extern int L4,L3,L2,L1,R1,R2,R3,R4;

int main(void)
{
    int i = 0;

    Motor_Init();
    motor.AtargetSpeed = 0;
    motor.BtargetSpeed = 0;
    uint8_t buffer1[32] = {0};
    uint8_t buffer2[32] = {0};

    SYSCFG_DL_init();

    DL_Timer_startCounter(PWM_0_INST);
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);

    NVIC_ClearPendingIRQ(ENCODER_INT_IRQN);
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);


    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(ENCODER_INT_IRQN);

    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);

    Motor_start();
    
    while(1)
    {
			
			//Motor_Send(300,300);
        if(!DL_GPIO_readPins(KEY_PORT, KEY_PIN_2_PIN))
        {
            delay_ms(10);
            while(!DL_GPIO_readPins(KEY_PORT, KEY_PIN_2_PIN));
            delay_ms(10);
            LED_NUM++;
            if(LED_NUM > 5)LED_NUM = 1;
            LED_flag = 1;
        }
        if(LED_flag)
        {
            LED_flag = 0;
            for(uint8_t i = 0; i < LED_NUM; i++)
            {
                DL_GPIO_setPins(LED_PORT, LED_PIN_16_PIN);
                delay_ms(100);
                DL_GPIO_clearPins(LED_PORT, LED_PIN_16_PIN);
                delay_ms(200);
            }
        }
        if(!DL_GPIO_readPins(KEY_PORT, KEY_PIN_3_PIN))
        {
            delay_ms(10);
            while(!DL_GPIO_readPins(KEY_PORT, KEY_PIN_3_PIN));
            delay_ms(10);
            Quanshu = LED_NUM;
            break;
        }
    }
    
    while(Quanshu)
    {
        sensor();
        if(L1 && R1)error = 0;
        
        else if(L1 && !L2)error = 1;
        else if(R1 && !R2)error = -1;
        
        else if(L1 && L2)error = 2;
        else if(R1 && R2)error = -2;
        
        else if(L2 && !L3)error = 3;
        else if(R2 && !R3)error = -3;
        
        else if(L2 && L3)error = 4;
        else if(R2 && R3)error = -4;
        
        else if(L3 && !L4)error = 5;
        else if(R3 && !R4)error = -5;
        
        else if(L3 && L4)error = 6;
        else if(R3 && R4)error = -6;
        
        else if(!L3 && L4)error = 7;
        else if(!R3 && R4)error = -7;
        
        else
        {
            if(HD_Last == 1)error = 8;
            else if(HD_Last == -1)error = -8;
        }
        
        if(L3)HD_Last = 1;
        else if(R3)HD_Last = -1;
        
        if(L4 && L3 && L2 && Flag == 0)
        {
            Num1++;
            Flag = 1;
        }else if(!L1 && !L2 && !L3 && !L4 && !R1 && Flag == 1)
        {
            Num1++;
            Flag = 2;
        }else if(L1 && R1 && Flag == 2)
        {
            Num1++;
            Flag = 0;
        }
        
        if(Num1 >= 12 * Quanshu - 1)
        {
            Flag = 3;
            Motor_Send(0, 0);
        }else
        {
            Motor_Send(185 - k * error, 185 + k * error);
        }
    }
}
