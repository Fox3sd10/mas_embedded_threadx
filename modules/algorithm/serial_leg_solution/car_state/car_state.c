#include "car_state.h"
#include "kinematics.h"
#include <math.h>
#include <solution_def.h>

static car_state_e car_state = STATE_STANDUP;
static uint16_t    stable_cnt = 0;

#define CAR_DONE_THETA  0.30f /* 两腿 |θ|<17° 判站立完成 */
#define CAR_DONE_CYCLES 1     /* 无防抖: 瞬时满足即切 */

void car_state_init(void)
{
    car_state = STATE_STANDUP;
    stable_cnt = 0;
}

car_state_e car_state_get(void)
{
    return car_state;
}

void car_state_update(void)
{
    const fk *l = kinematics_get_left();
    const fk *r = kinematics_get_right();
    if (l == NULL || r == NULL)
        return;

    if (car_state == STATE_STANDUP)
    {
        if (fabsf(l->theta) < CAR_DONE_THETA && fabsf(r->theta) < CAR_DONE_THETA)
        {
            if (++stable_cnt >= CAR_DONE_CYCLES)
            {
                car_state = STATE_BALANCE;
                stable_cnt = 0; /* 切换后清零, 防溢出兜底 */
            }
        }
        else
        {
            stable_cnt = 0; /* 任一腿超窗, 重新攒 */
        }
    }
    /* BALANCE: latch 不回退 (掉线用 car_state_reset); 后续 JUMP/STEP 在此扩展转移 */
}

void car_state_reset(void)
{
    car_state = STATE_STANDUP;
    stable_cnt = 0;
}

// void ObsState_init(ObsState *obs _state)
// {
//     //规定每个模块的最高位0为未完成/失能，1为完成/使能
//     //AllSwitchState:默认全开 第1位为起立使能位，第2位为跳跃使能位，第3位为上楼使能位，剩余5位预留
//     //GetUpState: 第8位为倒地自启标志位0为不用，1为用 ，
//         // 剩余6位，12分别为左腿的状态12，34分别为右腿的状态12
//         //同时规定，12只能存在一个1，34只能存在一个1
//         //第8位为倒地自启标志位为1时
//             //先两个腿转到同一个角度，在同时转动使车体翻过来
//     obs_state->GetUpState = 0;
//     obs_state->JumpState = 0;
//     obs_state->UpStairState = 0;
//     obs_state->AllSwitchState = 0XFF;
// }
// void GetUpInit(ObsState *obs_state)
// {
//     const fk *l = kinematics_get_left();
//     const fk *r = kinematics_get_right();
//     if(obs_state->AllSwitchState & 0x01) //起立使能位为1
//     {
//         if((obs_state->GetUpState & 0x80)==0)
//         {
// //            if(l->theta)
//         }






//     }

// }