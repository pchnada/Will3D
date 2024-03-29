#include "undefined.h"

#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "functions.h"

SO_NODE_SOURCE(UnDefined)
void UnDefined::InitClass()
{
	if (BaseShape::getClassTypeId().isBad())
	{
		BaseShape::InitClass();
	}

	getClassRenderEngineMode().setRenderMode(SbRenderEngineMode::OIV_OPENINVENTOR_RENDERING);
	SO_NODE_INIT_CLASS(UnDefined, BaseShape, "BaseShape");
}

void UnDefined::ExitClass()
{
	SO__NODE_EXIT_CLASS(UnDefined);
}

UnDefined::UnDefined(const int size, SbVec3f* vertices, const ShapeType& type /*= ShapeType::LINE*/)
{
	size_ = size;
	vertex_list_.resize(size);
	for (int i = 0; i < size; ++i)
	{
		vertex_list_[i] = vertices[i];
	}
		
	Initialize(type);
}

UnDefined::~UnDefined()
{
	vertex_list_.clear();
	SafeDelete(type_sensor_);
}

void UnDefined::SetColor(const SbColor& color)
{
	color_ = color;
	vertex_prop_->orderedRGBA.setValue(color_.getPackedValue(transparency_));
}

void UnDefined::SetTransparency(const float transparency)
{
	if (CompareFloat(transparency, 0) == -1)
	{
		return;
	}

	transparency_ = transparency;
	vertex_prop_->orderedRGBA.setValue(color_.getPackedValue(transparency_));
}

void UnDefined::ChangeVertices(const int start, const int num, SbVec3f* vertices)
{
	size_ = num;
	vertex_list_.clear();
	vertex_list_.resize(size_);

	for (int i = 0; i < size_; ++i)
	{
		vertex_list_[i] = vertices[start + i];
	}
	
	vertex_prop_->vertex.setValues(0, size_, &vertex_list_.front());
	if (IsNull(line_set_) == false)
	{
		line_set_->numVertices.setValue(size_);
	}
	else if (IsNull(face_set_) == false)
	{
		face_set_->numVertices.setValue(size_);
	}
}

void UnDefined::ChangeVertices(const int size, SbVec3f* vertices)
{	
	size_ = size;
	vertex_list_.clear();
	vertex_list_.resize(size_);

	for (int i = 0; i < size_; ++i)
	{
		vertex_list_[i] = vertices[i];
	}

	vertex_prop_->vertex.setValues(0, size_, &vertex_list_.front());
	if (IsNull(line_set_) == false)
	{
		line_set_->numVertices.setValue(size_);
	}
	else if (IsNull(face_set_) == false)
	{
		face_set_->numVertices.setValue(size_);
	}
}

void UnDefined::InitBaseProperties()
{
	setName(SbName("UnDefined"));

	vertex_prop_ = new SoVertexProperty();
	vertex_prop_->vertex.setValues(0, size_, &vertex_list_.front());	

	int type = shape_type_.getValue();
	if (type == static_cast<int>(ShapeType::LINE))
	{
		line_set_ = new SoLineSet();
		line_set_->vertexProperty.setValue(vertex_prop_);
		addChild(line_set_);
	}
	else if (type == static_cast<int>(ShapeType::FACE))
	{
		face_set_ = new SoFaceSet();
		face_set_->vertexProperty.setValue(vertex_prop_);
		addChild(face_set_);
	}	
}

void UnDefined::Initialize(const ShapeType& type)
{
	SO_NODE_CONSTRUCTOR(UnDefined);
	SO_NODE_ADD_FIELD(shape_type_, (static_cast<int>(ShapeType::LINE)));
	SO_NODE_DEFINE_ENUM_VALUE(ShapeType, static_cast<int>(ShapeType::LINE));
	SO_NODE_DEFINE_ENUM_VALUE(ShapeType, static_cast<int>(ShapeType::FACE));
	SO_NODE_SET_SF_ENUM_TYPE(shape_type_, ShapeType);

	shape_type_.setValue(static_cast<int>(type));

	InitBaseProperties();

	type_sensor_ = new SoFieldSensor(&UnDefined::TypeSensorCallBack, this);
	type_sensor_->attach(&shape_type_);
	type_sensor_->setPriority(0);
}

void UnDefined::ShapeTypeChange()
{
	int size = getNumChildren();
	removeChild(size - 1);

	vertex_prop_ = new SoVertexProperty();
	vertex_prop_->vertex.setValues(0, size_, &vertex_list_.front());
	vertex_prop_->orderedRGBA.setValue(color_.getPackedValue(transparency_));

	int shape_type = shape_type_.getValue();
	if (shape_type == static_cast<int>(ShapeType::LINE))
	{		
		line_set_ = new SoLineSet();
		line_set_->vertexProperty.setValue(vertex_prop_);
		face_set_ = nullptr;
		addChild(line_set_);
	}
	else if (shape_type == static_cast<int>(ShapeType::FACE))
	{
		face_set_ = new SoFaceSet();
		face_set_->vertexProperty.setValue(vertex_prop_);
		line_set_ = nullptr;
		addChild(face_set_);
	}
}

//bool UnDefined::EqualType(const ShapeType& type) const
//{
//	if (shape_type_.getValue() == static_cast<int>(type))
//	{
//		return true;
//	}
//
//	return false;
//}

void UnDefined::TypeSensorCallBack(void* data, SoSensor* sensor)
{
	UnDefined* myself = static_cast<UnDefined*>(data);
	myself->ShapeTypeChange();
}