#pragma once
#ifndef MY_COMMON_H
#define MY_COMMON_H
#include <Arduino.h>
#include <Wire.h>
#include <MPU9250.h>

namespace study {

extern MPU9250 *mpu;

void log(const char *msg) ;
void log(const char *msg, bool ln) ;
void log(const int msg) ;
void log(const int msg, bool ln) ;

void log(const long msg) ;
void log(const float msg) ;
void log(const float msg, bool ln) ;

void log(int x, int y, int z, bool ln) ;
void log(const char *prompt, int x, int y, int z, bool ln) ;
void log(float x, float y, float z, bool ln) ;
void log(const char *prompt, float x, float y, float z, bool ln) ;

int setupWire() ;

int setupSerial() ;


int setupMpu9250() ;

bool mpuUpdate();
float mpuGetRoll();
float mpuGetPitch();
float mpuGetYaw();
int setupExample();

} // namespace study
#endif