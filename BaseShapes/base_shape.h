/*===============================================================
Copyright (c) 2019 All rights reserved by W3DWILL.

File:			base_shape.h
Language:		C++
Library:		Open Inventor C++ 10.3.1 Visual Studio 2017
Author:			LIM TAE KYUN
First Date:		2019-09-06
Last Modify:	2019-10-02
===============================================================*/
#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoDrawStyle.h>

#include "define.h"

class SoFullPath;
class BaseShape : public SoSeparator
{
	SO_NODE_ABSTRACT_HEADER(BaseShape);
	BaseShape(const BaseShape&) = delete;
	BaseShape& operator = (const BaseShape&) = delete;

public:
	static void InitClass();
	static void ExitClass();

	virtual void SetPos(const SbVec3f& pos);
	virtual void SetPos(float pos[3]);
	virtual void SetPos(const float x, const float y, const float z);
	virtual void SetColor(const SbColor& color);
	virtual void SetColor(const float r, const float g, const float b);
	virtual void SetTransparency(const float transparency);

	void SetPickStyle(SoPickStyle::Style style);//SHAPE, BOUNDING_BOX, UNPICKABLE	
	void SetDrawStyle(SoDrawStyle::Style style);//FILLED, LINES, POINTS, INVISIBLE, MARKERS

	bool AddPropertyNode(SoNode* new_node, const SoType& type);
	bool RemovePropertyNode(const SoType& type);
	SoNode* FindPropertyNode(const SoType& type);

	static BaseShape* FindShapeByFullPath(const SoFullPath* full_path);

	inline const SbVec3f& pos() { return pos_; }

protected:
	BaseShape();
	virtual ~BaseShape() = 0;
	virtual void InitBaseProperties() {};
	virtual void FieldChange() {};

	virtual void notify(SoNotList* list) override;
	virtual void addChild(SoNode* child) override;
	virtual void insertChild(SoNode* child, int newChildIndex) override;
	virtual void internalAddChild(SoNode* child) override;
	virtual void internalInsertChild(SoNode* child, int newChildIndex) override;

private:
	bool CheckPropertyNode(const SoType& type) const;

protected:
	SbVec3f* vertices_ = nullptr;
	SbColor color_ = kWhite;
	float transparency_ = 0.f;

private:
	SbVec3f pos_ = kVecZero;
};

#endif // !_SHAPE_H_