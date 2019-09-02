/*===============================================================
Copyright (c) 2019 All rights reserved by W3DWILL.

File:			circle.h
Language:		C++
Library:		Open Inventor C++ 10.3.1 Visual Studio 2017
Author:			LIM TAE KYUN
First Date:		2019-09-16
Last Modify:	2019-09-30
===============================================================*/
#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include "base_shape.h"

class SoFaceSet;
class Circle : public BaseShape
{
	SO_NODE_HEADER(Circle);
	Circle(const Circle&) = delete;
	Circle& operator = (const Circle&) = delete;

public:
	static void InitClass();
	static void ExitClass();
	Circle();
	explicit Circle(const float radius);
	Circle(const float radius, const int exquisite);

	void SetExquisite(const int exquisite); // minimum 16 ~ maximum 360

protected:
	virtual ~Circle();

private:
	virtual void notify(SoNotList* list) override;
	virtual void InitBaseProperties() override;
	virtual void FieldChange() override;

	void Initialize();
	void Exquisite(const int exquisite = 16);
	void CreateVertices();
	void ChanegeVerticesPorperty();
	SoFaceSet* FindShape();
	//bool EqualRadius(const float radius) const;

public:
	SoSFFloat radius_;	//default 0.5f;

private:
	int exquisite_ = 16;

};

#endif // !_CIRCLE_H_