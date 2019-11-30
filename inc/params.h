//#############################################################################
//
//  $Created on:    September 2019.
//  $Author:        Tomislav Ivanis
//  $Copyright:     (C) 2019 Tomislav Ivanis
//  $E-mail:        tomislav.ivanis@gmail.com
//                  tomislav.ivanis@fer.hr
//
//   Redistributions of source code must retain the above copyright notice.
//
//#############################################################################

#ifndef INC_PARAMS_H_
#define INC_PARAMS_H_

// Absolute motor constants
#define PARAMS_MOTOR_VPHASE_V           (1.0F)
#define PARAMS_MOTOR_IPHASE_A           (1.0F)
#define PARAMS_MOTOR_RS_OHM             (1.0F)
#define PARAMS_MOTOR_LD_H               (1.0F)
#define PARAMS_MOTOR_LQ_H               (1.0F)
#define PARAMS_MOTOR_POLEPAIRS          (1U)
#define PARAMS_MOTOR_POLES              (PARAMS_MOTOR_POLEPAIRS*2)

// Per unit motor constants
#define PARMAS_MOTOR_VPHASE_PU          (1.0F)
#define PARAMS_MOTOR_IPHASE_PU          (1.0F)
#define PARMAS_MOTOR_RS_PU              (1.0F)

// Clarke and Park Transformation parameters
#define PARAMS_N_CURR_SENS              (2)
#define PARAMS_CLARKE_ALPHA_SF          (1.0F)
#define PARAMS_CLARKE_BETA_SF           (MATH_ONE_OVER_SQRT_THREE)

// PI controller parameters
#define PARAMS_PI_KP_ID                 (PARAMS_MOTOR_RS_OHM/PARAMS_MOTOR_LD_H)
#define PARAMS_PI_KI_ID                 (PARAMS_MOTOR_RS_OHM/PARAMS_MOTOR_LD_H)
#define PARAMS_PI_KP_IQ                 (PARAMS_MOTOR_RS_OHM/PARAMS_MOTOR_LQ_H)
#define PARAMS_PI_KI_IQ                 (PARAMS_MOTOR_RS_OHM/PARAMS_MOTOR_LQ_H)
#define PARAMS_PI_VMAX_V                (PARAMS_MOTOR_VPHASE_V)
#define PARAMS_VMAX_PU                  (MATH_ONE_OVER_SQRT_THREE)
#define PARAMS_PI_IQ_FF_VALUE           (0.0F)
#define PARAMS_PI_ID_FF_VALUE           (0.0F)

typedef struct _Motor_Vars_t
{
    const float32_t     VPhase_V;
    const float32_t     IPhase_A;

    float32_t           RS_OHM;

} Motor_Vars_t;

#endif /* INC_PARAMS_H_ */
