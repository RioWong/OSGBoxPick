#pragma once
#include <osgDB/ReadFile>
#include <osgDB/Archive>
#include <osg/NodeVisitor>
#include <osg/Matrix>

const int RightHandSide = -1;
const int LeftHandSide = +1;
const int CollinearOrientation = 0;

class BB2DVistor : public osg::NodeVisitor
{
public:
    BB2DVistor() : osg::NodeVisitor(NODE_VISITOR, TRAVERSE_ALL_CHILDREN)
    {
        allinpoly_ = new osg::Group;
        partinpoly_ = new osg::Group;
        isfirst = true;
        maxfilenamesize = 0;
        m_dX1 = m_dY1 = m_dX2 = m_dY2 = m_dX3 = m_dY3 = m_dX4 = m_dY4 = 0.0;
    }

    ~BB2DVistor(){};

    inline void setNode(osg::Group* gp)
    {
        inpoly_ = gp;
        inpoly_->removeChildren(0, inpoly_->getNumChildren());
        inpoly_->addChild(allinpoly_);
        inpoly_->addChild(partinpoly_);
    }

    inline void setBoundQuadix(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
    {
        m_dX1 = x1;
        m_dY1 = y1;
        m_dX2 = x2;
        m_dY2 = y2;
        m_dX3 = x3;
        m_dY3 = y3;
        m_dX4 = x4;
        m_dY4 = y4;
    }

    virtual void apply(osg::PagedLOD &lodNode);

private:
    double m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3, m_dX4, m_dY4;  //������ĸ�����
    osg::ref_ptr<osg::Group> inpoly_;   //����Ľڵ�
    osg::ref_ptr<osg::Group> allinpoly_;    //���е㶼�ڶ�����ڲ����ӽڵ�
    osg::ref_ptr<osg::Group> partinpoly_;   //���ֵ��ڶ�����ڲ����ӽڵ�
    int maxfilenamesize;    //�����ļ�������
public:
    bool isfirst;   //�Ƿ��һ��

};

