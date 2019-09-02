/*===============================================================
Copyright (c) 2019 All rights reserved by W3DWILL.

File:			quadrangle.h
Language:		C++
Library:		Open Inventor C++ 10.3.1 Visual Studio 2017
Author:			LIM TAE KYUN
First Date:		2019-09-06
Last Modify:	2019-09-30
===============================================================*/
#ifndef _QUADRANGLE_H_
#define _QUADRANGLE_H_

#include "base_shape.h"
#include <Inventor/fields/SoSFFloat.h>

class Quadrangle : public BaseShape
{
	SO_NODE_HEADER(Quadrangle);
	Quadrangle(const Quadrangle&) = delete;
	Quadrangle& operator = (const Quadrangle&) = delete;

public:
	static void InitClass();
	static void ExitClass();
	Quadrangle();
	Quadrangle(float width, float height);

	inline void SetSize(float width, float height)
	{
		width_.setValue(width);
		height_.setValue(height);
	}

protected:
	virtual ~Quadrangle();

private:
	virtual void notify(SoNotList* list) override;
	virtual void InitBaseProperties() override;
	virtual void FieldChange() override;

	void Initialize();
	//bool EqualWidth(const float width) const;
	//bool EqualHeight(const float height) const;

public:
	SoSFFloat width_;	//default 1
	SoSFFloat height_;	//default 1

};

#endif // !_QUADRANGLE_H_