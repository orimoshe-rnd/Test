#pragma once
#include <osg/Node>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Material>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/Billboard>
#include <osg/LineWidth>
#include <osg/DrawPixels>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osg/BlendFunc>

#include <QDebug> 

typedef std::vector< osg::ref_ptr<osg::Image> > ImageList;

void initImage(osg::Image* image);
osg::ref_ptr<osg::Node> createImageModel(osg::Vec3 pos, osg::Vec4 markColor, osg::Image* image);
osg::Geode* createAxisModel(osg::Vec3 startPos, osg::Vec3 endPos, osg::Vec4 markColor);
osg::Drawable* createAxis(const osg::Vec3& corner, const osg::Vec3& xdir, osg::Vec4 markColor);
osg::Drawable* createSquare(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, osg::ref_ptr<osg::Image> image);