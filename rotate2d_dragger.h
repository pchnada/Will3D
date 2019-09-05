#pragma once
#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/projectors/SbPlaneProjector.h>
//class SbPlaneProjector;
class SoVertexProperty;
class SoDrawStyle;
class SoLineSet;
class SoMaterial;
class SoSFRotation;

class Rotate2DDragger : public SoDragger
{
	SO_KIT_HEADER(Rotate2DDragger);
	SO_KIT_CATALOG_ENTRY_HEADER(dragger_separator);
public:	//Function
	Rotate2DDragger();
	static void InitClass(void);
	static void ExitClass(void);
	virtual void notify(SoNotList* not_list) override;
public:	//Attribute
	SoSFRotation rotation_;
protected:	//Function
	virtual ~Rotate2DDragger();
	static void StartCB(void* user_data, SoDragger* dragger);
	static void MotionCB(void* user_data, SoDragger* dragger);
	static void FinishCB(void* user_data, SoDragger* dragger);
protected:	//Attribute
	SbPlaneProjector* dragger_prj_ = nullptr;

private:	//Function
	void SetLineRotation(SbRotation line_rotation);
	void SetLineTranslate(SbVec3f line_translate);
	void SetLineCenter(SbVec2f center2D);
	void Start(void* user_data, SoDragger* dragger);
	void Drag(void* user_data, SoDragger* dragger);
	void Finish(void* user_data, SoDragger* dragger);
private:	//Attribute
};
