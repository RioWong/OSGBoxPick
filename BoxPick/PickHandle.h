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
class PickHandle
{
public:
    PickHandle();
    ~PickHandle();
protected:
    virtual bool handle(const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& aa);
private:
    osg::Vec3d getPos(const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);
public:
    // 经纬度信息
    void Picked(osg::Vec3d pos);
    void Moving(osg::Vec3d pos);

    // 世界坐标信息
    void PickedXYZ(osg::Vec3d pos);
    void MovingXYZ(osg::Vec3d pos);

    void RightPicked();

private:
    osg::Vec3d m_vecPostion;
    const osg::EllipsoidModel* m_pEllipsoid;
};

