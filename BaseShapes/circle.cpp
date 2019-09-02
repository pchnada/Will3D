#include "circle.h"

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>

#include "functions.h"

namespace
{
	const int kMinimum = 16;
	const int kMaximum = 360;
	const float kFixedRadius = 0.5f;
}

SO_NODE_SOURCE(Circle);
void Circle::InitClass()
{
	if (BaseShape::getClassTypeId().isBad())
	{
		BaseShape::InitClass();
	}
	getClassRenderEngineMode().setRenderMode(SbRenderEngineMode::OIV_OPENINVENTOR_RENDERING);
	SO_NODE_INIT_CLASS(Circle, BaseShape, "BaseShape");
}

void Circle::ExitClass()
{
	SO__NODE_EXIT_CLASS(Circle);
}

Circle::Circle()
{
	Initialize();
}

Circle::Circle(const float radius)
{
	Initialize();
	radius_.setValue(radius);
}

Circle::Circle(const float radius, const int exquisite)
{
	Exquisite(exquisite);
	Initialize();
	radius_.setValue(radius);
}

Circle::~Circle()
{

}

void Circle::SetExquisite(const int exquisite)
{
	Exquisite(exquisite);
	ChanegeVerticesPorperty();
}

void Circle::notify(SoNotList* list)
{
	BaseShape::notify(list);
	SoField* field = list->getLastField();
	if (field)
	{
		if (field->isOfType(SoSFFloat::getClassTypeId()))
		{
			if (&radius_ == field)
			{
				FieldChange();
			}
		}
	}
}

void Circle::InitBaseProperties()
{
	setName(SbName("Circle"));

	CreateVertices();

	SoCoordinate3* coords = new SoCoordinate3();
	coords->point.setValues(0, exquisite_, vertices_);

	SoFaceSet* face_set = new SoFaceSet();
	face_set->numVertices.setValue(exquisite_);

	addChild(coords);
	addChild(face_set);
}

void Circle::FieldChange()
{
	SoNode* transform = FindPropertyNode(SoTransform::getClassTypeId());
	if (IsNull(transform))
	{
		transform = new SoTransform();
		insertChild(transform, 0);
	}

	float diameter = fabs(radius_.getValue() * 2.f);
	static_cast<SoTransform*>(transform)->scaleFactor.setValue(diameter, diameter, 1.f);
}

void Circle::Initialize()
{
	SO_NODE_CONSTRUCTOR(Circle);
	SO_NODE_ADD_FIELD(radius_, (0.5f));

	InitBaseProperties();
}

void Circle::Exquisite(const int exquisite /*= kMinimum*/)
{
	if (exquisite <= kMinimum)
	{
		exquisite_ = kMinimum;
	}
	else if (exquisite >= kMaximum)
	{
		exquisite_ = kMaximum;
	}
	else
	{
		exquisite_ = exquisite;
	}
}

void Circle::CreateVertices()
{
	SafeDeleteArray(vertices_);

	vertices_ = new SbVec3f[exquisite_];
	for (int i = 0; i < exquisite_; ++i)
	{
		vertices_[i] = SbVec3f(
			kFixedRadius * cos(kPi * 2.f / exquisite_ * i),
			kFixedRadius * sin(kPi * 2.f / exquisite_ * i),
			0.f);
	}
}

void Circle::ChanegeVerticesPorperty()
{
	CreateVertices();

	SoNode* node = FindPropertyNode(SoCoordinate3::getClassTypeId());
	SoFaceSet* fece_set = FindShape();

	SoCoordinate3* coords = static_cast<SoCoordinate3*>(node);
	coords->point.deleteValues(0);
	coords->point.setValues(0, exquisite_, vertices_);
	fece_set->numVertices.setValue(exquisite_);
}

SoFaceSet* Circle::FindShape()
{
	SoNode* node = nullptr;
	int size = getNumChildren();
	for (int i = 0; i < size; ++i)
	{
		node = getChild(i);
		if (node->isOfType(SoFaceSet::getClassTypeId()))
		{
			return static_cast<SoFaceSet*>(node);
		}
	}

	return nullptr;
}

//bool Circle::EqualRadius(const float radius) const
//{
//	if (EqualFloat(radius_.getValue(), radius))
//	{
//		return true;
//	}
//
//	return false;
//}