/*===============================================================
Copyright (c) 2019 All rights reserved by W3DWILL.

File:			functions.h
Language:		C++
Library:		Open Inventor C++ 10.3.1 Visual Studio 2017
Author:			LIM TAE KYUN
First Date:		2019-09-18
Last Modify:	2019-10-01
===============================================================*/
#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <math.h>
#include "define.h"

namespace
{
	const int kError = -999999999;
	const float kErrorRangeFloat = 0.000001f;	// float 오차 범위
}

static bool EqualFloat(const float f1, const float f2)
{
	if (fabsf(f1 - f2) > kErrorRangeFloat)
	{
		return false;
	}
	return true;
}

// f1 == f2 = 0
// f1 > f2 = 1 
// f1 < f2 = -1
static int CompareFloat(const float f1, const float f2)
{
	if (EqualFloat(f1, f2))
	{
		return 0;
	}
	else if (f1 - f2 > 0)
	{
		return 1;
	}
	else if (f1 - f2 < 0)
	{
		return -1;
	}

	return kError;
}

template<typename T> void SafeDelete(T* p)
{
	if (p != nullptr)
	{
		delete p;
		p = nullptr;
	}	
}

template<typename T> void SafeDeleteArray(T* p)
{
	if (p != nullptr)
	{
		delete[] p;
		p = nullptr;
	}
}

template<typename T> bool IsNull(T* p)
{
	if (p == nullptr)
	{
		return true;
	}
	return false;
}

static const float Radian(const float degree)
{	
	if (EqualFloat(degree, 0.f))
	{
		return 0.f;
	}

	return degree * kPi / 180.f;
}

static const float Degree(const float Radian)
{
	if (EqualFloat(Radian, 0.f))
	{
		return 0.f;
	}

	return Radian * 180.f / kPi;
}

#endif // !_FUNCTIONS_H_