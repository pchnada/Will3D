#include "circular_arc.h"

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>

#include "functions.h"

namespace
{
	const float kFixedRadius = 0.5f;
}

SO_NODE_SOURCE(CircularArc);
void CircularArc::InitClass()
{
	if (BaseShape::getClassTypeId().isBad())
	{
		BaseShape::InitClass();
	}
	getClassRenderEngineMode().setRenderMode(SbRenderEngineMode::OIV_OPENINVENTOR_RENDERING);
	SO_NODE_INIT_CLASS(CircularArc, BaseShape, "BaseShape");
}

void CircularArc::ExitClass()
{
	SO__NODE_EXIT_CLASS(CircularArc);
}

CircularArc::CircularArc()
{
	Initialize();
}

CircularArc::CircularArc(const float radian, const float radius /*= 0.5f*/)
{
	Initialize();
	radius_.setValue(radius);
	radian_.setValue(radian);
}

CircularArc::~CircularArc()
{
	
}

void CircularArc::SetColor(const SbColor& color)
{
	color_ = color;
	SoLineSet* line_set = FindShape();
	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(line_set->vertexProperty.getValue());
	vertex_property->orderedRGBA.setValue(color_.getPackedValue(transparency_));
}

void CircularArc::SetTransparency(const float transparency)
{
	if (CompareFloat(transparency, 0) == -1)
	{
		return;
	}

	transparency_ = transparency;
	SoLineSet* line_set = FindShape();
	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(line_set->vertexProperty.getValue());
	vertex_property->orderedRGBA.setValue(color_.getPackedValue(transparency_));
}


void CircularArc::ReverseSide()
{
	is_reverse_side = !is_reverse_side;
	ChanegeVerticesPorperty();
}

void CircularArc::ReverseUpSide()
{
	is_reverse_upside = !is_reverse_upside;
	ChanegeVerticesPorperty();
}

void CircularArc::ReverseAll()
{
	is_reverse_side = !is_reverse_side;
	is_reverse_upside = !is_reverse_upside;
	ChanegeVerticesPorperty();
}

void CircularArc::notify(SoNotList* list)
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
			else if (&radian_ == field)
			{
				ChanegeVerticesPorperty();
			}
		}
	}
}

void CircularArc::InitBaseProperties()
{
	setName(SbName("CircularArc"));

	SoLineSet* line_set = new SoLineSet();
	SoVertexProperty* vertex_property = new SoVertexProperty();
	line_set->vertexProperty.setValue(vertex_property);
	
	addChild(line_set);
}

void CircularArc::FieldChange()
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

void CircularArc::Initialize()
{
	SO_NODE_CONSTRUCTOR(CircularArc);
	SO_NODE_ADD_FIELD(radius_, (0.5f));
	SO_NODE_ADD_FIELD(radian_, (0.f));

	InitBaseProperties();
}

void CircularArc::CreateVertices()
{
	float radian = radian_.getValue();
	
	if (radian > kPi * 2.f)
	{
		radian = kPi * 2.f;
	}
	else if (radian < -kPi * 2.f)
	{
		radian = -kPi * 2.f;
	}

	if (radian < 0)
	{
		radian = kPi * 2.f + radian;
	}

	float degree = Degree(radian);
	size_ = abs(static_cast<int>(degree)) + 1;
	
	SafeDeleteArray(vertices_);
	vertices_ = new SbVec3f[size_];

	if (is_reverse_side == false && is_reverse_upside == false)
	{
		for (int i = 0; i < size_; ++i)
		{
			vertices_[i] = SbVec3f(
				kFixedRadius * cos(radian / (size_ - 1) * i),
				kFixedRadius * sin(radian / (size_ - 1) * i),
				0.f);
		}
	}
	else if (is_reverse_side == true && is_reverse_upside == false)
	{
		for (int i = 0; i < size_; ++i)
		{
			vertices_[i] = SbVec3f(
				kFixedRadius * -cos(radian / (size_ - 1) * i),
				kFixedRadius * sin(radian / (size_ - 1) * i),
				0.f);
		}
	}
	else if (is_reverse_side == false && is_reverse_upside == true)
	{
		for (int i = 0; i < size_; ++i)
		{
			vertices_[i] = SbVec3f(
				kFixedRadius * cos(radian / (size_ - 1) * i),
				kFixedRadius * -sin(radian / (size_ - 1) * i),
				0.f);
		}
	}
	else if (is_reverse_side == true && is_reverse_upside == true)
	{
		for (int i = 0; i < size_; ++i)
		{
			vertices_[i] = SbVec3f(
				kFixedRadius * -cos(radian / (size_ - 1) * i),
				kFixedRadius * -sin(radian / (size_ - 1) * i),
				0.f);
		}
	}	
}

void CircularArc::ChanegeVerticesPorperty()
{
	CreateVertices();	

	SoLineSet* line_set = FindShape();
	line_set->numVertices.deleteValues(0);
	
	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(line_set->vertexProperty.getValue());
	vertex_property->vertex.deleteValues(0);

	if (size_ <= 1)
	{
		return;
	}

	line_set->numVertices.setValue(size_);
	vertex_property->vertex.setValues(0, size_, vertices_);
}

SoLineSet* CircularArc::FindShape()
{
	SoNode* node = nullptr;
	int size = getNumChildren();
	for (int i = 0; i < size; ++i)
	{
		node = getChild(i);
		if (node->isOfType(SoLineSet::getClassTypeId()))
		{
			return static_cast<SoLineSet*>(node);
		}
	}

	return nullptr;
}