/*===============================================================
Copyright (c) 2019 All rights reserved by W3DWILL.

File:			text_2d.h
Language:		C++
Library:		Open Inventor C++ 10.3.1 Visual Studio 2017
Author:			LIM TAE KYUN
First Date:		2019-09-16
Last Modify:	2019-09-30
===============================================================*/
#ifndef _TEXT_2D_H_
#define _TEXT_2D_H_

#include "base_shape.h"
#include <Inventor/fields/SoSFString.h>

class SoFont;
class SoText2;
class Text2D : public BaseShape
{
	SO_NODE_HEADER(Text2D);
	Text2D(const Text2D&) = delete;
	Text2D& operator = (const Text2D&) = delete;

public:
	enum Justification
	{
		LEFT = 0x01,
		CENTER = 0x03,
		RIGHT = 0x02,
		INHERITED = 0x04
	};

	static void InitClass();
	static void ExitClass();
	Text2D();
	explicit Text2D(const SbString& str);

	virtual void SetPos(const SbVec3f& pos) override;
	virtual void SetPos(float pos[3]) override;
	virtual void SetPos(float x, float y, float z) override;

	void SetFontSize(const float size);
	void SetColor(const SbColor& color);
	void SetJustification(const Justification& justification);

	inline void set_string(const SbString& str)
	{
		if (EqualString(str) == false)
		{
			string_.setValue(str);
		}
	}

protected:
	virtual ~Text2D();

private:
	virtual void notify(SoNotList* list) override;
	virtual void InitBaseProperties() override;
	virtual void FieldChange() override;

	void Initialize();
	bool EqualString(const SbString& str) const;

public:
	SoSFString string_;	//default "";

private:
	SoFont* font_ = nullptr;
	SoText2* text2_ = nullptr;
};

#endif // !_TEXT_2D_H_
