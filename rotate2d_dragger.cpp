#include "shape_helper.h"
#include "rotate2d_dragger.h"

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>
//#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/nodes/SoVertexProperty.h>

SO_KIT_SOURCE(Rotate2DDragger);

Rotate2DDragger::Rotate2DDragger()
{
	SO_KIT_CONSTRUCTOR(Rotate2DDragger);
	SO_KIT_ADD_CATALOG_ENTRY(dragger_separator, SoSeparator, true, geomSeparator, \x0, true);
	SO_KIT_ADD_FIELD(rotation_, (SbRotation::identity()));
	SO_KIT_INIT_INSTANCE();
	dragger_prj_ = new SbPlaneProjector();
	addStartCallback(Rotate2DDragger::StartCB, this);
	addMotionCallback(Rotate2DDragger::MotionCB, this);
	addFinishCallback(Rotate2DDragger::FinishCB, this);
	setPart("dragger_separator", ShapeHelper::DrawLineSep(SbVec2f(0.0f, 0.0f), 1.0f));
}

void Rotate2DDragger::InitClass(void)
{
	getClassRenderEngineMode().setRenderMode(SbRenderEngineMode::OIV_OPENINVENTOR_RENDERING);
	SO_KIT_INIT_CLASS(Rotate2DDragger, SoDragger, "SoDragger");
}

void Rotate2DDragger::ExitClass(void)
{
	SO__KIT_EXIT_CLASS(Rotate2DDragger);
}

void Rotate2DDragger::Start(void* user_data, SoDragger* dragger)
{
	Rotate2DDragger* myself = static_cast<Rotate2DDragger*>(user_data);
}

void Rotate2DDragger::Drag(void* user_data, SoDragger* dragger)
{
	Rotate2DDragger* myself = static_cast<Rotate2DDragger*>(user_data);
	myself->dragger_prj_->setViewVolume(myself->viewVolume);
	myself->dragger_prj_->setWorkingSpace(myself->getLocalToWorldMatrix());
	SbVec2f start_pt = myself->vpRegion.normalize(static_cast<SbVec2s>(myself->getStartLocaterPosition()));
	SbVec2f current_pt = myself->getNormalizedLocaterPosition();
	SbRotation rot = SbRotation(myself->dragger_prj_->project(start_pt), myself->dragger_prj_->project(current_pt));
	SbMatrix temp_prev_rot_mat = SbMatrix::identity();
	SbMatrix temp_current_rot_mat = SbMatrix::identity();
	SbMatrix temp_delta_rot_mat = SbMatrix::identity();
	SbVec3f temp_trans;
	SbRotation temp_rot;
	SbVec3f temp_scale;
	SbRotation temp_scale_orient;
	myself->getMotionMatrix().getTransform(temp_trans, temp_rot, temp_scale, temp_scale_orient);
	temp_rot.getValue(temp_prev_rot_mat);
	rot.getValue(temp_current_rot_mat);
	temp_delta_rot_mat = temp_prev_rot_mat.inverse().multLeft(temp_current_rot_mat);
	myself->setMotionMatrix(myself->appendRotation(myself->getStartMotionMatrix(), rot, SbVec3f(0.0f, 0.0f, 0.0f)));
	myself->rotation_.setValue(rot);
}

void Rotate2DDragger::Finish(void* user_data, SoDragger* dragger)
{
	Rotate2DDragger* myself = static_cast<Rotate2DDragger*>(user_data);
}

void Rotate2DDragger::notify(SoNotList* not_list)
{
	SoDragger::notify(not_list);
}

void Rotate2DDragger::SetLineRotation(SbRotation line_rotation)
{
}

void Rotate2DDragger::SetLineTranslate(SbVec3f line_translate)
{
}

void Rotate2DDragger::SetLineCenter(SbVec2f center2D)
{
	SbMatrix translate_matrix = SbMatrix::identity();
	translate_matrix.setTranslate(SbVec3f(center2D.getValue()[0], center2D.getValue()[1], 0.0f));
	translate_matrix = translate_matrix.multLeft(getMotionMatrix());
	setMotionMatrix(translate_matrix);
}

Rotate2DDragger::~Rotate2DDragger()
{
}

void Rotate2DDragger::StartCB(void* user_data, SoDragger* dragger)
{
	Rotate2DDragger* myself = static_cast<Rotate2DDragger*>(user_data);
	myself->Start(user_data, dragger);
}

void Rotate2DDragger::MotionCB(void* user_data, SoDragger* dragger)
{
	Rotate2DDragger* myself = static_cast<Rotate2DDragger*>(user_data);
	myself->Drag(user_data, dragger);
}

void Rotate2DDragger::FinishCB(void* user_data, SoDragger* dragger)
{
	Rotate2DDragger* myself = static_cast<Rotate2DDragger*>(user_data);
	myself->Finish(user_data, dragger);
}