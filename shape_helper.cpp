#pragma once
#include <Inventor/SbVec.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFaceSet.h>

static SoSeparator* MakeSplineCurveSep(SbVec3f* input_pt, int num, float detail = 0.05f, SoSeparator* target_sep = nullptr);
static SoSeparator* MakeLineSep(SbVec3f* input_pt, int num = 2, SoSeparator* target_sep = nullptr);
static int Create3DSplineCurve(SbVec3f* input_pt, SbVec3f* output, int num, float detail = 0.05f, bool inverse = false);
static int Create3DSplineCurve(SbVec3f* input_pt, std::vector<SbVec3f>* output_pt, int num, float detail = 0.05f, bool inverse = false);

SoSeparator* MakeSplineCurveSep(SbVec3f* input_pt, int num, float detail, SoSeparator* target_sep)
{
	//STL Vector 사용하여 크기 유동적으로 변경 및 setValues 시 Vector의 시작주소하고 크기를 넣어주도록 변경
	if (detail < 0.04f)
	{
		detail = 0.04f;
	}
	float px = 0, py = 0, pz = 0;
	int num_of_point = 0;
	for (float t = 0; t < 1; t += detail)
	{
		++num_of_point;
	}
	//std::cout << num_of_point << std::endl;
	int n = 0;
	float tt, _1_t, _2t, h00, h10, h01, h11;
	SbVec3f m0;
	SbVec3f m1;
	SbVec3f m2;
	SbVec3f m3;
	SoDrawStyle* curve_style = new SoDrawStyle();
	curve_style->style = SoDrawStyle::LINES;
	std::vector<SbVec3f> curve_vertex_code;
	curve_vertex_code.reserve(40);
	int index = 0;
	for (n = 0; n < num; n++)
	{
		for (float t = 0; t < 1; t += detail)
		{
			tt = t * t;
			_1_t = 1 - t;
			_2t = 2 * t;
			h00 = (1 + _2t) * (_1_t) * (_1_t);
			h10 = t * (_1_t) * (_1_t);
			h01 = tt * (3 - _2t);
			h11 = tt * (t - 1);

			if (!n)
			{
				//m0 = tangent(input_pt[n + 1], input_pt[n]);
				//m1 = tangent(input_pt[n + 2], input_pt[n]);
				m0 = SbVec3f((input_pt[n + 1].getValue()[0] - input_pt[n].getValue()[0]) / 2, (input_pt[n + 1].getValue()[1] - input_pt[n].getValue()[1]) / 2, (input_pt[n + 1].getValue()[2] - input_pt[n].getValue()[2]) / 2);
				m1 = SbVec3f((input_pt[n + 2].getValue()[0] - input_pt[n].getValue()[0]) / 2, (input_pt[n + 2].getValue()[1] - input_pt[n].getValue()[1]) / 2, (input_pt[n + 2].getValue()[2] - input_pt[n].getValue()[2]) / 2);
				px = h00 * input_pt[n].getValue()[0] + h10 * m0.getValue()[0] + h01 * input_pt[n + 1].getValue()[0] + h11 * m1.getValue()[0];
				py = h00 * input_pt[n].getValue()[1] + h10 * m0.getValue()[1] + h01 * input_pt[n + 1].getValue()[1] + h11 * m1.getValue()[1];
				pz = h00 * input_pt[n].getValue()[2] + h10 * m0.getValue()[2] + h01 * input_pt[n + 1].getValue()[2] + h11 * m1.getValue()[2];
				curve_vertex_code.push_back(SbVec3f(px, py, pz));
			}
			else if (n < num - 2)
			{
				//m1 = tangent(input_pt[n + 1], input_pt[n - 1]);
				//m2 = tangent(input_pt[n + 2], input_pt[n]);
				m0 = SbVec3f((input_pt[n + 1].getValue()[0] - input_pt[n - 1].getValue()[0]) / 2, (input_pt[n + 1].getValue()[1] - input_pt[n - 1].getValue()[1]) / 2, (input_pt[n + 1].getValue()[2] - input_pt[n - 1].getValue()[2]) / 2);
				m2 = SbVec3f((input_pt[n + 2].getValue()[0] - input_pt[n].getValue()[0]) / 2, (input_pt[n + 2].getValue()[1] - input_pt[n].getValue()[1]) / 2, (input_pt[n + 2].getValue()[2] - input_pt[n].getValue()[2]) / 2);
				px = h00 * input_pt[n].getValue()[0] + h10 * m1.getValue()[0] + h01 * input_pt[n + 1].getValue()[0] + h11 * m2.getValue()[0];
				py = h00 * input_pt[n].getValue()[1] + h10 * m1.getValue()[1] + h01 * input_pt[n + 1].getValue()[1] + h11 * m2.getValue()[1];
				pz = h00 * input_pt[n].getValue()[2] + h10 * m1.getValue()[2] + h01 * input_pt[n + 1].getValue()[2] + h11 * m2.getValue()[2];
				curve_vertex_code.push_back(SbVec3f(px, py, pz));
			}
			else if (n == num - 1)
			{
				//m2 = tangent(input_pt[n], input_pt[n - 2]);
				//m3 = tangent(input_pt[n], input_pt[n - 1]);
				m2 = SbVec3f((input_pt[n].getValue()[0] - input_pt[n - 2].getValue()[0]) / 2, (input_pt[n].getValue()[1] - input_pt[n - 2].getValue()[1]) / 2, (input_pt[n].getValue()[2] - input_pt[n - 2].getValue()[2]) / 2);
				m3 = SbVec3f((input_pt[n].getValue()[0] - input_pt[n - 1].getValue()[0]) / 2, (input_pt[n].getValue()[1] - input_pt[n - 1].getValue()[1]) / 2, (input_pt[n].getValue()[2] - input_pt[n - 1].getValue()[2]) / 2);
				px = h00 * input_pt[n - 1].getValue()[0] + h10 * m2.getValue()[0] + h01 * input_pt[n].getValue()[0] + h11 * m3.getValue()[0];
				py = h00 * input_pt[n - 1].getValue()[1] + h10 * m2.getValue()[1] + h01 * input_pt[n].getValue()[1] + h11 * m3.getValue()[1];
				pz = h00 * input_pt[n - 1].getValue()[2] + h10 * m2.getValue()[2] + h01 * input_pt[n].getValue()[2] + h11 * m3.getValue()[2];
				curve_vertex_code.push_back(SbVec3f(px, py, pz));
			}
		}
	}
	curve_vertex_code.push_back(input_pt[num - 1]);
	if (nullptr != target_sep)
	{
		for (int i = 0; i < target_sep->getNumChildren(); ++i)
		{
			if (target_sep->getChild(i)->isOfType(SoLineSet::getClassTypeId()))
			{
				SoLineSet* temp_set = static_cast<SoLineSet*>(target_sep->getChild(i));
				SoVertexProperty* temp_prop = static_cast<SoVertexProperty*>(temp_set->vertexProperty.getValue());
				temp_prop->orderedRGBA.setValue(SbColor(1.0f, 1.0f, 1.0f).getPackedValue());
				temp_prop->vertex.setValues(0, static_cast<int>(curve_vertex_code.size()), &curve_vertex_code.front());
				temp_set->numVertices.setValue(index);
				temp_set->vertexProperty.setValue(temp_prop);
			}
		}
	}
	else
	{
		SoVertexProperty* splane_vertex_prop = new SoVertexProperty();
		splane_vertex_prop->orderedRGBA.setValue(SbColor(1.0f, 1.0f, 1.0f).getPackedValue());
		splane_vertex_prop->vertex.setValues(0, static_cast<int>(curve_vertex_code.size()), &curve_vertex_code.front());
		SoLineSet* curve_set = new SoLineSet();
		curve_set->numVertices.setValue(static_cast<int>(curve_vertex_code.size()));
		curve_set->vertexProperty.setValue(splane_vertex_prop);
		SoSeparator* spline_sep = new SoSeparator();
		spline_sep->addChild(curve_set);
		return spline_sep;
	}
	return nullptr;
}

SoSeparator* MakeLineSep(SbVec3f* input_pt, int num, SoSeparator* target_sep)
{
	if (num > 16)
	{
		num = 16;
	}
	SbVec3f plane_vertex_coodinate_[16];
	for (int i = 0; i <= num; ++i)
	{
		plane_vertex_coodinate_[i] = input_pt[i%num];
	}
	if (nullptr == target_sep)
	{
		SoVertexProperty* plane_vertex_prop = new SoVertexProperty();
		plane_vertex_prop->orderedRGBA.setValue(SbColor(1.0f, 1.0f, 1.0f).getPackedValue());
		plane_vertex_prop->vertex.setValues(0, num + 1, plane_vertex_coodinate_);
		SoLineSet* plane_face_set = new SoLineSet();
		plane_face_set->numVertices.setValue(num + 1);
		plane_face_set->vertexProperty.setValue(plane_vertex_prop);
		SoSeparator* square_sep = new SoSeparator();
		square_sep->addChild(plane_face_set);
		return square_sep;
	}
	else
	{
		for (int i = 0; i < target_sep->getNumChildren(); ++i)
		{
			if (target_sep->getChild(i)->isOfType(SoLineSet::getClassTypeId()))
			{
				SoLineSet* temp_set = static_cast<SoLineSet*>(target_sep->getChild(i));
				SoVertexProperty* temp_prop = static_cast<SoVertexProperty*>(temp_set->vertexProperty.getValue());
				temp_prop->orderedRGBA.setValue(SbColor(1.0f, 1.0f, 1.0f).getPackedValue());
				temp_prop->vertex.setValues(0, num + 1, plane_vertex_coodinate_);
				temp_set->vertexProperty.setValue(temp_prop);
			}
		}
		return target_sep;
	}
}

int Create3DSplineCurve(SbVec3f* input_pt, SbVec3f* output, int num, float detail, bool inverse)
{
	if (detail < 0.04f)
	{
		detail = 0.04f;
	}
	float px = 0, py = 0, pz = 0;
	int num_of_point = 0;
	for (float t = 0; t < 1; t += detail)
	{
		++num_of_point;
	}
	int n = 0;
	float tt, _1_t, _2t, h00, h10, h01, h11;
	SbVec3f m0;
	SbVec3f m1;
	SbVec3f m2;
	SbVec3f m3;
	int index = 0;
	for (n = 0; n < num; n++)
	{
		for (float t = 0; t < 1; t += detail)
		{
			tt = t * t;
			_1_t = 1 - t;
			_2t = 2 * t;
			h00 = (1 + _2t) * (_1_t) * (_1_t);
			h10 = t * (_1_t) * (_1_t);
			h01 = tt * (3 - _2t);
			h11 = tt * (t - 1);

			if (!n)
			{
				//m0 = tangent(input_pt[n + 1], input_pt[n]);
				//m1 = tangent(input_pt[n + 2], input_pt[n]);
				m0 = SbVec3f((input_pt[n + 1].getValue()[0] - input_pt[n].getValue()[0]) / 2, (input_pt[n + 1].getValue()[1] - input_pt[n].getValue()[1]) / 2, (input_pt[n + 1].getValue()[2] - input_pt[n].getValue()[2]) / 2);
				m1 = SbVec3f((input_pt[n + 2].getValue()[0] - input_pt[n].getValue()[0]) / 2, (input_pt[n + 2].getValue()[1] - input_pt[n].getValue()[1]) / 2, (input_pt[n + 2].getValue()[2] - input_pt[n].getValue()[2]) / 2);
				px = h00 * input_pt[n].getValue()[0] + h10 * m0.getValue()[0] + h01 * input_pt[n + 1].getValue()[0] + h11 * m1.getValue()[0];
				py = h00 * input_pt[n].getValue()[1] + h10 * m0.getValue()[1] + h01 * input_pt[n + 1].getValue()[1] + h11 * m1.getValue()[1];
				pz = h00 * input_pt[n].getValue()[2] + h10 * m0.getValue()[2] + h01 * input_pt[n + 1].getValue()[2] + h11 * m1.getValue()[2];
				output[index] = SbVec3f(px, py, pz);
				++index;
			}
			else if (n < num - 2)
			{
				//m1 = tangent(input_pt[n + 1], input_pt[n - 1]);
				//m2 = tangent(input_pt[n + 2], input_pt[n]);
				m0 = SbVec3f((input_pt[n + 1].getValue()[0] - input_pt[n - 1].getValue()[0]) / 2, (input_pt[n + 1].getValue()[1] - input_pt[n - 1].getValue()[1]) / 2, (input_pt[n + 1].getValue()[2] - input_pt[n - 1].getValue()[2]) / 2);
				m2 = SbVec3f((input_pt[n + 2].getValue()[0] - input_pt[n].getValue()[0]) / 2, (input_pt[n + 2].getValue()[1] - input_pt[n].getValue()[1]) / 2, (input_pt[n + 2].getValue()[2] - input_pt[n].getValue()[2]) / 2);
				px = h00 * input_pt[n].getValue()[0] + h10 * m1.getValue()[0] + h01 * input_pt[n + 1].getValue()[0] + h11 * m2.getValue()[0];
				py = h00 * input_pt[n].getValue()[1] + h10 * m1.getValue()[1] + h01 * input_pt[n + 1].getValue()[1] + h11 * m2.getValue()[1];
				pz = h00 * input_pt[n].getValue()[2] + h10 * m1.getValue()[2] + h01 * input_pt[n + 1].getValue()[2] + h11 * m2.getValue()[2];
				output[index] = SbVec3f(px, py, pz);
				++index;
			}
			else if (n == num - 1)
			{
				//m2 = tangent(input_pt[n], input_pt[n - 2]);
				//m3 = tangent(input_pt[n], input_pt[n - 1]);
				m2 = SbVec3f((input_pt[n].getValue()[0] - input_pt[n - 2].getValue()[0]) / 2, (input_pt[n].getValue()[1] - input_pt[n - 2].getValue()[1]) / 2, (input_pt[n].getValue()[2] - input_pt[n - 2].getValue()[2]) / 2);
				m3 = SbVec3f((input_pt[n].getValue()[0] - input_pt[n - 1].getValue()[0]) / 2, (input_pt[n].getValue()[1] - input_pt[n - 1].getValue()[1]) / 2, (input_pt[n].getValue()[2] - input_pt[n - 1].getValue()[2]) / 2);
				px = h00 * input_pt[n - 1].getValue()[0] + h10 * m2.getValue()[0] + h01 * input_pt[n].getValue()[0] + h11 * m3.getValue()[0];
				py = h00 * input_pt[n - 1].getValue()[1] + h10 * m2.getValue()[1] + h01 * input_pt[n].getValue()[1] + h11 * m3.getValue()[1];
				pz = h00 * input_pt[n - 1].getValue()[2] + h10 * m2.getValue()[2] + h01 * input_pt[n].getValue()[2] + h11 * m3.getValue()[2];
				output[index] = SbVec3f(px, py, pz);
				++index;
			}
		}
	}
	output[index] = input_pt[num - 1];
	index++;
	return index;
}

int Create3DSplineCurve(SbVec3f * input_pt, std::vector<SbVec3f>* output_pt, int num, float detail, bool inverse)
{
	if (detail < 0.04f)
	{
		detail = 0.04f;
	}
	float px = 0, py = 0, pz = 0;
	int n = 0;
	float tt, _1_t, _2t, h00, h10, h01, h11;
	SbVec3f m0;
	SbVec3f m1;
	SbVec3f m2;
	SbVec3f m3;
	int index = 0;
	for (n = 0; n < num; n++)
	{
		for (float t = 0; t < 1; t += detail)
		{
			tt = t * t;
			_1_t = 1 - t;
			_2t = 2 * t;
			h00 = (1 + _2t) * (_1_t) * (_1_t);
			h10 = t * (_1_t) * (_1_t);
			h01 = tt * (3 - _2t);
			h11 = tt * (t - 1);

			if (!n)
			{
				//m0 = tangent(input_pt[n + 1], input_pt[n]);
				//m1 = tangent(input_pt[n + 2], input_pt[n]);
				m0 = SbVec3f((input_pt[n + 1].getValue()[0] - input_pt[n].getValue()[0]) / 2, (input_pt[n + 1].getValue()[1] - input_pt[n].getValue()[1]) / 2, (input_pt[n + 1].getValue()[2] - input_pt[n].getValue()[2]) / 2);
				m1 = SbVec3f((input_pt[n + 2].getValue()[0] - input_pt[n].getValue()[0]) / 2, (input_pt[n + 2].getValue()[1] - input_pt[n].getValue()[1]) / 2, (input_pt[n + 2].getValue()[2] - input_pt[n].getValue()[2]) / 2);
				px = h00 * input_pt[n].getValue()[0] + h10 * m0.getValue()[0] + h01 * input_pt[n + 1].getValue()[0] + h11 * m1.getValue()[0];
				py = h00 * input_pt[n].getValue()[1] + h10 * m0.getValue()[1] + h01 * input_pt[n + 1].getValue()[1] + h11 * m1.getValue()[1];
				pz = h00 * input_pt[n].getValue()[2] + h10 * m0.getValue()[2] + h01 * input_pt[n + 1].getValue()[2] + h11 * m1.getValue()[2];
				output_pt->push_back(SbVec3f(px, py, pz));
			}
			else if (n < num - 2)
			{
				//m1 = tangent(input_pt[n + 1], input_pt[n - 1]);
				//m2 = tangent(input_pt[n + 2], input_pt[n]);
				m0 = SbVec3f((input_pt[n + 1].getValue()[0] - input_pt[n - 1].getValue()[0]) / 2, (input_pt[n + 1].getValue()[1] - input_pt[n - 1].getValue()[1]) / 2, (input_pt[n + 1].getValue()[2] - input_pt[n - 1].getValue()[2]) / 2);
				m2 = SbVec3f((input_pt[n + 2].getValue()[0] - input_pt[n].getValue()[0]) / 2, (input_pt[n + 2].getValue()[1] - input_pt[n].getValue()[1]) / 2, (input_pt[n + 2].getValue()[2] - input_pt[n].getValue()[2]) / 2);
				px = h00 * input_pt[n].getValue()[0] + h10 * m1.getValue()[0] + h01 * input_pt[n + 1].getValue()[0] + h11 * m2.getValue()[0];
				py = h00 * input_pt[n].getValue()[1] + h10 * m1.getValue()[1] + h01 * input_pt[n + 1].getValue()[1] + h11 * m2.getValue()[1];
				pz = h00 * input_pt[n].getValue()[2] + h10 * m1.getValue()[2] + h01 * input_pt[n + 1].getValue()[2] + h11 * m2.getValue()[2];
				output_pt->push_back(SbVec3f(px, py, pz));
			}
			else if (n == num - 1)
			{
				//m2 = tangent(input_pt[n], input_pt[n - 2]);
				//m3 = tangent(input_pt[n], input_pt[n - 1]);
				m2 = SbVec3f((input_pt[n].getValue()[0] - input_pt[n - 2].getValue()[0]) / 2, (input_pt[n].getValue()[1] - input_pt[n - 2].getValue()[1]) / 2, (input_pt[n].getValue()[2] - input_pt[n - 2].getValue()[2]) / 2);
				m3 = SbVec3f((input_pt[n].getValue()[0] - input_pt[n - 1].getValue()[0]) / 2, (input_pt[n].getValue()[1] - input_pt[n - 1].getValue()[1]) / 2, (input_pt[n].getValue()[2] - input_pt[n - 1].getValue()[2]) / 2);
				px = h00 * input_pt[n - 1].getValue()[0] + h10 * m2.getValue()[0] + h01 * input_pt[n].getValue()[0] + h11 * m3.getValue()[0];
				py = h00 * input_pt[n - 1].getValue()[1] + h10 * m2.getValue()[1] + h01 * input_pt[n].getValue()[1] + h11 * m3.getValue()[1];
				pz = h00 * input_pt[n - 1].getValue()[2] + h10 * m2.getValue()[2] + h01 * input_pt[n].getValue()[2] + h11 * m3.getValue()[2];
				output_pt->push_back(SbVec3f(px, py, pz));
			}
		}
	}
	output_pt->push_back(input_pt[num - 1]);
	return static_cast<int>(output_pt->size());
}