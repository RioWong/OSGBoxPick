#include "boxpick.h"

BoxPicker::BoxPicker()
{
	x = 0.0f;
	y = 0.0f;
	bool OK = false;
	bool m_refresh = true;
}
BoxPicker::~BoxPicker()
{
}
bool BoxPicker::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	bool doit = false;
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (!viewer)
	{
		return false;
	}

	if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH)
	{
		x = ea.getXnormalized();
		y = ea.getYnormalized();
		x_pick = ea.getX();
		y_pick = ea.getY();
		OK = true;
		m_refresh = true;
		doit = true;
	}
	if (ea.getModKeyMask() == osgGA::GUIEventAdapter::MODKEY_LEFT_SHIFT)
	{
		doit = true;
	}
	if (ea.getEventType() == osgGA::GUIEventAdapter::DRAG)
	{
		if (OK&&ea.getModKeyMask() == osgGA::GUIEventAdapter::MODKEY_LEFT_SHIFT)
		{
            //doit = true;
			float end_x = ea.getXnormalized();
			float end_y = ea.getYnormalized();
			if (geometry.valid())
			{
				osg::Vec3Array* vertex = new osg::Vec3Array(4);
				(*vertex)[0] = osg::Vec3(x, 0, y);
				(*vertex)[1] = osg::Vec3(x, 0, end_y);
				(*vertex)[2] = osg::Vec3(end_x, 0, end_y);
				(*vertex)[3] = osg::Vec3(end_x, 0, y);

				geometry->setVertexArray(vertex);
				geometry->dirtyDisplayList();
			}
			geode->addDrawable(geometry.get());//绘制矩形框
		}

	}
	if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE)
	{
        m_refresh = false;
        doit = false;


		float pick_x = ea.getX();
		float pick_y = ea.getY();

		float xMin, xMax, yMin, yMax;
		xMin = osg::minimum(x_pick, pick_x);
		xMax = osg::maximum(x_pick, pick_x);
		yMin = osg::minimum(y_pick, pick_y);
		yMax = osg::maximum(y_pick, pick_y);//最初最終鼠标释放的矩形范围
		int iii = 0;
		osg::ref_ptr<osg::Node> node = new osg::Node();
		osg::ref_ptr<osg::Geode> m_geode = new osg::Geode();
		osg::ref_ptr<osg::Group> parent = new osg::Group();
		FILE *fp;
		fp = fopen("out_points.txt", "w");
		if (!fp)
		{
			std::cout << "error";
			return false;
		}
		int num = intersections.size();
		int nn = 0;
		osgUtil::PolytopeIntersector::Intersections intersections;
		osg::ref_ptr<osgUtil::PolytopeIntersector> intersector = new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, xMin, yMin, xMax, yMax);
		osgUtil::IntersectionVisitor iv(intersector.get());
		viewer->getCamera()->accept(iv);
		if (intersector->containsIntersections())
		{
			intersections = intersector->getIntersections();
			for (osgUtil::PolytopeIntersector::Intersections::iterator hitr = intersections.begin(); hitr != intersections.end(); ++hitr)
			{
				if (hitr->localIntersectionPoint[1] != 0)
					fprintf(fp, "%f %f %f\n", hitr->localIntersectionPoint[0], hitr->localIntersectionPoint[1], hitr->localIntersectionPoint[2]);
				nn++;
			}
			fclose(fp);
		}
        geometry->setVertexArray(NULL);
        geometry->dirtyDisplayList();
    }    

	return doit;
}
