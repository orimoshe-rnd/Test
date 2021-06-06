#include "osgbillboard.h"

osg::Drawable* createAxis(const osg::Vec3& corner, const osg::Vec3& xdir, osg::Vec4 markColor)
{
    // set up the Geometry.
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    osg::Vec3Array* coords = new osg::Vec3Array(2);
    (*coords)[0] = corner;
    (*coords)[1] = corner + xdir;

    geom->setVertexArray(coords);

    osg::Vec4 x_color(markColor);

    osg::Vec4Array* color = new osg::Vec4Array(2);
    (*color)[0] = x_color;
    (*color)[1] = x_color;

    geom->setColorArray(color, osg::Array::BIND_PER_VERTEX);

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));

    osg::StateSet* stateset = new osg::StateSet;
    osg::LineWidth* linewidth = new osg::LineWidth();
    linewidth->setWidth(4);
    stateset->setAttributeAndModes(linewidth, osg::StateAttribute::ON);
    stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    geom->setStateSet(stateset);

    return geom.release();
}

osg::Drawable* createLine(const osg::Vec3& start, const osg::Vec3& end, osg::Vec4 markColor)
{
    // set up the Geometry.
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    osg::Vec3Array* coords = new osg::Vec3Array(2);
    (*coords)[0] = start;
    (*coords)[1] = end;

    geom->setVertexArray(coords);

    osg::Vec4 x_color(markColor);

    osg::Vec4Array* color = new osg::Vec4Array(2);
    (*color)[0] = x_color;
    (*color)[1] = x_color;

    geom->setColorArray(color, osg::Array::BIND_PER_VERTEX);

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));

    osg::StateSet* stateset = new osg::StateSet;
    osg::LineWidth* linewidth = new osg::LineWidth();
    linewidth->setWidth(4);
    stateset->setAttributeAndModes(linewidth, osg::StateAttribute::ON);
    stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    geom->setStateSet(stateset);

    return geom.release();
}

/** create quad at specified position. */
osg::Drawable* createSquare(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, osg::ref_ptr<osg::Image> image, osg::Vec4 markColor)
{
    // set up the Geometry.
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    osg::Vec3Array* coords = new osg::Vec3Array(4);
    (*coords)[0] = corner;
    (*coords)[1] = corner+width;
    (*coords)[2] = corner+width+height;
    (*coords)[3] = corner+height;


    geom->setVertexArray(coords);

    osg::Vec3Array* norms = new osg::Vec3Array(1);
    (*norms)[0] = width^height;
    (*norms)[0].normalize();

    geom->setNormalArray(norms, osg::Array::BIND_OVERALL);

    osg::Vec2Array* tcoords = new osg::Vec2Array(4);
    (*tcoords)[0].set(0.0f,0.0f);
    (*tcoords)[1].set(1.0f,0.0f);
    (*tcoords)[2].set(1.0f,1.0f);
    (*tcoords)[3].set(0.0f,1.0f);
    geom->setTexCoordArray(0,tcoords);

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

    //material->setDiffuse(osg::Material::Face::FRONT ,markColor);

    if (image)
    {
        osg::StateSet* stateset = geom->getOrCreateStateSet();
        osg::Material* material = new osg::Material; //For material
        
        material->setDiffuse(osg::Material::Face::FRONT ,markColor);
        osg::Texture2D* texture = new osg::Texture2D;
        texture->setImage(image);


        stateset->setTextureAttribute(0,texture,osg::StateAttribute::OVERRIDE);
        stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
        stateset->setAttribute(material,osg::StateAttribute::OVERRIDE); //For material

        osg::BlendFunc* bf = new  osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_DST_COLOR );
        stateset->setAttributeAndModes(bf);

        stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
        stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

    }

    return geom.release();
}

osg::ref_ptr<osg::Node> createImageModel(osg::Vec3 pos, osg::Vec4 markColor, osg::Image* image)
{
    qDebug() <<"COLOR:"<< markColor[3];
    // create the root node which will hold the model.
    osg::ref_ptr<osg::Group> root = new osg::Group();

    // add the drawable into a single geode to be shared...
    osg::Billboard* center = new osg::Billboard();
    center->setMode(osg::Billboard::POINT_ROT_EYE);
    center->addDrawable(
                            createSquare(osg::Vec3(-0.5f / 2, 0.0f, -1 / 4),osg::Vec3(1.0f / 2,0.0f,0.0f),osg::Vec3(0.0f,0.0f,1.0f / 2),
                            image, markColor),
                            pos
                        );


    root->addChild(center);
    
    return root;
}

osg::Geode* createAxisModel(osg::Vec3 startPos, osg::Vec3 endPos, osg::Vec4 markColor)
{
    osg::Geode* axis = new osg::Geode();

    axis->addDrawable(createLine(startPos, endPos, markColor));

    return axis;
}