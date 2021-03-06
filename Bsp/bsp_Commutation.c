/********************************************************************************
 *@FileName    bsp_Commutation.c
 *@Author      Johnbee
 *@Version     V0.1
 *@Date        2015/02/23
 *@Brief       电机换相
 *******************************************************************************/
/*#includes*********************************************************************/
#include "bsp_Commutation.h"

/*#define***********************************************************************/
#define  MyAbs(a)    (a>0?a:-a)
    
/*全局变量声明******************************************************************/
static float Motor_SetedSpeed = 0;  //电机速度-100~100
static s16 Middle_PWM_Duty = 500;

static const  s8 I_IN  = 1;  //电流流入,相电流为正
static const  s8 I_OUT = -1; //电流流出,相电流为负
static const  s8 I_OFF = 0;  //无电流
u8 Motor_State = 0;  //电机运行标志
static const s8 BLDC_STATE_TAB[8][3] =  //正向表
{
    /*霍尔值转十进制后查表*/
    /* 1.C+A-   5.B+A-    4.B+C-    6.A+C-    2.B+C-   3.C+B- *///正向    
    {I_OFF, I_OFF, I_OFF},  //CBA = 000时ABC三相电流方向    
    {I_OUT, I_OFF, I_IN },  //C+A-; 
    {I_OFF, I_OUT, I_IN },  //C+B-; 这个在1会振动    
    {I_IN,  I_OUT, I_OFF},  //A+B-; 这个在1会振动  
    {I_IN , I_OFF, I_OUT},  //A+C-;     
    {I_OFF, I_IN , I_OUT},  //B+C-;              
    {I_OUT, I_IN , I_OFF},  //B+A-;   
    {I_OFF, I_OFF, I_OFF},  //CBA = 111,120度霍尔不会出现这种情况,如果出现了一定有问题,所以马上刹车 
};
#if 0
static const s8 BLDC_STATE_TAB_Fan[8][3] =  //反向表
{
   /*反向速度的反向表*/
    {I_OFF, I_OFF, I_OFF},  //CBA = 000时ABC三相电流方向    
    {I_OUT, I_OFF, I_IN },  //C+A-; 这个在1会反转,速度较快 
    {I_OFF, I_OUT, I_IN },  //C+B-; 这个在1会振动    
    {I_IN,  I_OUT, I_OFF},  //A+B-; 这个在1会振动  
    {I_IN , I_OFF, I_OUT},  //A+C-;这个在1会正转,速度较快       
    {I_OFF, I_IN , I_OUT},  //B+C-; 这个在1会正转,速度感觉也不快               
    {I_OUT, I_IN , I_OFF},  //B+A-; 这个在1会反转,速度感觉不快    
    {I_OFF, I_OFF, I_OFF},  //CBA = 111,120度霍尔不会出现这种情况,如果出现了一定有问题,所以马上刹车    
};
#endif
/*
 *@ <function name=>Hall_PrepareCommutation() </function>
 *@ <summary>
     霍尔产生的换相事件
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void Hall_PrepareCommutation(void)
{
    float temp_duty = 0;  //临时占空比
    s8 hall_state[3] = {0, 0, 0}; //ABC三相状态数组
    u8 newhallpos =  Get_HallInputValue();
    newhallpos = HallCode2Decimal(newhallpos);
    if(Motor_State == 0)//电机失能
    {
        TIM1_Set_PWMOutput_EnableState(1, 0, 0);
        TIM1_Set_PWMOutput_EnableState(2, 0, 0);
        TIM1_Set_PWMOutput_EnableState(3, 0, 0); 
        return;        
    }
#if 0   //拉低一桥的换相
    for (u8 ii = 0; ii < 3; ii++) /*先找到off的桥先拉低*/
    {
        #if 1 //双表换相
        if(Motor_SetedSpeed>0)
        {
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//正向         
        }
        else
        {
           hall_state[ii] = BLDC_STATE_TAB_Fan[newhallpos][ii]; //反向                    
        }
        #endif
        #if 0 //单表换相
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//正向        
        #endif
        if (hall_state[ii] == 0)
        {
            TIM1_Set_PWMOutput_EnableState(ii + 1, 0, 0); //全桥拉低
            //break;
        }
    }
    for (u8 ii = 0; ii < 3; ii++) /*再改变不是off的桥*/
    {
        #if 1 //双表换相
        if(Motor_SetedSpeed>=0)
        {
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//正向         
        }
        else
        {
           hall_state[ii] = BLDC_STATE_TAB_Fan[newhallpos][ii]; //反向                    
        }
        #endif
        #if 0 //单表换相
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//正向        
        #endif
        if (hall_state[ii] != 0)
        {
            TIM1_Set_PWMOutput_EnableState(ii + 1, 1, 1); // 使能输出
        }
    }
#endif
#if 1  //使用COM事件换相,更改可同时生效
    for (u8 ii = 0; ii < 3; ii++) /*直接查表换相*/
    {
        #if 0 //双表换相
        if(Motor_SetedSpeed>=0)
        {
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//正向         
        }
        else
        {
           hall_state[ii] = BLDC_STATE_TAB_Fan[newhallpos][ii]; //反向                    
        }       
        #endif
        #if 1 //单表换相
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//正向        
        #endif
        if (hall_state[ii] == 0)
        {
            TIM1_Set_PWMOutput_EnableState(ii + 1, 0, 0); //全桥拉低
        }
        else
        {
            TIM1_Set_PWMOutput_EnableState(ii + 1, 1, 1); //使能           
        }                
        temp_duty = hall_state[ii] * Motor_SetedSpeed / 2;      
        temp_duty += Middle_PWM_Duty;
        TIM1_Set_PWMDuty(ii + 1, temp_duty); 
    }   
#endif    
}
/*
 *@ <function name=>Set_MotorSpeed() </function>
 *@ <summary>
    设置电机速度
 *@ </summary>
 *@ <param name="speed">速度从-1000到1000</param>
 *@ <returns> null </returns>
 */
void Set_MotorSpeed(float speed)
{
#if 1  //只负责改变占空比,使能在换相事件中改变,双表换相
    s8 hall_state[3] = {0, 0, 0}; //ABC三相状态数组
    float temp_duty = 0;  //临时占空比
    static float last_speed = 0;
    u8 newhallpos =  Get_HallInputValue();
    newhallpos = HallCode2Decimal(newhallpos);
    Motor_SetedSpeed = speed;
    if(last_speed*Motor_SetedSpeed<=0)
    { 
        /*上次和这次速度方向不同时要触发一次换相事件*/
        Hall_PrepareCommutation();
        Manual_COMevent();
    }
    else
    {

        for(u8 ii = 0; ii<3;ii++)
        {  
            #if 0 //双表换相
            if(Motor_SetedSpeed>=0)
            {
               hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//正向         
            }
            else
            {
               hall_state[ii] = BLDC_STATE_TAB_Fan[newhallpos][ii]; //反向                    
            }
            #endif
            #if 1 //单表换相
            hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//正向  
            #endif
            temp_duty = hall_state[ii] * Motor_SetedSpeed/ 2;                  
            temp_duty += Middle_PWM_Duty;
            TIM1_Set_PWMDuty(ii + 1, temp_duty); 
        } 
        last_speed = Motor_SetedSpeed;
    }
#endif
}

/*
 *@ <function name=>Set_MotorState() </function>
 *@ <summary>
    set the motor state on or off
 *@ </summary>
 *@ <param name="myState"> 0 for off,!0 for on</param>
 *@ <returns> null </returns>
 */
void Set_MotorState(u8 myState)
{
    if(myState == 0)//off state
    {
        Motor_State = 0;
    }
    else           //on state
    {
        Motor_State = 1;
    }
    Hall_PrepareCommutation();
    Manual_COMevent();

}
/*
 *@ <function name=>GrayCode2Decimal() </function>
 *@ <summary>
    u16类型的Gray码转十进制
 *@ </summary>
 *@ <param name="x">Gray码</param>
 *@ <returns> Gray码对应的十进制数 </returns>
 */
static u16 GrayCode2Decimal(u16 x) 
{ 
    u16 y = x; 
    while(x>>=1) 
    {
        y ^= x;        
    }
    return y;        
}
/*
 *@ <function name=>HallCode2Decimal() </function>
 *@ <summary>
    霍尔序列码转十进制
    hall序列为7 5 4 6 2 3 1 0返回7 6 5 4 3 2 1 0 
 *@ </summary>
 *@ <param name="x">霍尔码</param>
 *@ <returns> 霍尔码对应的十进制数 </returns>
 */
static u8 HallCode2Decimal(u8 x)
{
    u16 temp = x,ref = 0;
    temp = GrayCode2Decimal(temp);
    if(temp == 7) 
    {
        ref = 5;
    }
    else if(temp == 5)
    {
        ref = 7;
    }
    else
    {
        ref = temp;
    }
    return ref;
}
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */

