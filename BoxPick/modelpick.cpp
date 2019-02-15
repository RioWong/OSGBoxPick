#include "modelpick.h"

PickHandler::PickHandler()
{}
PickHandler::~PickHandler()
{}
void PickHandler::pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea)
{

	if (view->computeIntersections(ea, intersections))
	{
		for (osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			std::ostringstream os;
			std::ostringstream ob;
			os << "(" << hitr->getLocalIntersectPoint() << ")" << std::endl;
			ob << "(" << hitr->getWorldIntersectPoint() << ")" << std::endl;
            std::cout << "(" << hitr->getLocalIntersectPoint() << ")" << std::endl;
            std::cout << "(" << hitr->getWorldIntersectPoint() << ")" << std::endl;
			gdlist = os.str();
			gdlists = ob.str();
			break;
		}
	}
}
bool PickHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	bool doit = false;
	switch (ea.getEventType())
	{
	case (osgGA::GUIEventAdapter::PUSH) :
	{
		osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
		if (view) pick(view, ea);
		return false;
	}
	case(osgGA::GUIEventAdapter::KEYDOWN) :
	{
		 if (ea.getKey() == 'c')
			 {
				osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
				osg::ref_ptr<osgGA::GUIEventAdapter> event = new osgGA::GUIEventAdapter(ea);
				event->setX((ea.getXmin() + ea.getXmax())*0.5);
				event->setY((ea.getYmin() + ea.getYmax())*0.5);
				if (view)
					{
						pick(view, *event);
					}
			 }
				return false;
		 if (ea.getKey() == osgGA::GUIEventAdapter::MODKEY_LEFT_SHIFT)
			{
				int n = 1;
			}
	}
	default:
		return false;
	}
}
