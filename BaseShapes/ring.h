/*===============================================================
Copyright (c) 2019 All rights reserved by W3DWILL.

File:			ring.h
Language:		C++
Library:		Open Inventor C++ 10.3.1 Visual Studio 2017
Author:			LIM TAE KYUN
First Date:		2019-09-30
Last Modify:	2019-09-30
===============================================================*/
#ifndef _RING_H_
#define _RING_H_

#include "base_shape.h"

class SoIndexedFaceSet;
class Ring : public BaseShape
{
	SO_NODE_HEADER(Ring);
	Ring(const Ring&) = delete;
	Ring& operator = (const Ring&) = delete;

public:
	static void InitClass();
	static void ExitClass();
	Ring();
	explicit Ring(const float radius);
	Ring(const float radius, const int exquisite);

	virtual void SetColor(const SbColor& color) override;
	virtual void SetTransparency(const float transparency) override;
	void SetExquisite(const int exquisite); // minimum 16 ~ maximum 360

protected:
	virtual ~Ring();

private:
	virtual void notify(SoNotList* list) override;
	virtual void InitBaseProperties() override;
	virtual void FieldChange() override;

	void Initialize();
	void Exquisite(const int exquisite = 16);
	void CreateVertices();
	void ChanegeVerticesPorperty();
	SoIndexedFaceSet* FindShape();
	//bool EqualRadius(const float radius) const;

public:
	SoSFFloat radius_;	//default 0.5f;
	SoSFFloat thickness_; //default 0.1f;

private:
	int exquisite_ = 16;
	int32_t* index_ = nullptr;

};

#endif // !_RING_H_