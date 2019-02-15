#include "stdafx.h"
#include "BB2DVistor.h"
#include <osgDB/ReadFile>
#include <osgDB/Archive>
#include <osg/PagedLOD>
#include <osg/MatrixTransform>
//判断点在直线的那一边

int orientation(double x1, double y1, double  x2, double  y2, double  px, double  py)
{
    double orin = (x2 - x1) * (py - y1) - (px - x1) * (y2 - y1);

    if (orin > (0.0))      return LeftHandSide;
    else if (orin < (0.0)) return RightHandSide;
    else                    return CollinearOrientation;
}

//判断点是否在四边形的内部
bool point_in_quadix(double px, double py, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    int or1 = orientation(x1, y1, x2, y2, px, py);
    int or2 = orientation(x2, y2, x3, y3, px, py);
    int or3 = orientation(x3, y3, x4, y4, px, py);
    int or4 = orientation(x4, y4, x1, y1, px, py);

    if ((or1 == or2) && (or2 == or3) && (or3 == or4))
        return true;
    else if (0 == or1)
        return (0 == (or2 * or4));
    else if (0 == or2)
        return (0 == (or1 * or3));
    else if (0 == or3)
        return (0 == (or2 * or4));
    else if (0 == or4)
        return (0 == (or1 * or3));
    else
        return false;
}

//判断多边形是否在矩形内部
bool point_in_rectangle(double minx, double miny, double maxx, double maxy, double px, double py)
{
    if (px > minx && px < maxx && py > miny && py < maxy)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BB2DVistor::apply(osg::PagedLOD &lodNode)
{
    //获取绘制节点
    osg::ref_ptr<osg::PagedLOD> lod_ = dynamic_cast<osg::PagedLOD*>(&lodNode);
    if (lod_)
    {
        if (isfirst)
        {
            if (lod_->getFileName(0) != ""&&lod_->getDatabasePath() != "")
            {
                std::string archive_file = lod_->getDatabasePath();
                archive_file = archive_file.substr(0, archive_file.rfind('/'));
                std::string file_ext = archive_file.substr(archive_file.rfind('.'));
                if (file_ext == ".osga")
                {
                    osg::ref_ptr<osgDB::Archive> root_archive = osgDB::openArchive(archive_file, osgDB::Archive::READ);
                    osgDB::Archive::FileNameList fileNames;
                    if (root_archive->getFileNames(fileNames))
                    {
                        int number = 0;
                        for (osgDB::Archive::FileNameList::const_iterator itr = fileNames.begin(); itr != fileNames.end(); ++itr)
                        {
                            std::string current_file = *itr;
                            if (current_file.substr(0, 6) == "octree")
                            {
                                if (maxfilenamesize < (current_file.size()))
                                {
                                    maxfilenamesize = (current_file.size());
                                    number++;
                                }
                            }
                        }
                        if (maxfilenamesize != 0)
                        {
                            isfirst = false;
                        }
                    }
                }
            }
        }
        //获取节点信息
        if (!isfirst && lod_->getFileName(0) != ""&&lod_->getFileName(0).substr(0, 6) == "octree" && lod_->getFileName(0).size() == maxfilenamesize)
        {
            osg::ref_ptr<osg::MatrixTransform> mainiverootmt = dynamic_cast<osg::MatrixTransform*>(lod_->getParent(0));
            if (mainiverootmt)
            {
                double minx = mainiverootmt->getMatrix().getTrans()[0];
                double miny = mainiverootmt->getMatrix().getTrans()[1];
                double minz = mainiverootmt->getMatrix().getTrans()[2];

                osg::Vec3d lodcenter = lod_->getCenter();
                double lodlength = lod_->getRadius() * 10.0 / 9.0;
                double lodminx = lodcenter[0] + minx - lodlength / 2;
                double lodminy = lodcenter[1] + miny - lodlength / 2;
                double lodmaxx = lodcenter[0] + minx + lodlength / 2;
                double lodmaxy = lodcenter[1] + miny + lodlength / 2;

                //判断边界是否在点云范围内
                bool isBoundinLod = false;
                if (point_in_rectangle(lodminx, lodminy, lodmaxy, lodmaxy, m_dX1, m_dY1)
                    || point_in_rectangle(lodminx, lodminy, lodmaxy, lodmaxy, m_dX2, m_dY2)
                    || point_in_rectangle(lodminx, lodminy, lodmaxy, lodmaxy, m_dX3, m_dY3)
                    || point_in_rectangle(lodminx, lodminy, lodmaxy, lodmaxy, m_dX4, m_dY4))
                {
                    isBoundinLod = true;
                }
                //判断点云范围是否在边界内
                if (point_in_quadix(lodminx, lodminy, m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3, m_dX4, m_dY4)
                    || point_in_quadix(lodminx, lodmaxy, m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3, m_dX4, m_dY4)
                    || point_in_quadix(lodmaxx, lodmaxy, m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3, m_dX4, m_dY4)
                    || point_in_quadix(lodmaxx, lodminy, m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3, m_dX4, m_dY4)
                    || isBoundinLod)
                {
                    std::string childivefile = lod_->getDatabasePath() + lod_->getFileName(0);
                    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(childivefile);
                    node->setName(childivefile);
                    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
                    mt->setMatrix(osg::Matrix::translate(minx, miny, minz));
                    mt->addChild(node);
                    if (point_in_quadix(lodminx, lodminy, m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3, m_dX4, m_dY4)
                        && point_in_quadix(lodminx, lodmaxy, m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3, m_dX4, m_dY4)
                        && point_in_quadix(lodmaxx, lodmaxy, m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3, m_dX4, m_dY4)
                        && point_in_quadix(lodmaxx, lodminy, m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3, m_dX4, m_dY4))
                    {
                        allinpoly_->addChild(mt);
                    }
                    else
                    {
                        partinpoly_->addChild(mt);
                    }
                }
            }
        }
    }

    traverse(lodNode);
}