#pragma once
//config file path
#define A8_PROPERTIES ("A8_PROPERTIES")
#define a8_properties ("a8.properties")
/********************************************************/
/* FCS - flight control system                          */
#define P_fcs_servo_idx_fr ("fcs/servo-fr-idx")
#define P_fcs_servo_idx_fl ("fcs/servo-fl-idx")
#define P_fcs_servo_idx_ar ("fcs/servo-ar-idx")
#define P_fcs_servo_idx_al ("fcs/servo-al-idx")
#define P_fcs_servo_fr_clockwise ("fcs/servo-fr-clockwise")
#define P_fcs_altitude_pid_k3 ("fcs/altitude_pid_k3")
#define P_fcs_roll_pid_k3 ("fcs/roll_pid_k3")
#define P_fcs_pitch_pid_k3 ("fcs/pitch_pid_k3")
#define P_fcs_yaw_pid_k3 ("fcs/yaw_pid_k3")
/* Attitude controller tick rate.*/
#define P_fcs_att_tick_rate ("fcs/att-tick-rate") 
/********************************************************/
/* GS - ground station*/
/* fg - foreground*/
#define P_gs_enable_fg ("gs/enable-fg")

//simulation
#define P_sim_jsb_sim_bind_port ("sim/jsb-sim-bind-port")
#define P_sim_jsb_sim_data_file ("sim/jsb-sim-data-file")

namespace a8::fc {

} // namespace a8::fc
