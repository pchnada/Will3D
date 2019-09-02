/*===============================================================
Copyright (c) 2019 All rights reserved by W3DWILL.

File:			spline.h
Language:		C++
Library:		Open Inventor C++ 10.3.1 Visual Studio 2017
Author:			LIM TAE KYUN
First Date:		2019-09-18
Last Modify:	2019-09-30
===============================================================*/
#ifndef _SPLINE_H_
#define _SPLINE_H_

#include "base_shape.h"
#include <Inventor/fields/SoSFDouble.h>

class SoLineSet;
class SoPolylineResamplerApproximation3d;
class Spline : public BaseShape
{
	SO_NODE_HEADER(Spline);
	Spline(const Spline&) = delete;
	Spline& operator = (const Spline&) = delete;

public:
	static void InitClass();
	static void ExitClass();
	Spline();
	explicit Spline(const double spacing);

	void AddInPoint(const SbVec3f& point);
	bool InsertInPoint(const int index, const SbVec3f& point);
	bool ChangeInPoint(const int index, const SbVec3f& point);
	bool SetInPoints(const int start, const int num, SbVec3f* points = nullptr); //최소 2개의 점 이상
	void RemoveInPoint(const int index);
	void RemoveAllInPoints();

	inline const int in_points_size() const { return in_points_size_; }
	const int out_points_size() const;
	const SbVec3f* out_points() const;

	virtual void SetColor(const SbColor& color) override;
	virtual void SetTransparency(const float transparency) override;
	void SetLineWidth(const float width);
	float LineWidth();

protected:
	virtual ~Spline();

private:
	virtual void notify(SoNotList* list) override;
	virtual void InitBaseProperties() override;
	virtual void FieldChange() override;

	void Initialize();
	void SetSpline();
	SoLineSet* FindShape();
	//bool EqualSpacing(const double spacing) const;

public:
	SoSFDouble spacing_; //default 0.05;

private:
	SoRef<SoPolylineResamplerApproximation3d> resampler_ = nullptr;
	int in_points_size_ = 0;

};

#endif // !_SPLINE_H_ng7