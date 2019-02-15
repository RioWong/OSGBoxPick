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
//#pragma region ѡ�����
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
//                    Pick(ea.getX(), ea.getY());//��ͨ���¼�ea��������������  
//                    std::cout << "���ѡ�е�����Ϊ��" << ea.getX() << "   " << ea.getY()<<std::endl;
//                }
//                return true;
//        }
//        return false;
//    }
//protected:
//    void Pick(float x, float y)
//    {
//        osgUtil::LineSegmentIntersector::Intersections intersections;//����һ���ཻ���ԵĽ����  
//        if (mViewer->computeIntersections(x, y, intersections))//����view��computerIntersection������������Ļ�볡���ཻ������뵽�������  
//        {
//            osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
//            for (; hitr != intersections.end(); hitr++)
//            {
//                if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))
//                {
//                    const osg::NodePath& np = hitr->nodePath;
//                    for (int i = np.size() - 1; i >= 0; --i)
//                    {
//                        //�������е�ģ�Ͷ�̬ת��ΪScribe���ͣ����������ģ������Scribe�ڵ��򷵻ص���ʵ�ʵ�ģ��Scribe����  
//                        osgFX::Scribe *sc = dynamic_cast<osgFX::Scribe*>(np[i]);
//                        if (sc != NULL)//����ҵ���Ӧ��sc������������  
//                        {
//                            std::cout << "ѡ�е�����Ϊ��" << hitr->getLocalIntersectPoint()[0] 
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
//    osg::ref_ptr<osgFX::Scribe> sc = new osgFX::Scribe();//���һ��scribe�ڵ㣬�ýڵ��µ�ģ�ͻᱻ�Ӱ����߸�����ʾ��  
//    sc->addChild(cow.get());//��ģ��ţ���뵽scribe�ڵ��У���ô����֮�󣬸�ţ�ͻ��аױ߸�����ʾ  
//    root->addChild(sc.get());//���Ӱױߵ�ģ��ţ���뵽Ĭ�ϵ�ԭ��λ�ã�Ĭ��Ϊԭ��  
//    root->addChild(cow.get());//��ģ��ţ���뵽Ĭ�ϵ�ԭ��λ�ã���ô��ʱԭģ��ţ��֮ǰ����İױ�ţ�ͻ��ص�  
//    viewer.setSceneData(root.get());//��root�ڵ���뵽��������  
//    viewer.addEventHandler(new CPickHandler(&viewer));//Ϊ���������¼������ܣ��������������뵽�¼��ڲ�  
//    viewer.setUpViewOnSingleScreen(0);//���õ�����ʾ
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

//----------------------ѡ����ԡ���lineBoxPickʧ��-----------------------------------------------

//#pragma region ѡ�����
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
//    viewer->addEventHandler(lineBoxPicker.get());//���߿�ѡģʽ
//    lineBoxPicker->viewer = viewer;
//    //viewer->addEventHandler(new PickHandler());//
//
//    //��ͼ
//    osg::ref_ptr<osg::Geometry> geo = new osg::Geometry;
//    /*geo->setDataVariance(osg::Object::DYNAMIC);*/
//    geo->setUseDisplayList(false);
//
//    //���廭�����ɫ
//    osg::Vec4Array* colors = new osg::Vec4Array;
//    colors->push_back(osg::Vec4(1.0, 1.0, 0.0, 1.0));
//    geo->setColorArray(colors);
//    geo->setColorBinding(osg::Geometry::BIND_OVERALL);
//
//    //���巨��
//    osg::Vec3Array* normal = new osg::Vec3Array(1);
//    (*normal)[0] = osg::Vec3(0, -1, 0);
//    geo->setNormalArray(normal);
//    geo->setNormalBinding(osg::Geometry::BIND_OVERALL);
//
//    //���ö����������
//    osg::ref_ptr<osg::DrawArrays> pri = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4);
//    geo->addPrimitiveSet(pri.get());
//
//    ////1.�����ѡ��ģʽ
//    //osg::ref_ptr<osg::PolygonMode> polyMode = new osg::PolygonMode;
//    //polyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
//    //geo->getOrCreateStateSet()->setAttributeAndModes(polyMode.get());
//    //geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//
//
//    //�������
//    osg::Camera* camera = new osg::Camera;
//    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//    camera->setViewMatrixAsLookAt(osg::Vec3(0, -1, 0), osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 1));
//    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
//    /*camera->setRenderOrder(osg::Camera::POST_RENDER);*/
//    /*camera->setAllowEventFocus(false);*/
//    /*camera->setProjectionMatrix(osg::Matrix::ortho2D(-1.0, 1.0, -1.0, 1.0));*/
//
//    //2.���ƿ�ѡģʽ
//    //����ͼ�������ͼ�¼�����
//    lineBoxPicker->_Line = geo;//��
//    lineBoxPicker->_Point = geo;//��
//    lineBoxPicker->geometry = geo;//���ƾ���
//    //boxpicker->geometry = geo;
//    //������ͼ�¼������
//    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//    lineBoxPicker->geode = geode;
//    camera->addChild(geode.get());
//
//    //������ڵ�
//    osg::Node* model2 = osgDB::readNodeFile("Bunny.ply");
//    root->addChild(camera);
//    root->addChild(model2);
//    viewer->setSceneData(root.get());
//    viewer->setCameraManipulator(new osgGA::OrbitManipulator);//
//    viewer->setUpViewOnSingleScreen(0);//���õ�����ʾ
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

//-------------------------------��ѡ�����ɹ�-------------------------------------

int main(int argc, char* argv[])
{
    osg::ArgumentParser argument(&argc, argv);
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<BoxPicker> boxpicker = new BoxPicker;

    viewer->setCameraManipulator(new osgGA::TrackballManipulator);
    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
    viewer->addEventHandler(boxpicker.get());//��ѡģʽ
    //viewer->addEventHandler(new PickHandler());//

    //��ͼ
    osg::ref_ptr<osg::Geometry> geo = new osg::Geometry;
    /*geo->setDataVariance(osg::Object::DYNAMIC);*/
    geo->setUseDisplayList(false);

    //���廭�����ɫ
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0, 1.0, 0.0, 1.0));
    geo->setColorArray(colors);
    geo->setColorBinding(osg::Geometry::BIND_OVERALL);

    //���巨��
    osg::Vec3Array* normal = new osg::Vec3Array(1);
    (*normal)[0] = osg::Vec3(0, -1, 0);
    geo->setNormalArray(normal);
    geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

    //���ö����������
    osg::ref_ptr<osg::DrawArrays> pri = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4);
    geo->addPrimitiveSet(pri.get());

    ////1.�����ѡ��ģʽ
    //osg::ref_ptr<osg::PolygonMode> polyMode = new osg::PolygonMode;
    //polyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
    //geo->getOrCreateStateSet()->setAttributeAndModes(polyMode.get());
    //geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);


    //�������
    osg::Camera* camera = new osg::Camera;
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrixAsLookAt(osg::Vec3(0, -1, 0), osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 1));
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    /*camera->setRenderOrder(osg::Camera::POST_RENDER);*/
    /*camera->setAllowEventFocus(false);*/
    /*camera->setProjectionMatrix(osg::Matrix::ortho2D(-1.0, 1.0, -1.0, 1.0));*/

    //2.��ѡģʽ
    //����ͼ�������ͼ�¼�����
    boxpicker->geometry = geo;
    //������ͼ�¼������
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    boxpicker->geode = geode;
    camera->addChild(geode.get());

    //������ڵ�
    osg::Node* model2 = osgDB::readNodeFile("Bunny.ply");
    root->addChild(camera);
    root->addChild(model2);
    viewer->setSceneData(root.get());
    viewer->setCameraManipulator(new osgGA::OrbitManipulator);//
    viewer->setUpViewOnSingleScreen(0);//���õ�����ʾ
    viewer->realize();
    osgViewer::GraphicsWindow * pWnd = dynamic_cast<osgViewer::GraphicsWindow*>(viewer->getCamera()->getGraphicsContext());
    if (pWnd)
    {
        pWnd->setWindowRectangle(200, 200, 600, 600);
        pWnd->setWindowDecoration(true);
    }
    return viewer->run();
}