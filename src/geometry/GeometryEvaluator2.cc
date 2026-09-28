#include "geometry/GeometryEvaluator.h"
#include "geometry/GeometryCache.h"
#include "geometry/Polygon2d.h"
#include "geometry/PolySetUtils.h"
#include "geometry/PolySet.h"
#include "geometry/PolySetBuilder.h"
#include "geometry/roof_ss.h"
#include "geometry/roof_vd.h"
#include "geometry/ClipperUtils.h"

#include "core/Tree.h"
#include "core/module.h"
#include "core/ModuleInstantiation.h"
#include "core/RoofNode.h"
#include "core/State.h"
#include "core/CgalAdvNode.h"
#include "core/ProjectionNode.h"
#include "core/CsgOpNode.h"
#include "core/TextNode.h"
#include "core/HelicoidalExtrudeNode.h"
#include "io/DxfData.h"
#include <ciso646> // C alternative tokens (xor)
#include <algorithm>

#include "utils/boost-utils.h"
#include "utils/calc.h"
#include "utils/printutils.h"
#include "utils/degree_trig.h"
#include "utils/svg.h"
#ifdef ENABLE_CGAL
#include "geometry/cgal/CGALCache.h"
#include "geometry/cgal/cgalutils.h"
#include <CGAL/convex_hull_2.h>
#include <CGAL/Point_2.h>
#endif


// A.G. Scaling object added
static void AGZ_fillRing(std::vector<Vector3d> &ring, const Outline2d &o, double a, bool flip, double step, double xStep=0, double xScale = 1, double yScale = 1, double zScale = 1, double xOffset = 0, double axeRotate=0, double zRotate = 0, double xRotate = 90)
{
  if (flip) {
    unsigned int l = o.vertices.size() - 1;
    for (unsigned int i = 0; i < o.vertices.size(); ++i) {
	double x = o.vertices[l-i][0];
	double y = o.vertices[l-i][1];
	double Y = y;
	if(zRotate != 0) {
		Y = x * sin_degrees(zRotate) + y * cos_degrees(zRotate);
		x = x * cos_degrees(zRotate) - y * sin_degrees(zRotate);
	}
	double X = x  * xScale;
	double X2 = X ;
	double Y2 = Y * yScale * cos_degrees(xRotate);
	double Z2 = Y * yScale * sin_degrees(xRotate);
	            
	ring[i][0] = (xOffset+xStep* xScale) * sin_degrees(a) + X2 * sin_degrees(a+axeRotate) - Y2 * cos_degrees(a+axeRotate);
				ring[i][1] = (xOffset+xStep* xScale) * cos_degrees(a) + X2 * cos_degrees(a+axeRotate) + Y2 * sin_degrees(a+axeRotate);
	ring[i][2] = Z2 + step * zScale  ;
    }
  } else {
    for (unsigned int i = 0; i < o.vertices.size(); ++i) {
	double x = o.vertices[i][0];
	double y = o.vertices[i][1];
	double Y = y;
	if(zRotate != 0) {
		Y = x * sin_degrees(zRotate) + y * cos_degrees(zRotate);
		x = x * cos_degrees(zRotate) - y * sin_degrees(zRotate);
	}
	double X = x  * xScale;

	double X2 = X ;
	double Y2 = Y * yScale * cos_degrees(xRotate);
	double Z2 = Y * yScale * sin_degrees(xRotate);
	            
	ring[i][0] = (xOffset+xStep*xScale)*sin_degrees(a) + X2*sin_degrees(a+axeRotate) - Y2*cos_degrees(a+axeRotate);
				ring[i][1] = (xOffset+xStep*xScale)*cos_degrees(a) + X2*cos_degrees(a+axeRotate) + Y2*sin_degrees(a+axeRotate);
	ring[i][2] = Z2 + step * zScale  ;
    }
  }
}

// Specific closure when rotating
static int AGZ_selectIndexMobius(std::vector<Vector3d> &list) {
	int index = 0;
	double d = 0;
	for(int i=list.size()-1; i>=0;i--) {
		Vector3d u = list[i];
		double l = u[0]*u[0] + u[1]*u[1] + u[2] * u[2];
		if(l > d)
		{
			index = i;
			d = l;
		}
	}
	return index;
}

// A.G. Helicoidal Extrude from Rotate Extrude !
/*!
    Input to extrude should be clean. This means non-intersecting, correct winding order
    etc., the input coming from a library like Clipper.

    FIXME: We should handle some common corner cases better:
    o 2D polygon having an edge being on the Y axis:
        In this case, we don't need to generate geometry involving this edge as it
        will be an internal edge.
    o 2D polygon having a vertex touching the Y axis:
        This is more complex as the resulting geometry will (may?) be nonmanifold.
        In any case, the previous case is a specialization of this, so the following
        should be handled for both cases:
        Since the ring associated with this vertex will have a radius of zero, it will
        collapse to one vertex. Any quad using this ring will be collapsed to a triangle.

    Currently, we generate a lot of zero-area triangles

*/
static std::unique_ptr<Geometry> helicoidalPolygon(const HelicoidalExtrudeNode &node, const Polygon2d &poly)
{
	bool bMobius = false;
    if (node.angle == 0) return nullptr;

    PolySetBuilder builder;
    builder.setConvexity(node.convexity);
    
    bool b360 = (node.angle == 360);
    if(node.delta > 0) b360 = false;
    if(node.step != 0) b360 = false;

//    PolySet *ps = new PolySet(3);
//    ps->setConvexity(node.convexity);

    double min_x = 0;
    double max_x = 0;
    double min_y = 0;
    double max_y = 0;
    unsigned int fragCircle = 0;
    unsigned int fragments = 0;
    for(const auto &o : poly.outlines()) {
        for(const auto &v : o.vertices) {
            min_x = fmin(min_x, v[0]);
            max_x = fmax(max_x, v[0]);
            min_y = fmin(min_y, v[1]);
            max_y = fmax(max_y, v[1]);

            if ((max_x - min_x) > (max_x+node.xOffset) && (max_x - min_x) > fabs(min_x+node.xOffset)) {
			    LOG(message_group::Error,Location::NONE,"", "all points for helicoidal_extrude() must have the same X coordinate sign (range is %1$.2f -> %2$.2f)", min_x , max_x);
//                delete ps;
                return nullptr;
            }
        }
    }
    if((node.axeRotate != 0.0)  && (min_x < 0)){
		Polygon2d poly2;
		LOG(message_group::Warning,Location::NONE,"", "Translate Polygone %1$.2f", -min_x);
 		for(auto &o : poly.outlines()) {
			Outline2d outline;
			for( auto &v : o.vertices) {
				outline.vertices.emplace_back(v[0] - min_x, v[1]);
			}
			poly2.addOutline(outline);
		}
		return helicoidalPolygon(node, poly2);
	}
		Transform2d mat2;
		mat2.matrix() <<
		  cos_degrees(node.zRotate), -sin_degrees(node.zRotate), 0,
		  sin_degrees(node.zRotate), cos_degrees(node.zRotate), 0,
		  0, 0, 1;
    //fragments = (unsigned int)fmax(Calc::get_fragments_from_r(max_x - min_x, node.fn, node.fs, node.fa) * std::abs(node.angle) / 360, 1);
    // Calcul provenant de rotate extrude non fonctionnel !
    fragCircle = Calc::get_fragments_from_r(fmax(std::abs(max_x+node.xOffset), std::abs(min_x+node.xOffset)) , node.fn, node.fs, node.fa);
    fragments = (unsigned int)fmax(fragCircle * std::abs(node.angle) / 360, 1);
    if(node.scaleOpe.size() >= 1) {
		
	}
    double delta_x = max_x - min_x;
    double delta_y = max_y - min_y;
    bool flip_faces = ((min_x+node.xOffset) >= 0 && node.angle > 0 && !b360) || ((min_x+node.xOffset) < 0 && (node.angle < 0 || b360));

    double step  = node.step;
    double xStep = node.delta;
    xStep  /= fragments;

 	//step fait référence à un tour complet
	double  ratio = (node.angle < 360) ? 360 / std::abs(node.angle) : 1.0;
	int  frag360 =  (int)fmax(fragCircle / ratio, 1);
    double pas0 = step / frag360;
    double pas = step * (std::abs(node.angle) / 360) / fragments;
    LOG(message_group::Warning,Location::NONE,"", "PAS %1$.5f / %2$.5f", pas0, pas);
    
    Transform3d rotS(angle_axis_degrees(-node.axeRotate, Vector3d::UnitY()));
    if(!b360) {
        auto ps_start = poly.tessellate(); // starting face
        ps_start->transform(rotS);
        PolySetUtils::translate(*ps_start, Vector3d(node.xOffset,0,0));
        
        Transform3d rot(angle_axis_degrees(node.xRotate, Vector3d::UnitX()));
        ps_start->transform(rot);
        // Flip vertex ordering
        if (!flip_faces) {
            for(auto &p : ps_start->indices) {
                std::reverse(p.begin(), p.end());
            }
        }
		builder.appendPolySet(*ps_start);

        auto ps_end = poly.tessellate();
       
        if(node.zRotate != 0) {
			std::unique_ptr<Geometry> geometry = poly.copy();
			Polygon2d& polygon =  dynamic_cast<Polygon2d &>(*geometry);
            polygon.transform(mat2);
            ps_end = polygon.tessellate();
		}
            
        Transform3d rotS2(angle_axis_degrees(-node.axeRotate, Vector3d::UnitY()));
               
        PolySetUtils::scale(*ps_end, Vector3d(node.xScalEnd, node.yScalEnd, 1));
        ps_end->transform(rotS2);
        PolySetUtils::translate(*ps_end, Vector3d(xStep*fragments*node.xScalEnd, 0, 0));
        Transform3d rotX(angle_axis_degrees(node.xRotate, Vector3d::UnitX()));
        ps_end->transform(rotX);
        PolySetUtils::translate(*ps_end, Vector3d(0, 0, pas*fragments*node.zScalEnd));
       
        PolySetUtils::translate(*ps_end, Vector3d(node.xOffsetEnd,0,0));
        Transform3d rot2(angle_axis_degrees(node.angle, Vector3d::UnitZ()) );
        ps_end->transform(rot2);
        if (flip_faces) {
            for(auto &p : ps_end->indices) {
                std::reverse(p.begin(), p.end());
            }
        }
		builder.appendPolySet(*ps_end);
    }

	if(b360 && (node.zRotate != 0))
		bMobius = true;
		
    for(const auto &o : poly.outlines()) {
        std::vector<Vector3d> rings[2];
        rings[0].resize(o.vertices.size());
        rings[1].resize(o.vertices.size());

        AGZ_fillRing(rings[0], o, (b360) ? -90 : 90, flip_faces, 0, 0, 1,1,1, node.xOffset, node.axeRotate, 0, node.xRotate); // first ring
        int nbFrag = bMobius ? fragments-1 : fragments;
        double xOffsetStep = (node.xOffsetEnd - node.xOffset) / nbFrag;
        for (unsigned int j = 0; j < nbFrag; j++) {
            double a;
            if (b360)
                a = -90 + ((j+1)%fragments) * 360.0 / fragments; // start on the -X axis, for legacy support
            else
                a = 90 - (j+1)* node.angle / fragments; // start on the X axis
                
            AGZ_fillRing(rings[(j+1)%2], o, a, flip_faces, (j+1)*pas, (j+1)*xStep, 1+(j+1)*(node.xScalEnd-1)/nbFrag, 1+(j+1)*(node.yScalEnd-1)/nbFrag, 1+(j+1)*(node.zScalEnd-1)/nbFrag, node.xOffset+(j+1)*xOffsetStep, node.axeRotate, (j+1)*node.zRotate/fragments, node.xRotate);
            for (size_t i=0;i<o.vertices.size();i++) {
				builder.appendPolygon({
                	rings[j%2][(i+1)%o.vertices.size()],
                	rings[(j+1)%2][(i+1)%o.vertices.size()],
                	rings[j%2][i]
                });
				builder.appendPolygon({
                	rings[(j+1)%2][(i+1)%o.vertices.size()],
                	rings[(j+1)%2][i],
                	rings[j%2][i],
                });
            }
        }
		if(bMobius) {
			// Rejoindre les extremités
			if(nbFrag%2 == 1)
				rings[0] = rings[1];
			AGZ_fillRing(rings[1], o, -90 , flip_faces, 0, 0, 1,1,1, node.xOffset, node.axeRotate, 0, node.xRotate); // first ring
			int idx0 = AGZ_selectIndexMobius(rings[0]);
			int idx1 = AGZ_selectIndexMobius(rings[1]);
			for (size_t i=0;i<o.vertices.size();i++) {
				builder.appendPolygon({
					rings[0][(i+1+idx0)%o.vertices.size()],
					rings[1][(i+1+idx1)%o.vertices.size()],
					rings[0][(i+idx0)%o.vertices.size()]
				});
				builder.appendPolygon({
					rings[1][(i+1+idx1)%o.vertices.size()],
					rings[1][(i+idx1)%o.vertices.size()],
					rings[0][(i+idx0)%o.vertices.size()]
				});
			}
		}

    }
    return builder.build();
}

/*!
    input: List of 2D objects
    output: 3D PolySet
    operation:
        o Union all children
        o Perform extrude
 */
Response GeometryEvaluator::visit(State &state, const HelicoidalExtrudeNode &node)
{
    if (state.isPrefix() && isSmartCached(node)) return Response::PruneTraversal;
    if (state.isPostfix()) {
        std::shared_ptr<const Geometry> geom;
        if (!isSmartCached(node)) {
			std::shared_ptr<const Polygon2d> geometry;
            //const Geometry *geometry = nullptr;
            geometry = applyToChildren2D(node, OpenSCADOperator::UNION);

            if (geometry) {
                //const Polygon2d *polygons = dynamic_cast<const Polygon2d*>(geometry);
                geom = helicoidalPolygon(node, *geometry);
                //geom.reset(rotated);
                //delete geometry;
            }
        }
        else {
            geom = smartCacheGet(node, state.preferNef());
        }
        addToParent(state, node, geom);
        node.progress_report();
    }
    return Response::ContinueTraversal;
}

