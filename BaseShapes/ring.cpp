#include "ring.h"

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

#include "functions.h"

namespace
{
	const int kMinimum = 16;
	const int kMaximum = 360;
	const int kSpace = 5;
	const float kFiexdRadius = 0.5f;
}

SO_NODE_SOURCE(Ring);
void Ring::InitClass()
{
	if (BaseShape::getClassTypeId().isBad())
	{
		BaseShape::InitClass();
	}
	getClassRenderEngineMode().setRenderMode(SbRenderEngineMode::OIV_OPENINVENTOR_RENDERING);
	SO_NODE_INIT_CLASS(Ring, BaseShape, "BaseShape");
}

void Ring::ExitClass()
{
	SO__NODE_EXIT_CLASS(Ring);
}

Ring::Ring()
{
	Initialize();	
}

Ring::Ring(const float radius)
{
	Initialize();
	radius_.setValue(radius);
}

Ring::Ring(const float radius, const int exquisite)
{
	Exquisite(exquisite);
	Initialize();
	radius_.setValue(radius);
}

Ring::~Ring()
{
	SafeDeleteArray(index_);
}

void Ring::SetColor(const SbColor& color)
{
	color_ = color;
	SoIndexedFaceSet* face_set = FindShape();
	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(face_set->vertexProperty.getValue());
	vertex_property->orderedRGBA.setValue(color_.getPackedValue(transparency_));
}

void Ring::SetTransparency(float transparency)
{
	if (CompareFloat(transparency, 0) == -1)
	{
		return;
	}

	transparency_ = transparency;
	SoIndexedFaceSet* face_set = FindShape();
	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(face_set->vertexProperty.getValue());
	vertex_property->orderedRGBA.setValue(color_.getPackedValue(transparency_));
}

void Ring::SetExquisite(const int exquisite)
{
	Exquisite(exquisite);
	ChanegeVerticesPorperty();
}

void Ring::notify(SoNotList* list)
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
			else if (&thickness_ == field)
			{
				ChanegeVerticesPorperty();
			}				
		}
	}
}

void Ring::InitBaseProperties()
{
	setName(SbName("Ring"));

	CreateVertices();

	SoVertexProperty* vertex_property = new SoVertexProperty();	
	vertex_property->vertex.setValues(0, exquisite_ * 2, vertices_);

	SoIndexedFaceSet* face_set = new SoIndexedFaceSet();
	face_set->coordIndex.setValues(0, exquisite_ * kSpace, index_);
	face_set->vertexProperty.setValue(vertex_property);	

	addChild(face_set);
}

void Ring::FieldChange()
{
	SoNode* transform = FindPropertyNode(SoTransform::getClassTypeId());
	if (IsNull(transform))
	{
		transform = new SoTransform();
		insertChild(transform, 0);
	}

	float diameter = fabs(radius_.getValue()) * 2.f;
	static_cast<SoTransform*>(transform)->scaleFactor.setValue(diameter, diameter, 1.f);
}

void Ring::Initialize()
{
	SO_NODE_CONSTRUCTOR(Ring);
	SO_NODE_ADD_FIELD(radius_, (0.5f));
	SO_NODE_ADD_FIELD(thickness_, (0.1f));
	InitBaseProperties();
}

void Ring::Exquisite(const int exquisite /*= kMinimum*/)
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

void Ring::CreateVertices()
{
	SafeDeleteArray(vertices_);
	SafeDeleteArray(index_);

	int size = exquisite_ * 2;
	vertices_ = new SbVec3f[size];

	float ratio = 1.f;
	SoNode* transform = FindPropertyNode(SoTransform::getClassTypeId());
	if (IsNull(transform) == false)
	{
		SbVec3f scale = static_cast<SoTransform*>(transform)->scaleFactor.getValue();
		ratio /= scale[0];
	}

	float half = fabs(thickness_.getValue() / 2.f) * ratio;
	for (int i = 0; i < exquisite_; ++i)
	{
		vertices_[i] = SbVec3f(
			(kFiexdRadius - half) * cos(kPi * 2.f / exquisite_ * i),
			(kFiexdRadius - half) * sin(kPi * 2.f / exquisite_ * i),
			0.f);

		vertices_[i + exquisite_] = SbVec3f(
			(kFiexdRadius + half) * cos(kPi * 2.f / exquisite_ * i),
			(kFiexdRadius + half) * sin(kPi * 2.f / exquisite_ * i),
			0.f);		
	}

	int cnt = exquisite_ * kSpace;
	index_ = new int32_t[cnt];
	int j = 0;
	for (int i = 0; i < cnt - kSpace; i += kSpace)
	{
		index_[i] = j + 1;
		index_[i + 1] = j;
		index_[i + 2] = j + exquisite_;
		index_[i + 3] = j + exquisite_ + 1;
		index_[i + 4] = SO_END_FACE_INDEX;
		++j;
	}

	index_[cnt - kSpace] = 0;
	index_[cnt - kSpace + 1] = j;
	index_[cnt - kSpace + 2] = j + exquisite_;
	index_[cnt - kSpace + 3] = exquisite_;
	index_[cnt - kSpace + 4] = SO_END_FACE_INDEX;
}

void Ring::ChanegeVerticesPorperty()
{
	CreateVertices();

	SoIndexedFaceSet* face_set = FindShape();
	face_set->coordIndex.deleteValues(0);
	face_set->coordIndex.setValues(0, exquisite_ * kSpace, index_);

	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(face_set->vertexProperty.getValue());
	vertex_property->vertex.deleteValues(0);
	vertex_property->vertex.setValues(0, exquisite_ * 2, vertices_);

	face_set->vertexProperty.setValue(vertex_property);
}

SoIndexedFaceSet* Ring::FindShape()
{
	SoNode* node = nullptr;
	int size = getNumChildren();
	for (int i = 0; i < size; ++i)
	{
		node = getChild(i);
		if (node->isOfType(SoIndexedFaceSet::getClassTypeId()))
		{
			return static_cast<SoIndexedFaceSet*>(node);
		}
	}

	return nullptr;
}

//bool Ring::EqualRadius(const float radius) const
//{
//	if (EqualFloat(radius_.getValue(), radius))
//	{
//		return true;
//	}
//
//	return false;
//}

