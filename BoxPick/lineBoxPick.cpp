#include "stdafx.h"
#include "lineBoxPick.h"

#include <osg/Point>
#include <osg/BlendFunc>
#include <osg/Material>
#include <osg/BlendColor>
#include <osgText/Text>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osg/LineWidth>
#include <osg/MatrixTransform>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgFX/Scribe>  
#include <Eigen/Geometry>
#include "BB2DVistor.h"
lineBoxPick::lineBoxPick()
{
    _linePoints.clear();
    _tempPoint.clear();
    _mousePoints.clear();
}

lineBoxPick::~lineBoxPick()
{
}

bool lineBoxPick::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE &&
        ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON &&
        (ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_CTRL))
    {
        if (_mousePoints.size() == 2)
        {
            std::cout << "鼠标选中点" << _mousePoints.size() << "坐标为：" << ea.getX() << "   " << ea.getY() << std::endl;
            _mousePoints.push_back(osg::Vec2d(ea.getX(), ea.getY()));
            _tempPoint.clear();
            _tempPoint.swap(_mousePoints);
            std::vector<osg::Vec2d>().swap(_mousePoints);
            {
                osg::Vec2d start = _tempPoint[0];
                osg::Vec2d end   = _tempPoint[1];
                osg::Vec2d point = _tempPoint[2];
                getQuadCoord(start, end, point);//求得变换后的矩形区域
                if (geometry.valid())
                {
                    osg::Vec3Array* vertex = new osg::Vec3Array(4);
                    (*vertex)[0] = osg::Vec3(V1[0], 0, V1[1]);
                    (*vertex)[1] = osg::Vec3(V2[0], 0, V2[1]);
                    (*vertex)[2] = osg::Vec3(V3[0], 0, V3[1]);
                    (*vertex)[3] = osg::Vec3(V4[0], 0, V4[1]);
                    geometry->setVertexArray(vertex);
                    geometry->dirtyDisplayList();
                }
                geode->addDrawable(geometry.get());//绘制矩形框
            }
            {
                //遍历点云
                osg::ref_ptr<osg::Group> m_pTempTree = new osg::Group;
                BB2DVistor *pVisitor = new BB2DVistor();
                pVisitor->setBoundQuadix(V1[0], V1[1], V2[0], V2[1], V3[0], V3[1], V4[0], V4[1]);
                pVisitor->setNode(m_pTempTree);
                root->accept(*pVisitor);
            }

            //第三个点时清空vector
            //osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
            //(*colors)[0] = osg::Vec4(1, 1, 1, 1);
            //osg::Vec3 normal(0.0, 0.0, 1.0);
            //osg::Vec3dArray* selVertices = new osg::Vec3dArray;
            //osg::Vec3Array* normals = new osg::Vec3Array();
            //normals->push_back(normal);
            //osg::Vec3d center = _linePoints[0];
            //for (int i = 0; i < _linePoints.size(); ++i)
            //{
            //    selVertices->push_back(_linePoints[i] - center);
            //}
            //_Line = new osg::Geometry;
            //_Line->setUseDisplayList(true);
            //_Line->setUseVertexBufferObjects(true);
            //_Line->setVertexArray(selVertices);
            //_Line->setNormalArray(normals, osg::Array::BIND_PER_VERTEX);
            //_Line->setColorArray(colors.get());
            //_Line->setColorBinding(osg::Geometry::BIND_OVERALL);
            //_Line->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, selVertices->size()));
            //osg::ref_ptr<osg::Geode> geode = new osg::Geode;
            //geode->addDrawable(_Line.get());
            //geode->getOrCreateStateSet()->setAttribute(new osg::Point(4.0f));
            ////geode->getOrCreateStateSet()->setAttribute(new osg::StateAttribute::LineWidth(2.0f));
            //geode->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
            //geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
            //geode->setCullingActive(false);
            //osg::ref_ptr<osg::MatrixTransform> pointTrans = new osg::MatrixTransform;
            //pointTrans->setMatrix(osg::Matrix::translate(center));
            //pointTrans->addChild(geode);
            //root = new osg::Group;
            //root->addChild(pointTrans);
            ////if (DataManager().isSketchUp)
            //{
            //    //QString sFilePath = "excute.rb";
            //    //
            //    //QFile file(sFilePath);
            //    ////方式：Append为追加，WriteOnly，ReadOnly  
            //    //if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            //    //    QMessageBox::critical(NULL, "提示", "无法创建文件");
            //    //    return false;
            //    //}
            //    //QTextStream out(&file);
            //    //out << "model = Sketchup.active_model" << endl;
            //    //out << "entities = model.entities" << endl;
            //    //for (int i = 0; i < _linePoints.size(); ++i)
            //    //{
            //    //    osg::Vec3d lpt = _linePoints[i] - DataManager().TransPoint;
            //    //    QString pt = QString("pt%1=[%2,%3,%4]").arg(i).arg(lpt[0]).arg(lpt[1]).arg(lpt[2]);
            //    //    out << pt << endl;
            //    //}
            //    //QString addline = QString("new_face = entities.add_edges ");
            //    //for (int i = 0; i < _linePoints.size() - 1; ++i)
            //    //{
            //    //    addline += QString("pt%1,").arg(i);
            //    //
            //    //}
            //    //addline += QString("pt%1").arg(_linePoints.size() - 1);
            //    //out << addline << endl;
            //    //out.flush();
            //    //file.close();
            //    //QProcess p;
            //    //p.start(("SUB.exe"), QStringList("excute.rb"));
            //    //QProcess::startDetached(("SUB.exe"),QStringList("excute.rb"));
            //}
            //_linePoints.clear();
            //tempRoot = new osg::Group;
            //tempRoot->removeChildren(0, tempRoot->getNumChildren());
        }
        if (_mousePoints.size()>2)
        {
            std::vector<osg::Vec2d>().swap(_mousePoints);
        }
    }

    if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE &&
        ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON &&
        (ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_CTRL))
    {
        viewer = dynamic_cast<osgViewer::View*>(&aa);
        if (viewer)
        {
            std::cout << "鼠标选中点" << _mousePoints.size() << "坐标为：" << ea.getX() << "   " << ea.getY() << std::endl;
            _mousePoints.push_back(osg::Vec2d(ea.getX(), ea.getY()));
            osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
                new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
            osgUtil::IntersectionVisitor iv(intersector.get());
            viewer->getCamera()->accept(iv);
            //-------------------------------------------------------------------------
            if (intersector->containsIntersections())
            {
                osgUtil::LineSegmentIntersector::Intersection intersections = *(intersector->getIntersections().begin());
                _PointPanoramicTransForm = new osg::MatrixTransform;//根节点的子节点
                _PointPanoramicTransForm->addChild(root);
                root->addChild(_PointPanoramicTransForm);
                osg::Matrixd transformMatrix = _PointPanoramicTransForm->getMatrix();
                osg::Vec3d worldpoint = intersections.getWorldIntersectPoint();
                worldpoint = transformMatrix.inverse(transformMatrix).preMult(worldpoint);

                _linePoints.push_back(osg::Vec3d(worldpoint[0], worldpoint[1], worldpoint[2]));
                if (_linePoints.size() > 0)
                {
                    tempRoot->addChild(addPoint(_linePoints.back()[0], _linePoints.back()[1], _linePoints.back()[2]));
                    if (_linePoints.size() > 1)
                    {
                        osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
                        (*colors)[0] = osg::Vec4(1, 1, 1, 1);
                        osg::Vec3 normal(0.0, 0.0, 1.0);
                        osg::Vec3dArray* selVertices = new osg::Vec3dArray;
                        osg::Vec3Array* normals = new osg::Vec3Array();
                        normals->push_back(normal);
                        selVertices->push_back(osg::Vec3d(0, 0, 0));
                        selVertices->push_back(osg::Vec3d(worldpoint[0], worldpoint[1], worldpoint[2]) - _linePoints[_linePoints.size() - 2]);
                        std::cout << "选中点坐标为：" << worldpoint[0] << "  " << worldpoint[1] << "  " << worldpoint[2] << std::endl;
                        osg::ref_ptr<osg::Geometry> _Line = new osg::Geometry;
                        _Line->setUseDisplayList(true);
                        _Line->setUseVertexBufferObjects(true);
                        _Line->setVertexArray(selVertices);
                        _Line->setNormalArray(normals, osg::Array::BIND_PER_VERTEX);
                        _Line->setColorArray(colors.get());
                        _Line->setColorBinding(osg::Geometry::BIND_OVERALL);
                        _Line->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, selVertices->size()));
                        geode = new osg::Geode;
                        geode->addDrawable(_Line.get());
                        //geode->getOrCreateStateSet()->setAttribute(new osg::StateAttribute::LineWidth(2.0f));
                        geode->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
                        geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
                        geode->setCullingActive(false);
                        osg::ref_ptr<osg::MatrixTransform> pointTrans = new osg::MatrixTransform;
                        pointTrans->setMatrix(osg::Matrix::translate(_linePoints[_linePoints.size() - 2]));
                        pointTrans->addChild(geode);
                        tempRoot->addChild(pointTrans);
                    }
                }

            }
        }
    }
    return false;
}

void lineBoxPick::Pick(float x, float y)
{
    osgUtil::LineSegmentIntersector::Intersections intersections;//声明一个相交测试的结果集  
    if (viewer->computeIntersections(x, y, intersections))//利用view的computerIntersection函数来测试屏幕与场景相交结果存入到结果集中  
    {
        osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
        for (; hitr != intersections.end(); hitr++)
        {
            if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))
            {
                const osg::NodePath& np = hitr->nodePath;
                for (int i = np.size() - 1; i >= 0; --i)
                {
                    //将场景中的模型动态转换为Scribe类型，如果场景的模型中有Scribe节点则返回的是实际的模型Scribe对象  
                    osgFX::Scribe *sc = dynamic_cast<osgFX::Scribe*>(np[i]);
                    if (sc != NULL)//如果找到相应的sc，则隐藏起来  
                    {
                        std::cout << "选中点坐标为：" << hitr->getLocalIntersectPoint()[0]
                            << "   " << hitr->getLocalIntersectPoint()[1]
                            << "   " << hitr->getLocalIntersectPoint()[2] << std::endl;
                        if (sc->getNodeMask() != 0)
                        {
                            sc->setNodeMask(0);
                        }
                    }
                }
            }
        }
    }
}

osg::ref_ptr<osg::MatrixTransform> lineBoxPick::addPoint(double x, double y, double z)
{
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
    (*colors)[0] = osg::Vec4(1.0, 1.0, 1.0, 1.0);
    osg::Vec3 normal(0.0, 0.0, 1.0);
    osg::Vec3dArray* selVertices = new osg::Vec3dArray;
    osg::Vec3Array* normals = new osg::Vec3Array();
    normals->push_back(normal);
    selVertices->push_back(osg::Vec3d(0, 0, 0));
    _Point = new osg::Geometry;
    _Point->setUseDisplayList(true);
    _Point->setUseVertexBufferObjects(true);
    _Point->setVertexArray(selVertices);
    _Point->setNormalArray(normals, osg::Array::BIND_PER_VERTEX);
    _Point->setColorArray(colors.get());
    _Point->setColorBinding(osg::Geometry::BIND_OVERALL);
    _Point->addPrimitiveSet(new osg::DrawArrays(GL_POINTS, 0, 1));

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(_Point.get());
    geode->getOrCreateStateSet()->setAttribute(new osg::Point(4.0f));
    geode->getOrCreateStateSet()->setMode(GL_POINT_SMOOTH, osg::StateAttribute::ON);
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    geode->setCullingActive(false);

    osg::Vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
    osgText::Text* text = new osgText::Text;
    geode->addDrawable(text);
    //设置字体 
    //设置位置
    text->setPosition(osg::Vec3(0, 0, 0));
    text->setCharacterSize(30);
    //text->setText((QString("(X:%1,Y:%2,Z:%3)").arg(x, 0, 'f', 3).arg(y, 0, 'f', 3).arg(z, 0, 'f', 3)).toStdString());
    text->setAxisAlignment(osgText::Text::AxisAlignment::SCREEN);
    text->setColor(color);
    text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
    text->setBackdropType(osgText::Text::OUTLINE);//对文字进行描边
    text->setBackdropColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));//描边颜色

    osg::ref_ptr<osg::MatrixTransform> pointTrans = new osg::MatrixTransform;
    //pointTrans->ref();
    pointTrans->setMatrix(osg::Matrix::translate(x, y, z));
    pointTrans->addChild(geode);
    return pointTrans;
}

double lineBoxPick::distPointToLine(osg::Vec2d start, osg::Vec2d end, osg::Vec2d point)
{
    return abs((point[0] - start[0])*(end[1] - start[1]) - (end[0] - start[0]) * (point[1] - start[1])) /
        sqrt((end[0] - start[0]) * (end[0] - start[0]) + (end[1] - start[1])*(end[1] - start[1]));
}

void  lineBoxPick::getQuadCoord(osg::Vec2d start, osg::Vec2d end, osg::Vec2d point)
{
    Eigen::Matrix3d rotateMatrix;
    Eigen::AngleAxisd rotateVec(M_PI / 2, Eigen::Vector3d(0, 0, 1.0));
    rotateMatrix = rotateVec.toRotationMatrix();//转为旋转矩阵
    Eigen::Vector3d vecOrig(end[0] - start[0], end[1] - start[1], 0);
    Eigen::Vector3d vecRotated = rotateVec * vecOrig;
    vecRotated.normalize();
    Eigen::Vector3d p1(start[0], start[1], 0);
    Eigen::Vector3d p2(end[0], end[1], 0);
    double dist = distPointToLine(start, end, point);
    Eigen::Vector3d vert1 = p1 + vecRotated * dist;
    Eigen::Vector3d vert2 = p1 - vecRotated * dist;
    Eigen::Vector3d vert3 = p2 + vecRotated * dist;
    Eigen::Vector3d vert4 = p2 - vecRotated * dist;
    V1[0] = vert1[0];
    V1[1] = vert1[1];
    V2[0] = vert2[0];
    V2[1] = vert2[1];
    V3[0] = vert3[0];
    V3[1] = vert3[1];
    V4[0] = vert4[0];
    V4[1] = vert4[1];
}