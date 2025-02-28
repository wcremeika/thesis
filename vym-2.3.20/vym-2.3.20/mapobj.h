#ifndef MAPOBJ_H
#define MAPOBJ_H

#include <QGraphicsItem>

#include "xmlobj.h"

#define dZ_BBOX         0   // testing
#define dZ_DEPTH      100
#define dZ_FRAME_LOW   10	
#define dZ_LINK        20
#define dZ_XLINK       40
#define dZ_SELBOX      60
#define dZ_FLOATIMG    70
#define dZ_ICON        80
#define dZ_TEXT        90
#define  Z_INIT      9999
#define  Z_LINEEDIT 10000 

class ConvexPolygon;

#include "treeitem.h"

/*! \brief Base class for all objects visible on a map
*/

class MapObj:public XMLObj, public QGraphicsItem {
public:
    MapObj (QGraphicsItem *parent=NULL,TreeItem *ti=NULL);
    virtual ~MapObj ();
    virtual void init ();
    virtual void copy (MapObj*);

    virtual void setTreeItem(TreeItem *);
    virtual TreeItem* getTreeItem() const;

    virtual qreal x();
    virtual qreal y();
    virtual qreal width();
    virtual qreal height();
    virtual QPointF getAbsPos();
    virtual QString getPos();			//! Return position as string (x,y)
    virtual void move (double x,double y);      //! move to absolute Position
    virtual void move (QPointF p);
    virtual void moveBy (double x,double y);    //! move to relative Position

    virtual QRectF boundingRect () const;    
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

    virtual QRectF getBBox();			//! returns bounding box
    virtual ConvexPolygon getBoundingPolygon();	//! return bounding convex polygon
    virtual QPolygonF getClickPoly();		//! returns polygon to click
    virtual QPainterPath getClickPath();	//! returns path to click
    virtual bool isInClickBox (const QPointF &p);   //! Checks if p is in clickBox
    virtual QSizeF getSize();			//! returns size of bounding box

    virtual void setRotation(const qreal &a);
    virtual qreal getRotation();
    virtual bool isVisibleObj();
    virtual void setVisibility(bool);
    virtual void positionBBox()=0;       
    virtual void calcBBoxSize()=0;

protected:  
    QRectF bbox;		    // bounding box of MO itself
    QPolygonF clickPoly;		    // area where mouseclicks are found
    QPointF absPos;		    // Position on canvas
    bool visible;

    qreal angle;		    //! Rotation angle

    TreeItem *treeItem;		    //! Crossrefence to treemodel

    QGraphicsPolygonItem *boundingPolygon;	//FIXME-3 testing only
};

#endif
