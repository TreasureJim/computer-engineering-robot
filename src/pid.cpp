#include "pid.h"

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

	return proportional + pid->integrator - differential;
}