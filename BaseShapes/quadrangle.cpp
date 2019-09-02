#include "quadrangle.h"

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>

#include "functions.h"

namespace
{
	const int kNumVertices = 4;
}

SO_NODE_SOURCE(Quadrangle);
void Quadrangle::InitClass()
{
	if (BaseShape::getClassTypeId().isBad())
	{
		BaseShape::InitClass();
	}

	getClassRenderEngineMode().setRenderMode(SbRenderEngineMode::OIV_OPENINVENTOR_RENDERING);
	SO_NODE_INIT_CLASS(Quadrangle, BaseShape, "BaseShape");
}

void Quadrangle::ExitClass()
{
	SO__NODE_EXIT_CLASS(Quadrangle);
}

Quadrangle::Quadrangle()
{
	Initialize();
}

Quadrangle::Quadrangle(float width, float height)
{
	Initialize();
	SetSize(width, height);
}

Quadrangle::~Quadrangle()
{

}

void Quadrangle::notify(SoNotList* list)
{
	BaseShape::notify(list);
	SoField* field = list->getLastField();
	if (IsNull(field) == false)
	{
		if (field->isOfType(SoSFFloat::getClassTypeId()))
		{
			if (&width_ == field || &height_ == field)
			{
				FieldChange();				
			}
		}
	}
}

void Quadrangle::InitBaseProperties()
{
	setName(SbName("Quadrangle"));

	vertices_ = new SbVec3f[kNumVertices];
	vertices_[0] = { -0.5f, 0.5f, 0.f };
	vertices_[1] = { -0.5f, -0.5f, 0.f };
	vertices_[2] = { 0.5f, -0.5f, 0.f };
	vertices_[3] = { 0.5f, 0.5f, 0.f };

	SoCoordinate3* coords = new SoCoordinate3();
	coords->point.setValues(0, kNumVertices, vertices_);

	SoFaceSet* face_set_ = new SoFaceSet();
	face_set_->numVertices.setValue(kNumVertices);

	SafeDeleteArray(vertices_);

	addChild(coords);
	addChild(face_set_);
}

void Quadrangle::FieldChange()
{
	SoNode* transform = FindPropertyNode(SoTransform::getClassTypeId());
	if (IsNull(transform))
	{
		transform = new SoTransform();
		insertChild(transform, 0);
	}

	float width = fabs(width_.getValue());
	float height = fabs(height_.getValue());

	static_cast<SoTransform*>(transform)->scaleFactor.setValue(width, height, 1.f);
}

void Quadrangle::Initialize()
{
	SO_NODE_CONSTRUCTOR(Quadrangle);
	SO_NODE_ADD_FIELD(width_, (1.f));
	SO_NODE_ADD_FIELD(height_, (1.f));

	InitBaseProperties();
}

//bool Quadrangle::EqualWidth(const float width) const
//{
//	return EqualFloat(width_.getValue(), width);
//}
//
//bool Quadrangle::EqualHeight(const float height) const
//{
//	return EqualFloat(height_.getValue(), height);
//}