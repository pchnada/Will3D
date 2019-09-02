#include "line.h"

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>

#include "functions.h"

namespace
{
	const int kNumVertices = 2;
}

SO_NODE_SOURCE(Line)
void Line::InitClass()
{
	if (BaseShape::getClassTypeId().isBad())
	{
		BaseShape::InitClass();
	}

	getClassRenderEngineMode().setRenderMode(SbRenderEngineMode::OIV_OPENINVENTOR_RENDERING);
	SO_NODE_INIT_CLASS(Line, BaseShape, "BaseShape");
}

void Line::ExitClass()
{
	SO__NODE_EXIT_CLASS(Line);
}

Line::Line()
{
	Initialize();
}

Line::Line(const float length)
{
	Initialize();
	length_.setValue(length);
}

Line::Line(const LineType& type)
{
	Initialize();
	line_type_.setValue(static_cast<int>(type));
}

Line::Line(const float length, const LineType& type)
{
	Initialize();
	SetLine(length, type);
}

Line::~Line()
{

}

void Line::SetColor(const SbColor& color)
{
	SoLineSet* line_set = FindShape();
	if (IsNull(line_set))
	{
		return;
	}

	color_ = color;
	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(line_set->vertexProperty.getValue());
	vertex_property->orderedRGBA.setValue(color_.getPackedValue(transparency_));
}

void Line::SetTransparency(float transparency)
{
	SoLineSet* line_set = FindShape();
	if (IsNull(line_set) || EqualFloat(transparency_, transparency))
	{
		return;
	}

	transparency_ = transparency;
	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(line_set->vertexProperty.getValue());
	vertex_property->orderedRGBA.setValue(color_.getPackedValue(transparency_));
}

void Line::SetLineWidth(const float width)
{
	if (CompareFloat(width, 0) != 1)
	{
		return;
	}

	SoNode* draw_style = FindPropertyNode(SoDrawStyle::getClassTypeId());
	if (IsNull(draw_style))
	{
		draw_style = new SoDrawStyle();
		insertChild(draw_style, 0);
	}

	static_cast<SoDrawStyle*>(draw_style)->lineWidth.setValue(width);
}

void Line::notify(SoNotList* list)
{
	BaseShape::notify(list);
	SoField* field = list->getLastField();
	if (field)
	{
		if (&length_ == field)
		{
			FieldChange();
		}
		else if (&line_type_ == field)
		{
			LineTypeChange();
		}
	}
}

void Line::InitBaseProperties()
{
	setName(SbName("Line"));

	vertices_ = new SbVec3f[kNumVertices];
	vertices_[0] = { 0.5f, 0.f, 0.f };
	vertices_[1] = { -0.5f, 0.f, 0.f };

	SoLineSet* line_set = new SoLineSet();
	SoVertexProperty* vertex_propoerty = new SoVertexProperty();
	vertex_propoerty->vertex.setValues(0, kNumVertices, vertices_);
	line_set->vertexProperty.setValue(vertex_propoerty);

	addChild(line_set);
}

void Line::FieldChange()
{
	SoNode* transform = FindPropertyNode(SoTransform::getClassTypeId());
	if (IsNull(transform))
	{
		transform = new SoTransform();
		insertChild(transform, 0);
	}

	float length = fabs(length_.getValue());
	int line_type = line_type_.getValue();
	if (line_type == static_cast<int>(LineType::HORIZONTAL))
	{
		static_cast<SoTransform*>(transform)->scaleFactor.setValue(length, 1.f, 1.f);
	}
	else if (line_type == static_cast<int>(LineType::VERTICAL))
	{
		static_cast<SoTransform*>(transform)->scaleFactor.setValue(1.f, length, 1.f);
	}
}

void Line::Initialize()
{
	SO_NODE_CONSTRUCTOR(Line);
	SO_NODE_ADD_FIELD(length_, (1.f));
	SO_NODE_ADD_FIELD(line_type_, (static_cast<int>(LineType::HORIZONTAL)));

	SO_NODE_DEFINE_ENUM_VALUE(LineType, static_cast<int>(LineType::VERTICAL));
	SO_NODE_DEFINE_ENUM_VALUE(LineType, static_cast<int>(LineType::HORIZONTAL));
	SO_NODE_SET_SF_ENUM_TYPE(line_type_, LineType);

	InitBaseProperties();
}

void Line::LineTypeChange()
{
	SoLineSet* line_set = FindShape();
	if (IsNull(line_set))
	{
		return;
	}

	int line_type = line_type_.getValue();
	if (line_type == static_cast<int>(LineType::HORIZONTAL))
	{
		vertices_[0] = { -0.5f, 0.f, 0.f };
		vertices_[1] = { 0.5f, 0.f, 0.f };
	}
	else if (line_type == static_cast<int>(LineType::VERTICAL))
	{
		vertices_[0] = { 0.f, -0.5f, 0.f };
		vertices_[1] = { 0.f, 0.5f, 0.f };
	}

	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(line_set->vertexProperty.getValue());
	vertex_property->vertex.setValues(0, kNumVertices, vertices_);

	FieldChange();
}

SoLineSet* Line::FindShape()
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

//bool Line::EqualLength(const float length) const
//{
//	return EqualFloat(length_.getValue(), length);
//}
//
//bool Line::EqualType(const LineType& type) const
//{
//	if (line_type_.getValue() == static_cast<int>(type))
//	{
//		return true;
//	}
//
//	return false;
//}