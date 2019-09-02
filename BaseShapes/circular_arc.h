/*===============================================================
Copyright (c) 2019 All rights reserved by W3DWILL.

File:			circular_arc.h
Language:		C++
Library:		Open Inventor C++ 10.3.1 Visual Studio 2017
Author:			LIM TAE KYUN
First Date:		2019-10-01
Last Modify:	2019-10-02
===============================================================*/
#ifndef _CIRCULAR_ARC_H_
#define _CIRCULAR_ARC_H_

#include "base_shape.h"

class SoLineSet;
class CircularArc : public BaseShape
{
	SO_NODE_HEADER(CircularArc);
	CircularArc(const CircularArc&) = delete;
	CircularArc& operator = (const CircularArc&) = delete;

public:
	static void InitClass();
	static void ExitClass();
	CircularArc();
	CircularArc(const float radian, const float radius = 0.5f);

	virtual void SetColor(const SbColor& color) override;
	virtual void SetTransparency(const float transparency) override;

	void ReverseSide();
	void ReverseUpSide();
	void ReverseAll();

protected:
	virtual ~CircularArc();

private:
	virtual void notify(SoNotList* list) override;
	virtual void InitBaseProperties() override;
	virtual void FieldChange() override;

	void Initialize();
	void CreateVertices();
	void ChanegeVerticesPorperty();
	SoLineSet* FindShape();

public:
	SoSFFloat radius_;	//default 0.5f;
	SoSFFloat radian_;	//default 0.f;

private:
	int size_ = 0;
	bool is_reverse_side = false;
	bool is_reverse_upside = false;

};

#endif // !_CIRCULAR_ARC_H_