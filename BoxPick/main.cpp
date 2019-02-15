#include <osg/observer_ptr>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgFX/Scribe>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgGA/OrbitManipulator>
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/PolytopeIntersector>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/Optimizer>
#include <iostream>
#include "boxpick.h"
#include "modelpick.h"
#include "lineBoxPick.h"

#include "stdafx.h"  
#include <osgDB/ReadFile>  
#include <osgViewer/Viewer>  
#include <osg/Node>  
#include <osgFX/Scribe>  
#include <osgGA/GUIEventHandler>  
#include <osgUtil/LineSegmentIntersector>  
//#pragma region 选点测试
//class CPickHandler :public osgGA::GUIEventHandler
//{
//public:
//    CPickHandler(osgViewer::Viewer *viewer) :mViewer(viewer){}
//    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
//    {
//        switch (ea.getEventType())
//        {
//            case osgGA::GUIEventAdapter::PUSH:
//                if (ea.getButton() == 1)
//                {
//                    Pick(ea.getX(), ea.getY());//可通过事件ea获得鼠标点击的坐标  
//                    std::cout << "鼠标选中点坐标为：" << ea.getX() << "   " << ea.getY()<<std::endl;
//                }
//                return true;
//        }
//        return false;
//    }
//protected:
//    void Pick(float x, float y)
//    {
//        osgUtil::LineSegmentIntersector::Intersections intersections;//声明一个相交测试的结果集  
//        if (mViewer->computeIntersections(x, y, intersections))//利用view的computerIntersection函数来测试屏幕与场景相交结果存入到结果集中  
//        {
//            osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
//            for (; hitr != intersections.end(); hitr++)
//            {
//                if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))
//                {
//                    const osg::NodePath& np = hitr->nodePath;
//                    for (int i = np.size() - 1; i >= 0; --i)
//                    {
//                        //将场景中的模型动态转换为Scribe类型，如果场景的模型中有Scribe节点则返回的是实际的模型Scribe对象  
//                        osgFX::Scribe *sc = dynamic_cast<osgFX::Scribe*>(np[i]);
//                        if (sc != NULL)//如果找到相应的sc，则隐藏起来  
//                        {
//                            std::cout << "选中点坐标为：" << hitr->getLocalIntersectPoint()[0] 
//                                << "   " << hitr->getLocalIntersectPoint()[1]
//                                << "   " << hitr->getLocalIntersectPoint()[2] << std::endl;
//                            if (sc->getNodeMask() != 0)
//                            {
//                                //sc->setNodeMask(0);
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//    osgViewer::Viewer *mViewer;
//};
//int _tmain(int argc, _TCHAR* argv[])
//{
//    osgViewer::Viewer viewer;
//    osg::ref_ptr<osg::Group> root = new osg::Group();
//    root->addChild(osgDB::readNodeFile("cessna.osg"));
//    osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");
//    osg::ref_ptr<osgFX::Scribe> sc = new osgFX::Scribe();//添加一个scribe节点，该节点下的模型会被加白描线高亮显示。  
//    sc->addChild(cow.get());//将模型牛加入到scribe节点中，那么加入之后，该牛就会有白边高亮显示  
//    root->addChild(sc.get());//将加白边的模型牛加入到默认的原点位置，默认为原点  
//    root->addChild(cow.get());//将模型牛加入到默认的原点位置，那么此时原模型牛和之前加入的白边牛就会重叠  
//    viewer.setSceneData(root.get());//将root节点加入到场景当中  
//    viewer.addEventHandler(new CPickHandler(&viewer));//为场景加入事件处理功能，并将场景本身传入到事件内部  
//    viewer.setUpViewOnSingleScreen(0);//设置单屏显示
//    viewer.realize();
//    osgViewer::GraphicsWindow * pWnd = dynamic_cast<osgViewer::GraphicsWindow*>(viewer.getCamera()->getGraphicsContext());
//    if (pWnd)
//    {
//        pWnd->setWindowRectangle(200, 200, 600, 600);
//        pWnd->setWindowDecoration(true);
//    }
//    return viewer.run();
//}
//#pragma endregion

//----------------------选点测试——lineBoxPick失败-----------------------------------------------

//#pragma region 选点测试
//int main(int argc, char* argv[])
//{
//    osg::ArgumentParser argument(&argc, argv);
//    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
//    osg::ref_ptr<osg::Group> root = new osg::Group;
//    //osg::ref_ptr<BoxPicker> boxpicker = new BoxPicker;
//    osg::ref_ptr<lineBoxPick> lineBoxPicker = new lineBoxPick;
//    lineBoxPicker->root = root;
//    viewer->setCameraManipulator(new osgGA::TrackballManipulator);
//    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
//    viewer->addEventHandler(lineBoxPicker.get());//绘线框选模式
//    lineBoxPicker->viewer = viewer;
//    //viewer->addEventHandler(new PickHandler());//
//
//    //绘图
//    osg::ref_ptr<osg::Geometry> geo = new osg::Geometry;
//    /*geo->setDataVariance(osg::Object::DYNAMIC);*/
//    geo->setUseDisplayList(false);
//
//    //定义画框的颜色
//    osg::Vec4Array* colors = new osg::Vec4Array;
//    colors->push_back(osg::Vec4(1.0, 1.0, 0.0, 1.0));
//    geo->setColorArray(colors);
//    geo->setColorBinding(osg::Geometry::BIND_OVERALL);
//
//    //定义法线
//    osg::Vec3Array* normal = new osg::Vec3Array(1);
//    (*normal)[0] = osg::Vec3(0, -1, 0);
//    geo->setNormalArray(normal);
//    geo->setNormalBinding(osg::Geometry::BIND_OVERALL);
//
//    //设置顶点关联方程
//    osg::ref_ptr<osg::DrawArrays> pri = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4);
//    geo->addPrimitiveSet(pri.get());
//
//    ////1.多边形选择模式
//    //osg::ref_ptr<osg::PolygonMode> polyMode = new osg::PolygonMode;
//    //polyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
//    //geo->getOrCreateStateSet()->setAttributeAndModes(polyMode.get());
//    //geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//
//
//    //设置相机
//    osg::Camera* camera = new osg::Camera;
//    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//    camera->setViewMatrixAsLookAt(osg::Vec3(0, -1, 0), osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 1));
//    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
//    /*camera->setRenderOrder(osg::Camera::POST_RENDER);*/
//    /*camera->setAllowEventFocus(false);*/
//    /*camera->setProjectionMatrix(osg::Matrix::ortho2D(-1.0, 1.0, -1.0, 1.0));*/
//
//    //2.绘制框选模式
//    //将绘图属性与绘图事件关联
//    lineBoxPicker->_Line = geo;//点
//    lineBoxPicker->_Point = geo;//线
//    lineBoxPicker->geometry = geo;//绘制矩形
//    //boxpicker->geometry = geo;
//    //关联绘图事件与相机
//    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//    lineBoxPicker->geode = geode;
//    camera->addChild(geode.get());
//
//    //加入根节点
//    osg::Node* model2 = osgDB::readNodeFile("Bunny.ply");
//    root->addChild(camera);
//    root->addChild(model2);
//    viewer->setSceneData(root.get());
//    viewer->setCameraManipulator(new osgGA::OrbitManipulator);//
//    viewer->setUpViewOnSingleScreen(0);//设置单屏显示
//    viewer->realize();
//    osgViewer::GraphicsWindow * pWnd = dynamic_cast<osgViewer::GraphicsWindow*>(viewer->getCamera()->getGraphicsContext());
//    if (pWnd)
//    {
//        pWnd->setWindowRectangle(200, 200, 600, 600);
//        pWnd->setWindowDecoration(true);
//    }
//    return viewer->run();
//}
//#pragma endregion 

//-------------------------------框选——成功-------------------------------------

int main(int argc, char* argv[])
{
    osg::ArgumentParser argument(&argc, argv);
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<BoxPicker> boxpicker = new BoxPicker;

    viewer->setCameraManipulator(new osgGA::TrackballManipulator);
    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
    viewer->addEventHandler(boxpicker.get());//框选模式
    //viewer->addEventHandler(new PickHandler());//

    //绘图
    osg::ref_ptr<osg::Geometry> geo = new osg::Geometry;
    /*geo->setDataVariance(osg::Object::DYNAMIC);*/
    geo->setUseDisplayList(false);

    //定义画框的颜色
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0, 1.0, 0.0, 1.0));
    geo->setColorArray(colors);
    geo->setColorBinding(osg::Geometry::BIND_OVERALL);

    //定义法线
    osg::Vec3Array* normal = new osg::Vec3Array(1);
    (*normal)[0] = osg::Vec3(0, -1, 0);
    geo->setNormalArray(normal);
    geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

    //设置顶点关联方程
    osg::ref_ptr<osg::DrawArrays> pri = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4);
    geo->addPrimitiveSet(pri.get());

    ////1.多边形选择模式
    //osg::ref_ptr<osg::PolygonMode> polyMode = new osg::PolygonMode;
    //polyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
    //geo->getOrCreateStateSet()->setAttributeAndModes(polyMode.get());
    //geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);


    //设置相机
    osg::Camera* camera = new osg::Camera;
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrixAsLookAt(osg::Vec3(0, -1, 0), osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 1));
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    /*camera->setRenderOrder(osg::Camera::POST_RENDER);*/
    /*camera->setAllowEventFocus(false);*/
    /*camera->setProjectionMatrix(osg::Matrix::ortho2D(-1.0, 1.0, -1.0, 1.0));*/

    //2.框选模式
    //将绘图属性与绘图事件关联
    boxpicker->geometry = geo;
    //关联绘图事件与相机
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    boxpicker->geode = geode;
    camera->addChild(geode.get());

    //加入根节点
    osg::Node* model2 = osgDB::readNodeFile("Bunny.ply");
    root->addChild(camera);
    root->addChild(model2);
    viewer->setSceneData(root.get());
    viewer->setCameraManipulator(new osgGA::OrbitManipulator);//
    viewer->setUpViewOnSingleScreen(0);//设置单屏显示
    viewer->realize();
    osgViewer::GraphicsWindow * pWnd = dynamic_cast<osgViewer::GraphicsWindow*>(viewer->getCamera()->getGraphicsContext());
    if (pWnd)
    {
        pWnd->setWindowRectangle(200, 200, 600, 600);
        pWnd->setWindowDecoration(true);
    }
    return viewer->run();
}