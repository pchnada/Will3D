/*===============================================================
Copyright (c) 2019 All rights reserved by W3DWILL.

File:			undefined.h
Language:		C++
Library:		Open Inventor C++ 10.3.1 Visual Studio 2017
Author:			LIM TAE KYUN
First Date:		2019-09-24
Last Modify:	2019-09-30
===============================================================*/
#ifndef _UNDEFINED_H_
#define _UNDEFINED_H_

#include "base_shape.h"
#include <vector>
#include <Inventor/fields/SoSFFloat.h>
#include "functions.h"

class SoFieldSensor;
class SoLineSet;
class SoFaceSet;
class SoVertexProperty;
class UnDefined : public BaseShape
{
	SO_NODE_HEADER(UnDefined);
	UnDefined(const UnDefined&) = delete;
	UnDefined& operator = (const UnDefined&) = delete;

public:
	enum class ShapeType
	{
		LINE,
		FACE
	};

	static void InitClass();
	static void ExitClass();

	UnDefined(const int size, SbVec3f* vertices, const ShapeType& Type = ShapeType::LINE);

	virtual void SetColor(const SbColor& color) override;
	virtual void SetTransparency(const float transparency) override;
	
	void ChangeVertices(const int start, const int num, SbVec3f* vertices);
	void ChangeVertices(const int size, SbVec3f* vertices);

	inline const int size() const { return size_; }
	inline void set_shape_type(const ShapeType& type)
	{
		if (EqualType(type) == false)
		{
			shape_type_.setValue(static_cast<int>(type));
		}
	}		

protected:	
	virtual ~UnDefined();

private:
	UnDefined() {};
	virtual void InitBaseProperties() override;

	void Initialize();
	void ShapeTypeChange();
	bool EqualType(const ShapeType& type) const;

	static void TypeSensorCallBack(void* data, SoSensor* sensor);

public:
	SoSFInt32 shape_type_;

private:
	SoFieldSensor* type_sensor_ = nullptr;
	SoSeparator* shape_seperator_ = nullptr;;
	SoLineSet* line_set_ = nullptr;
	SoFaceSet* face_set_ = nullptr;
	SoVertexProperty* vertex_prop_ = nullptr;

	vector<SbVec3f> vertex_list_;

	int size_ = 0;
};

#endif // !_UNDEFINED_H_