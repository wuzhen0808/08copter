#pragma once
namespace study {
// pid
float PID;
float pid_p = 0;
float pid_i = 0;
float pid_d = 0;
/////////////////PID CONSTANTS/////////////////
double kp = 3.55;  // 3.55
double ki = 0.003; // 0.003
double kd = 2.05;  // 2.05
float previous_error;

void updatePid(float error,float elapsedTime){
    pid_p = kp * error;
    // if (-3 < error < 3) {
    //     pid_i = pid_i + (ki * error);
    // }
    pid_d = kd * ((error - previous_error) / elapsedTime);
    //
    PID = pid_p + pid_i + pid_d;
    if (PID < -1000) {
        PID = -1000;
    }
    if (PID > 1000) {
        PID = 1000;
    }
    previous_error = error;
}
} // namespace study