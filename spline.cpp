#include "spline.h"

#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <ImageViz/Engines/ImageSegmentation/ComputationalGeometry/SoPolylineResamplerApproximation3d.h>

#include "functions.h"

SO_NODE_SOURCE(Spline);
void Spline::InitClass()
{
	if (BaseShape::getClassTypeId().isBad())
	{
		BaseShape::InitClass();
	}

	getClassRenderEngineMode().setRenderMode(SbRenderEngineMode::OIV_OPENINVENTOR_RENDERING);
	SO_NODE_INIT_CLASS(Spline, BaseShape, "BaseShape");
}

void Spline::ExitClass()
{
	SO__NODE_EXIT_CLASS(Spline);
}

Spline::Spline()
{
	Initialize();
}

Spline::Spline(const double spacing)
{
	Initialize();
	spacing_.setValue(spacing);
}

Spline::~Spline()
{

}

void Spline::AddInPoint(const SbVec3f& point)
{
	int num = resampler_->inPolyline.getNum();
	if (in_points_size_ >= num)
	{
		resampler_->inPolyline.insertSpace(in_points_size_, 1);
	}

	resampler_->inPolyline.set1Value(in_points_size_, point);
	++in_points_size_;

	SetSpline();
}

bool Spline::InsertInPoint(const int index, const SbVec3f& point)
{
	if (index > in_points_size_ || index < 0)
	{
		return false;
	}

	int num = resampler_->inPolyline.getNum();
	if (in_points_size_ == num)
	{
		resampler_->inPolyline.insertSpace(index, 1);
	}

	resampler_->inPolyline.set1Value(index, point);
	++in_points_size_;

	SetSpline();
	return true;
}

bool Spline::ChangeInPoint(const int index, const SbVec3f& point)
{
	if (index >= in_points_size_ || index < 0)
	{
		return false;
	}

	resampler_->inPolyline.set1Value(index, point);

	SetSpline();
	return true;
}

bool Spline::SetInPoints(const int start, const int num, SbVec3f* points/* = nullptr*/)
{
	if (points == nullptr || start < 0 || num < 2)
	{
		return false;
	}

	if (in_points_size_ > 0)
	{
		RemoveAllInPoints();
	}

	for (int i = 0; i < num; ++i)
	{
		AddInPoint(points[start + i]);
	}

	return true;
}

void Spline::RemoveInPoint(const int index)
{
	if (index >= in_points_size_)
	{
		return;
	}

	resampler_->inPolyline.deleteValues(index, 1);
	--in_points_size_;

	SetSpline();
}

void Spline::RemoveAllInPoints()
{
	if (in_points_size_ <= 0)
	{
		return;
	}

	resampler_->inPolyline.deleteValues(0);

	SoLineSet* line_set = FindShape();
	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(line_set->vertexProperty.getValue());
	vertex_property->vertex.deleteValues(0);

	in_points_size_ = 0;
}

const int Spline::out_points_size() const
{
	return static_cast<int>(resampler_->outPolyline.getSize());
}

const SbVec3f* Spline::out_points() const
{
	return resampler_->outPolyline.getValue();
}

void Spline::SetColor(const SbColor& color)
{
	color_ = color;
	SoLineSet* line_set = FindShape();
	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(line_set->vertexProperty.getValue());
	vertex_property->orderedRGBA.setValue(color_.getPackedValue(transparency_));
}

void Spline::SetTransparency(float transparency)
{
	if (EqualFloat(transparency_, transparency))
	{
		return;
	}

	transparency_ = transparency;
	SoLineSet* line_set = FindShape();
	SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(line_set->vertexProperty.getValue());
	vertex_property->orderedRGBA.setValue(color_.getPackedValue(transparency_));
}

void Spline::SetLineWidth(const float width)
{
	SoNode* draw_style = FindPropertyNode(SoDrawStyle::getClassTypeId());
	if (IsNull(draw_style))
	{
		draw_style = new SoDrawStyle();
		insertChild(draw_style, 0);
	}

	if (CompareFloat(width, 0) != -1)
	{
		static_cast<SoDrawStyle*>(draw_style)->lineWidth.setValue(width);
	}
}

float Spline::LineWidth()
{
	SoNode* draw_style = FindPropertyNode(SoDrawStyle::getClassTypeId());
	if (IsNull(draw_style))
	{
		draw_style = new SoDrawStyle();
		insertChild(draw_style, 0);
	}

	return static_cast<SoDrawStyle*>(draw_style)->lineWidth.getValue();
}

void Spline::notify(SoNotList* list)
{
	SoSeparator::notify(list);
	SoField* field = list->getLastField();
	if (field)
	{
		if (field->isOfType(SoSFDouble::getClassTypeId()))
		{
			if (&spacing_ == field)
			{
				FieldChange();
			}
		}
	}
}

void Spline::InitBaseProperties()
{
	setName(SbName("Spline"));

	SoLineSet* line_set = new SoLineSet();
	SoVertexProperty* vertex_propoerty = new SoVertexProperty();
	line_set->vertexProperty.setValue(vertex_propoerty);

	addChild(line_set);
}

void Spline::FieldChange()
{
	double sp = fabs(spacing_.getValue());
	if (IsNull(resampler_.ptr()) || sp == 0.0)
	{
		return;
	}

	resampler_->spacing.setValue(sp);
	SetSpline();
}

void Spline::Initialize()
{
	SO_NODE_CONSTRUCTOR(Spline);
	SO_NODE_ADD_FIELD(spacing_, (0.05));

	resampler_ = new SoPolylineResamplerApproximation3d();
	resampler_->interpolationMode = SoPolylineResamplerApproximation3d::InterpolationMode::CATMULLROM;
	resampler_->spacing = spacing_;

	InitBaseProperties();
}

void Spline::SetSpline()
{
	if (in_points_size_ > 1)
	{
		size_t size = resampler_->outPolyline.getSize();
		const SbVec3f* points = resampler_->outPolyline.getValue();

		SoLineSet* line_set = FindShape();
		SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(line_set->vertexProperty.getValue());
		vertex_property->vertex.deleteValues(0);
		vertex_property->vertex.setValuesPointer(static_cast<int>(size), points);
	}
	else if (in_points_size_ <= 1)
	{
		SoLineSet* line_set = FindShape();
		SoVertexProperty* vertex_property = static_cast<SoVertexProperty*>(line_set->vertexProperty.getValue());

		int num = vertex_property->vertex.getNum();
		if (vertex_property->vertex.getNum() > 1)
		{
			vertex_property->vertex.deleteValues(0);
		}
	}
}

SoLineSet* Spline::FindShape()
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

//bool Spline::EqualSpacing(const double spacing) const
//{
//	if (spacing_.getValue() == spacing)
//	{
//		return true;
//	}
//
//	return false;
//}