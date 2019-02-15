#include <osgUtil/IntersectionVisitor>
#include <osgUtil/PolytopeIntersector>
#include <osgUtil/LineSegmentIntersector>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgGA/OrbitManipulator>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <iostream>


class BoxPicker : public osgGA::GUIEventHandler
{
public:
	BoxPicker();
	~BoxPicker();
private:
	float x, y;
	float x_pick, y_pick;
	bool OK;
	bool m_refresh;
	osgUtil::LineSegmentIntersector::Intersections intersections;
public:
	osg::observer_ptr<osg::Geometry> geometry;
	osg::observer_ptr<osg::Geode> geode = new osg::Geode;
private:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
};