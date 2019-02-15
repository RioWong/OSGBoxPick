#pragma once
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/PolytopeIntersector>
#include <osgUtil/LineSegmentIntersector>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgGA/OrbitManipulator>
#include <osg/MatrixTransform>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <iostream>
class lineBoxPick : public osgGA::GUIEventHandler
{
public:
    lineBoxPick();
    ~lineBoxPick();
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

    osg::ref_ptr<osg::MatrixTransform> addPoint(double x, double y, double z);
    void Pick(float x, float y);
private:
    double distPointToLine(osg::Vec2d start, osg::Vec2d end, osg::Vec2d point);//直线的起点、终点，最后选择的点
    void  getQuadCoord(osg::Vec2d start, osg::Vec2d end, osg::Vec2d point);//通过坐标系变换求得四个顶点值
public:
    osg::ref_ptr<osg::Group> root;
    osg::ref_ptr<osg::Group> tempRoot;
    osg::ref_ptr<osg::MatrixTransform> _PointPanoramicTransForm;
public:
    osgViewer::View* viewer;
    osg::ref_ptr<osg::Geode> geode;
    osg::ref_ptr<osg::Geometry> _Point;
    osg::ref_ptr<osg::Geometry> _Line;
    osg::ref_ptr<osg::Geometry> geometry;
private:
    osg::Vec2d V1, V2, V3, V4;
    std::vector<osg::Vec3d> _linePoints;
    std::vector<osg::Vec2d> _tempPoint;
    std::vector<osg::Vec2d> _mousePoints;
};

