#ifndef PID_H_
#define PID_H_

typedef struct
{
	float Kp;
	float Ki;
	float Kd;

	float Hz;

	float integrator;
	float prevError;

	// float limMin;
	// float limMax;

	// float limMinInt;
	// float limMaxInt;

} PIDController;

void PIDController_Init(PIDController *pid, float Kp, float Ki, float Kd, float Hz);
float PIDController_Compute(PIDController *pid, float goalvalue, float measurement);

#endif