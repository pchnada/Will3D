/*===============================================================
Copyright (c) 2019 All rights reserved by W3DWILL.

File:			define.h
Language:		C++
Library:		Open Inventor C++ 10.3.1 Visual Studio 2017
Author:			LIM TAE KYUN
First Date:		2019-08-07
Last Modify:	2019-08-16
===============================================================*/
#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <math.h>

using namespace std;

const float kPi			= 3.1415926f;	// pi
const float kPi_2		= 1.5707963f;	// pi / 2

//color
const float kBlack[3] = { 0.f, 0.f, 0.f };
const float kWhite[3] = { 1.f, 1.f, 1.f };
const float kRed[3] = { 1.f, 0.f, 0.f };
const float kGreen[3] = { 0.f, 1.f, 0.f };
const float kBlue[3] = { 0.f, 0.f, 1.f };
const float kYellow[3] = { 1.f, 1.f, 0.f };
const float kAqua[3] = { 0.f, 1.f, 1.f };
const float kMagenta[3] = { 1.f, 0.f, 1.f };
const float kGray[3] = { 0.5f, 0.5f, 0.5f };
const float kSkyBlue[3] = { 0.5294f, 0.8078f, 0.9215f };
const float kSteelBlue[3] = { 0.2745f, 0.5098f, 0.7058f };
const float kViolet[3] = { 0.9333f, 0.5098f, 0.9333f };
const float kBeige[3] = { 0.9607f, 0.9607f, 0.8627f };
const float kOrange[3] = { 1.f, 0.6470f, 0.f };

#include <Inventor/SbVec.h>
const SbVec3f kVecZero = { 0.f, 0.f, 0.f };
const SbVec3f kAxisX = { 1.f, 0.f, 0.f };
const SbVec3f kAxisY = { 0.f, 1.f, 0.f };
const SbVec3f kAxisZ = { 0.f, 0.f, 1.f };

#endif // !_DEFINE_H_