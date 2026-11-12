#ifndef _CAR_STATE_H_
#define _CAR_STATE_H_

/* 腿部运动状态机 (预留动作状态) */
typedef enum
{
    STATE_STANDUP = 0, /* 起立 */
    STATE_BALANCE,     /* 正常平衡 */
    /* 预留: STATE_JUMP, STATE_STEP, ... */
    STATE_NUM
} car_state_e;


void        car_state_init(void);   /* 上电默认 STANDUP */
void        car_state_update(void); /* 每周期调用(需 kinematics 已更新): 判据+防抖+latch */
car_state_e car_state_get(void);
void        car_state_reset(void);  /* 掉线: 回 STANDUP (不碰 x) */

#endif
