#pragma once

#include "core/CurveDiscretizer.h"
#include "geometry/linalg.h"
#include "node.h"
#include "Value.h"
#ifdef ENABLE_PYTHON
#include <src/python/python_public.h>
#endif

class HelicoidalExtrudeNode : public AbstractPolyNode
{
public:
	VISITABLE();
	HelicoidalExtrudeNode(std::shared_ptr<const ModuleInstantiation>mi, CurveDiscretizer discretizer) : AbstractPolyNode(std::move(mi)), discretizer(std::move(discretizer)) {
		convexity = 0;
		fn = fs = fa = 0;
		origin_x = origin_y = scale = 0;
        angle = 360;
		step = 0;
        delta = 0;
        scale = 1.0;
        xScalEnd =yScalEnd = zScalEnd = 1;
		nbRotation = 1;
        xOffset = 0.0;
        axeRotate = 0;
        zRotate = 0.0;
        xRotate = 0.0;
        xOffsetEnd = 0.0;
        // AGZ 16/12/2024: Extension scaling
        xScalBegin = yScalBegin = 1;
	}
	std::string toString() const override;
	std::string name() const override { return "helicoidal_extrude"; }

	int convexity;
	double fn, fs, fa;
	double origin_x, origin_y, scale, angle;
    double step;
    double delta;
    double xScalEnd, yScalEnd, zScalEnd;
    double nbRotation;
	std::string layername;
	
	double xOffset;
	double axeRotate;
	double zRotate;
	double xRotate;
	double xOffsetEnd;

    // AGZ 16/12/2024: Extension scaling
    double xScalBegin, yScalBegin;
    
    std::vector<Vector3d> scaleOpe;	// Angle, xScalEnd, yScalEnd
#ifdef ENABLE_PYTHON
  void *profile_func;
  void *twist_func;
  std::string profile_func_hash;
  std::string twist_func_hash;
#endif
  CurveDiscretizer discretizer;
};
