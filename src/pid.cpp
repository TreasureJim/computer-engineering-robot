#include "pid.h"
#include "helpers.h"

/// @brief Initialises the PID Controller
/// @param pid pointer to PIDController struct
/// @param Kp proportional constant
/// @param Ki integrator constant
/// @param Kd derivative constant
void PIDController_Init(PIDController *pid, float Kp, float Ki, float Kd, float Hz)
{
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;

	pid->Hz = Hz;

	pid->integrator = 0.0f;
	pid->prevError = 0.0f;
}

/// @brief Computes the value of the PID
/// @param pid point to PIDController struct
/// @param goalvalue the value that should be reached
/// @param measurement the current value
/// @return change amount
float PIDController_Compute(PIDController *pid, float goalvalue, float measurement)
{
	float error = goalvalue - measurement;

	// Proportional
	float proportional = pid->Kp * error;

	// Integral
	pid->integrator += pid->Ki / pid->Hz * error;

	// Differential
	float differential = pid->Kd * (error - pid->prevError);

	float output = proportional + pid->integrator - differential;
	if (output < -1.0f)
	{
		SetError();
		return -1.0f;
	}
	if (output > 1.0f)
	{
		SetError();
		return 1.0f;
	}

	ClearError();
	return output;
}
/// @brief Enables the Interrupt on Channel A
void PID_Start()
{
	TIMSK1 |= 0b1 << OCIE1A;
}
/// @brief Disables the Interrupt on Channel A
void PID_Stop()
{
	TIMSK1 &= ~(0b1 << OCIE1A);
}

/// @brief Timer1 Initialization for the PID controller
void PID_Timer_Init()
{
	// TCCR1B = 0b11 << WGM12 | 0b100 << CS10;
	// ICR1 = 15625;
	TCCR1B = 0b11 << WGM12 | 0b011 << CS10;
	ICR1 = 6250;
}