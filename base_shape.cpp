#include "base_shape.h"

#include <Inventor/SoPath.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include "functions.h"

SO_NODE_ABSTRACT_SOURCE(BaseShape)
void BaseShape::InitClass()
{
	getClassRenderEngineMode().setRenderMode(SbRenderEngineMode::OIV_OPENINVENTOR_RENDERING);
	SO_NODE_INIT_ABSTRACT_CLASS(BaseShape, SoSeparator, "SoSeparator");
}

void BaseShape::ExitClass()
{
	SO__NODE_EXIT_CLASS(BaseShape);
}

BaseShape::BaseShape()
{
	SO_NODE_CONSTRUCTOR(BaseShape);
	setName(SbName("BaseShape"));
}

BaseShape::~BaseShape()
{
	SafeDeleteArray(vertices_);
}

void BaseShape::SetPos(const SbVec3f& pos)
{
	if (pos_ == pos)
	{
		return;
	}

	SoNode* transform = FindPropertyNode(SoTransform::getClassTypeId());
	if (IsNull(transform))
	{
		transform = new SoTransform();
		insertChild(transform, 0);
	}

	pos_ = pos;
	static_cast<SoTransform*>(transform)->translation.setValue(pos);
}

void BaseShape::SetPos(float pos[3])
{
	SetPos(SbVec3f(pos));
}

void BaseShape::SetPos(const float x, const float y, const float z)
{
	SetPos(SbVec3f(x, y, z));
}

void BaseShape::SetColor(const SbColor& color)
{
	SoNode* material = FindPropertyNode(SoMaterial::getClassTypeId());
	if (IsNull(material))
	{
		material = new SoMaterial();
		insertChild(material, 0);
	}
	static_cast<SoMaterial*>(material)->diffuseColor.setValue(color);
}

void BaseShape::SetColor(const float r, const float g, const float b)
{
	SetColor(SbColor(r, g, b));
}

void BaseShape::SetTransparency(const float transparency)
{
	if (CompareFloat(transparency, 0.f) == -1)
	{
		return;
	}

	SoNode* material = FindPropertyNode(SoMaterial::getClassTypeId());
	if (IsNull(material))
	{
		material = new SoMaterial();
		insertChild(material, 0);
	}
	static_cast<SoMaterial*>(material)->transparency.setValue(transparency);
}

void BaseShape::SetPickStyle(SoPickStyle::Style style)
{
	SoNode* pick_style = FindPropertyNode(SoPickStyle::getClassTypeId());
	if (IsNull(pick_style))
	{
		pick_style = new SoPickStyle();
		insertChild(pick_style, 0);
	}
	static_cast<SoPickStyle*>(pick_style)->style.setValue(style);
}

void BaseShape::SetDrawStyle(SoDrawStyle::Style style)
{
	SoNode* draw_style = FindPropertyNode(SoDrawStyle::getClassTypeId());
	if (IsNull(draw_style))
	{
		draw_style = new SoDrawStyle();
		insertChild(draw_style, 0);
	}
	static_cast<SoDrawStyle*>(draw_style)->style.setValue(style);
}

bool BaseShape::AddPropertyNode(SoNode* new_node, const SoType& type)
{
	if (CheckPropertyNode(type) ||
		new_node->isOfType(type) == SbBool(false) ||
		new_node->isOfType(SoGroup::getClassTypeId()) == SbBool(true))
	{
		new_node->ref();
		new_node->unref();
		return false;
	}

	insertChild(new_node, 0);
	return true;
}

bool BaseShape::RemovePropertyNode(const SoType& type)
{
	SoNode* node = FindPropertyNode(type);
	if (IsNull(node))
	{
		return false;
	}

	removeChild(node);
	return true;
}

SoNode* BaseShape::FindPropertyNode(const SoType& type)
{
	SoNode* node = nullptr;
	int size = getNumChildren();
	for (int i = 0; i < size; ++i)
	{
		node = getChild(i);
		if (node->isOfType(type))
		{
			return node;
		}
	}

	return nullptr;
}

BaseShape* BaseShape::FindShapeByFullPath(const SoFullPath* full_path)
{
	if (IsNull(full_path))
	{
		return nullptr;
	}

	SoNode* node = full_path->getTail();
	SoType soType = node->getClassTypeId();
	if (node->isOfType(SoShape::getClassTypeId()))
	{
		SoNode* shape = full_path->getNodeFromTail(1);
		if (shape->isOfType(BaseShape::getClassTypeId()))
		{
			return static_cast<BaseShape*>(shape);
		}
	}
	return nullptr;
}

void BaseShape::notify(SoNotList* list)
{
	SoSeparator::notify(list);
}

void BaseShape::addChild(SoNode* child)
{
	SoSeparator::addChild(child);
}

void BaseShape::insertChild(SoNode* child, int newChildIndex)
{
	SoSeparator::insertChild(child, newChildIndex);
}

void BaseShape::internalAddChild(SoNode* child)
{
	SoSeparator::internalAddChild(child);
}

void BaseShape::internalInsertChild(SoNode* child, int newChildIndex)
{
	SoSeparator::internalInsertChild(child, newChildIndex);
}

bool BaseShape::CheckPropertyNode(const SoType& type) const
{
	int size = getNumChildren();
	for (int i = 0; i < size; ++i)
	{
		if (getChild(i)->isOfType(type))
		{
			return true;
		}
	}

	return false;
}