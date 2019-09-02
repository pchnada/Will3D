#include "text_2d.h"

#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText2.h>
#include "functions.h"

SO_NODE_SOURCE(Text2D);
void Text2D::InitClass()
{
	if (BaseShape::getClassTypeId().isBad())
	{
		BaseShape::InitClass();
	}
	getClassRenderEngineMode().setRenderMode(SbRenderEngineMode::OIV_OPENINVENTOR_RENDERING);
	SO_NODE_INIT_CLASS(Text2D, BaseShape, "BaseShape");
}

void Text2D::ExitClass()
{
	SO__NODE_EXIT_CLASS(Text2D);
}

Text2D::Text2D()
{
	Initialize();
}

Text2D::Text2D(const SbString& str)
{
	Initialize();
	set_string(str);
}

Text2D::~Text2D()
{

}

void Text2D::SetPos(const SbVec3f& pos)
{
	SoNode* translation = FindPropertyNode(SoTranslation::getClassTypeId());
	if (translation == nullptr)
	{
		translation = new SoTranslation();
		insertChild(translation, 0);
	}

	static_cast<SoTranslation*>(translation)->translation.setValue(pos);
}

void Text2D::SetPos(float pos[3])
{
	SoNode* translation = FindPropertyNode(SoTranslation::getClassTypeId());
	if (translation == nullptr)
	{
		translation = new SoTranslation();
		insertChild(translation, 0);
	}

	static_cast<SoTranslation*>(translation)->translation.setValue(pos);
}

void Text2D::SetPos(float x, float y, float z)
{
	SoNode* translation = FindPropertyNode(SoTranslation::getClassTypeId());
	if (translation == nullptr)
	{
		translation = new SoTranslation();
		insertChild(translation, 0);
	}

	static_cast<SoTranslation*>(translation)->translation.setValue(x, y, z);
}

void Text2D::SetFontSize(const float size)
{
	if (CompareFloat(size, 0) == 1)
	{
		font_->size.setValue(size);
	}
}

void Text2D::SetColor(const SbColor& color)
{
	SoNode* meterial = FindPropertyNode(SoMaterial::getClassTypeId());
	if (meterial == nullptr)
	{
		meterial = new SoMaterial();
		insertChild(meterial, 0);
	}

	static_cast<SoMaterial*>(meterial)->diffuseColor.setValue(color);
}

void Text2D::SetJustification(const Justification& justification)
{
	text2_->justification = justification;
}

void Text2D::Initialize()
{
	SO_NODE_CONSTRUCTOR(Text2D);
	SO_NODE_ADD_FIELD(string_, (SbString("")));

	InitBaseProperties();
}

void Text2D::notify(SoNotList* list)
{
	BaseShape::notify(list);
	SoField* field = list->getLastField();
	if (field)
	{
		if (field->isOfType(SoSFString::getClassTypeId()))
		{
			if (&string_ == field)
			{
				FieldChange();
			}
		}
	}
}

void Text2D::InitBaseProperties()
{
	setName(SbName("Text2D"));

	font_ = new SoFont();
	font_->size.setValue(20.f);

	text2_ = new SoText2();
	text2_->justification = Text2D::CENTER;

	SoSeparator* text2_set = new SoSeparator();
	text2_set->addChild(text2_);

	addChild(font_);
	addChild(text2_set);
}

void Text2D::FieldChange()
{
	text2_->string.setValue(string_.getValue());
}

bool Text2D::EqualString(const SbString& str) const
{
	string str1 = string_.getValue().getSString();
	string str2 = str.getSString();

	if (str1.compare(str2.c_str()) == 0)
	{
		return true;
	}

	return false;
}