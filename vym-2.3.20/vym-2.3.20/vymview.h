#ifndef VYMVIEW_H 
#define VYMVIEW_H

#include <QItemSelectionModel>
#include <QMainWindow>
#include <QWidget>

class DockEditor;
class VymModel;
class MapEditor;
class SlideEditor;
class TreeEditor;
class QTreeView;

class VymView:public QMainWindow
{
    Q_OBJECT
public:
    VymView(VymModel *model);
    ~VymView();
    void readSettings();
    VymModel* getModel();
    MapEditor* getMapEditor();
    bool treeEditorIsVisible();
    bool slideEditorIsVisible();
    void initFocus();
    void nextSlide();
    void previousSlide();

public slots:
    void changeSelection (const QItemSelection &newSel, const QItemSelection &delSel);
    void expandAll ();
    void expandOneLevel ();
    void collapseOneLevel ();
    void collapseUnselected();
    void showSelection ();
    void toggleTreeEditor();
    void toggleSlideEditor();

private:
    VymModel *model;
    TreeEditor *treeEditor;
    DockEditor *treeEditorDE;
    SlideEditor *slideEditor;
    DockEditor  *slideEditorDE;

    MapEditor *mapEditor;
    //DockEditor *mapEditorDE;
    QItemSelectionModel *selModel;
};


#endif

