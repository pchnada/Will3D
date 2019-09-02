/*===============================================================
Copyright (c) 2019 All rights reserved by W3DWILL.

File:			line.h
Language:		C++
Library:		Open Inventor C++ 10.3.1 Visual Studio 2017
Author:			LIM TAE KYUN
First Date:		2019-09-10
Last Modify:	2019-09-30
===============================================================*/
#ifndef _LINE_H_
#define _LINE_H_

#include "base_shape.h"
#include <Inventor/fields/SoSFFloat.h>

class SoLineSet;
class Line : public BaseShape
{
	SO_NODE_HEADER(Line);
	Line(const Line&) = delete;
	Line& operator = (const Line&) = delete;

public:
	enum class LineType
	{
		VERTICAL,
		HORIZONTAL
	};

	static void InitClass();
	static void ExitClass();
	Line();
	explicit Line(const float length);
	explicit Line(const LineType& style);
	Line(const float length, const LineType& type);

	virtual void SetColor(const SbColor& color) override;
	virtual void SetTransparency(const float transparency) override;
	void SetLineWidth(const float width);

	inline void SetLine(float length, const LineType& type)
	{
		length_.setValue(length);
		line_type_.setValue(static_cast<int>(type));
	}

protected:
	virtual ~Line();

private:
	virtual void notify(SoNotList* list) override;
	virtual void InitBaseProperties() override;
	virtual void FieldChange() override;

	void Initialize();
	void LineTypeChange();
	SoLineSet* FindShape();
	//bool EqualLength(const float length) const;
	//bool EqualType(const LineType& type) const;

public:
	SoSFFloat length_;		//default 1
	SoSFEnum line_type_;	//default LineType::HORIZONTAL
};

#endif // !_LINE_H_