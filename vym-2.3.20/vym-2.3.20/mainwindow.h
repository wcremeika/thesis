#ifndef MAINWINDOW_H 
#define MAINWINDOW_H

#include <QMainWindow>

#include "branchpropeditor.h"
#include "extrainfodialog.h"
#include "flag.h"
#include "file.h"
#include "historywindow.h"
#include "mapeditor.h"
#include "shortcuts.h"
#include "texteditor.h"
#include "vymview.h"

class Main : public QMainWindow 
{
    Q_OBJECT

public:
    /*! Modifier modes are used when CTRL together with a mouse button is pressed */
    enum ModMode {
	ModModeNone,	//!< Unused
	ModModeColor,	//!< Pick color from object
	ModModeCopy,	//!< Copy object
	ModModeXLink	//!< Create a XLink (XLinkObj) from selected object
	};

    Main(QWidget* parent=0, Qt::WFlags f=0);
    ~Main();
    void loadCmdLine();

private:
    QProgressDialog progressDialog;
    int progressMax;
    int progressCounter;
    int progressCounterTotal;
public:	
    void statusMessage (const QString &);
    void setProgressMaximum (int max);
    void addProgressValue (float v);
    void initProgressCounter(uint n=1);
    void removeProgressCounter();

public slots:
    void fileNew();
    void fileNewCopy();

protected:
    void closeEvent( QCloseEvent* );

private:
    void setupAPI();
    void setupFileActions();
    void setupEditActions();
    void setupSelectActions();
    void setupFormatActions();
    void setupViewActions();
    void setupModeActions();
    void setupWindowActions();
    void setupFlag(
	Flag *flag, 
	QToolBar *tb, 
	const QString &name, 
	const QString &tooltip,
	const QKeySequence &ks=0
	);
    void setupFlagActions();
    void setupNetworkActions();
    void setupSettingsActions();
    void setupTestActions();
    void setupHelpActions();
    void setupContextMenus();
    void setupRecentMapsMenu();
    void setupMacros();
    void setupToolbars();
    VymView* currentView() const;
public:	
    MapEditor* currentMapEditor() const;
    VymModel* currentModel() const;
    uint currentModelID() const;
    VymModel* getModel(uint) const;
    void gotoModel (VymModel *m);
    int modelCount();
    
private slots:
    void editorChanged(QWidget*);

    File::ErrorCode fileLoad(QString ,const LoadMode &, const FileType &ftype);
    void fileLoad(const LoadMode &);
    void fileLoad();
    void fileSaveSession();
public slots:    
    void fileRestoreSession();
private slots:    
    void fileLoadRecent();
    void addRecentMap (const QString &);
    void fileSave(VymModel*, const SaveMode & );
    void fileSave();
public slots:	
    void fileSave(VymModel*);	// autosave from MapEditor
private slots:	
    void fileSaveAs(const SaveMode &);
    void fileSaveAs();
    void fileImportKDE4Bookmarks();
    void fileImportFirefoxBookmarks();
    void fileImportFreemind();
    void fileImportMM();
    void fileImportDir();
    void fileExportXML();
    void fileExportHTML();
    void fileExportImage();
    void fileExportPDF();
    void fileExportSVG();
    void fileExportAO();
    void fileExportASCII();
    void fileExportCSV();
    void fileExportOrgMode();
    void fileExportLaTeX();
    void fileExportKDE4Bookmarks();
    void fileExportTaskjuggler();
    void fileExportImpress();
    void fileExportLast();
    bool closeTab(int i);
    bool fileCloseMap();
    void filePrint();
    bool fileExitVYM();

public slots:
    void editUndo();	
    void editRedo();	
    void gotoHistoryStep (int);
private slots:	
    void editCopy();	
    void editPaste();	
    void editCut(); 

public slots:    
    void updateQueries (VymModel*);
private:
    bool openURL(const QString &url);
    void openTabs(QStringList);
public slots:
    void editOpenURL();
    void editOpenURLTab();
private slots:
    void editOpenMultipleVisURLTabs(bool ignoreScrolled=true);
    void editOpenMultipleURLTabs();
    void editNote2URLs();
    void editURL();
    void editLocalURL();
    void editHeading2URL();
    void editBugzilla2URL();
    void getBugzillaData();
    void getBugzillaDataSubtree();
    void editFATE2URL();
    void openVymLinks(const QStringList &, bool background=false);
    void editVymLink();
    void editOpenMultipleVymLinks();
public slots:
    void editHeading();
    void editHeadingFinished(VymModel *m);
    void editOpenVymLink(bool background=false);
    void editOpenVymLinkBackground();
private slots:
    void editDeleteVymLink();
    void editToggleHideExport();
    void editToggleTask();
    void editCycleTaskStatus();
    void editTaskSleepN();
    void editAddTimestamp();
    void editMapInfo();
    void editMoveUp();	
    void editMoveDown();    
    void editDetach();	
    void editSortChildren();
    void editSortBackChildren();
    void editToggleScroll();
    void editExpandAll();
    void editExpandOneLevel();
    void editCollapseOneLevel();
    void editCollapseUnselected();
    void editUnscrollChildren();
    void editGrowSelectionSize();
    void editShrinkSelectionSize();
    void editResetSelectionSize();
    void editAddAttribute();
    void editAddMapCenter();
    void editNewBranch();
    void editNewBranchBefore();
    void editNewBranchAbove();
    void editNewBranchBelow();
    void editImportAdd();
    void editImportReplace();
    void editSaveBranch();
    void editDeleteKeepChildren();
    void editDeleteChildren();
    void editDeleteSelection();
    void editLoadImage();
    void editSaveImage();
    void popupFollowXLink ();
    void editFollowXLink (QAction *);
    void editEditXLink (QAction *);

private slots:    
    void editToggleTarget();
    void editGoToTarget();
    void editMoveToTarget();
    void editSelectPrevious();
    void editSelectNext();
    void editSelectNothing();
    void editOpenFindResultWidget();
    void editFindNext(QString s);
    void editFindDuplicateURLs();

public slots:
    void formatPickColor();
    QColor getCurrentColor();
    void setCurrentColor(QColor);

private slots:    
    void formatSelectColor();
    void formatColorBranch();
    void formatColorSubtree();
    void formatLinkStyleLine();
    void formatLinkStyleParabel();
    void formatLinkStylePolyLine();
    void formatLinkStylePolyParabel();
    void formatSelectBackColor();
    void formatSelectBackImage();
    void formatSelectLinkColor();
    void formatSelectSelectionColor();
    void formatSelectFont();
    void formatToggleLinkColorHint();
    void formatHideLinkUnselected();

public slots:
    void viewZoomReset();
    void viewZoomIn();
    void viewZoomOut();
    void viewRotateCounterClockwise();
    void viewRotateClockwise();
    void viewCenter();

public slots:
    void networkStartServer();
    void networkConnect();
    bool settingsPDF();
    bool settingsURL();
    void settingsMacroDir();
    void settingsUndoLevels();

public:
    bool useAutosave();
    void setAutosave( bool b);

public slots:
    void settingsAutosaveTime();
    void settingsTaskShowParentsLevel();
    void settingsAutoLayoutToggle();
    void settingsWriteBackupFileToggle();
    void settingsToggleAnimation();

    void windowToggleNoteEditor();
    void windowToggleTreeEditor();
    void windowToggleTaskEditor();
    void windowToggleSlideEditor();
    void windowToggleScriptEditor();
    void windowToggleHistory();
    void windowToggleProperty();
    void updateHistory(SimpleSettings &);
    void windowToggleAntiAlias();
public:
    bool isAliased();
    bool hasSmoothPixmapTransform();
public slots:
    void windowToggleSmoothPixmap();
    void updateHeading();
    void updateNoteFlag();
    void updateNoteEditor (QModelIndex index);
    void selectInNoteEditor (QString s, int i);
    void changeSelection (VymModel *model,const QItemSelection &newSel, const QItemSelection &delSel);

    void updateActions();
    ModMode getModMode();
    bool autoEditNewBranch();
    bool autoSelectNewBranch();
    void setScriptFile(const QString &fn);
    QVariant execute(const QString &);
    void executeEverywhere (const QString &);
    void gotoWindow (const int &n);

private slots:
    void windowNextEditor();
    void windowPreviousEditor();
    void nextSlide();
    void previousSlide();

    void standardFlagChanged();

    void testFunction1();
    void testFunction2();
    void toggleWinter();
    void toggleHideExport();
    void testCommand();

    void helpDoc();
    void helpDemo();
    void helpShortcuts();
    void helpAbout();
    void helpAboutQT();

    void callMacro ();

private:
    QTabWidget *tabWidget;
    qint64 *browserPID;

    QStringList imageTypes;

    QList <VymView*> vymViews;	    //! Keeps track of models and views related to a tab 
    QString prevSelection;

    HistoryWindow *historyWindow;

    QDockWidget *headingEditorDW;
    QDockWidget *noteEditorDW;
    QDockWidget *scriptEditorDW;

    BranchPropertyEditor *branchPropertyEditor;

    QList <QAction*> actionListMap;
    QList <QAction*> actionListBranches;
    QList <QAction*> actionListItems;

    QColor currentColor;

    int xLinkMenuWidth;

    QMenu *recentFilesMenu;
    enum { MaxRecentFiles = 9 };
    QAction *recentFileActions[MaxRecentFiles];

    QAction *macroActions[13];
    QStringList macro;

    Switchboard switchboard;

    QMenu *toolbarsMenu;
    QToolBar *fileToolbar;
    QToolBar *clipboardToolbar;
    QToolBar *editActionsToolbar;
    QToolBar *selectionToolbar;
    QToolBar *editorsToolbar;
    QToolBar *colorsToolbar;
    QToolBar *zoomToolbar;
    QToolBar *modModesToolbar;
    QToolBar *referencesToolbar;
    QToolBar *standardFlagsToolbar;

    QAction* actionFileNew;
    QAction* actionFileNewCopy;
    QAction* actionFileOpen;
    QAction* actionFileSave;
    QAction* actionFilePrint;
    QAction* actionFileExportLast;
    QAction* actionUndo;
    QAction* actionRedo;
    QAction *actionCopy;
    QAction *actionCut;
    QAction *actionPaste;
    QAction *actionMoveUp;
    QAction *actionMoveDown;
    QAction *actionDetach;
    QAction *actionSortChildren;
    QAction *actionSortBackChildren;
    QAction *actionToggleScroll;
    QAction *actionExpandAll;
    QAction *actionExpandOneLevel;
    QAction *actionCollapseOneLevel;
    QAction *actionCollapseUnselected;
    QAction* actionOpenURL;
    QAction* actionOpenURLTab;
    QAction* actionOpenMultipleVisURLTabs;
    QAction* actionOpenMultipleURLTabs;
    QAction* actionGetURLsFromNote;
    QAction* actionURLNew;
    QAction* actionLocalURL;
    QAction* actionHeading2URL;
    QAction* actionBugzilla2URL;
    QAction* actionGetBugzillaData;
    QAction* actionGetBugzillaDataSubtree;
    QAction* actionFATE2URL;
    QAction *actionOpenVymLink;
    QAction *actionOpenVymLinkBackground;
    QAction *actionOpenMultipleVymLinks;
    QAction *actionEditVymLink;
    QAction *actionDeleteVymLink;
    QAction *actionAddTimestamp;
    QAction *actionToggleTask;
    QAction *actionCycleTaskStatus;
    QAction *actionTaskSleep0;
    QAction *actionTaskSleepN;
    QAction *actionTaskSleep1;
    QAction *actionTaskSleep2;
    QAction *actionTaskSleep3;
    QAction *actionTaskSleep5;
    QAction *actionTaskSleep7;
    QAction *actionTaskSleep14;
    QAction *actionTaskSleep28;
    QAction *actionToggleHideExport;
    QAction *actionMapInfo;
    QAction *actionHeading;
    QAction *actionDelete;
    QAction *actionAddAttribute;
public:
    QAction *actionAddMapCenter;

private:    
    QAction *actionAddBranch;
    QAction *actionAddBranchBefore;
    QAction *actionAddBranchAbove;
    QAction *actionAddBranchBelow;
    QAction *actionDeleteKeepChildren;
    QAction *actionDeleteChildren;
    QAction *actionImportAdd;
    QAction *actionImportReplace;
    QAction *actionSaveBranch;
    QAction *actionLoadImage;

    QAction *actionGrowSelectionSize;
    QAction *actionShrinkSelectionSize;
    QAction *actionResetSelectionSize;

    QAction *actionToggleTarget;
    QAction *actionGoToTarget;
    QAction *actionMoveToTarget;
    QAction *actionSelectPrevious;
    QAction *actionSelectNext;
    QAction *actionSelectNothing;
    QAction *actionFind;

    QAction *actionFormatColor;
    QAction *actionFormatPickColor;
    QAction *actionFormatColorBranch;
    QAction *actionFormatColorSubtree;
    QAction *actionFormatLinkColorHint;
    QAction *actionFormatBackColor;
    QAction *actionFormatBackImage;
    QAction *actionFormatLinkColor;
    QAction *actionFormatSelectionColor;
    QAction *actionFormatFont;

    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionZoomReset;
    QAction *actionRotateCounterClockwise;
    QAction *actionRotateClockwise;
    QAction *actionCenterOn;

    QActionGroup *actionGroupModModes;
    QAction *actionModModeColor;
    QAction *actionModModeXLink;
    QAction *actionModModeCopy;

    QAction *actionToggleHideMode;

    QAction *actionToggleWinter;

    QActionGroup *actionGroupFormatFrameTypes;

    QActionGroup *actionGroupFormatLinkStyles;
    QAction *actionFormatLinkStyleLine;
    QAction *actionFormatLinkStyleParabel;
    QAction *actionFormatLinkStylePolyLine;
    QAction *actionFormatLinkStylePolyParabel;
    QAction *actionFormatHideLinkUnselected;

    QAction *actionViewToggleNoteEditor;
    QAction *actionViewToggleHeadingEditor;
    QAction *actionViewToggleTreeEditor;
    QAction *actionViewToggleTaskEditor;
    QAction *actionViewToggleSlideEditor;
    QAction *actionViewToggleScriptEditor;
    QAction *actionViewToggleHistoryWindow;
    QAction *actionViewTogglePropertyEditor;
    QAction *actionViewToggleAntiAlias;
    QAction *actionViewToggleSmoothPixmapTransform;
    QAction* actionViewCenter;

    QAction* actionSettingsAutoEditNewBranch;
    QAction* actionSettingsAutoSelectNewBranch;
    QAction* actionSettingsAutoSelectText;
    QAction* actionSettingsUseFlagGroups;
    QAction* actionSettingsUseHideExport;
    QAction* actionSettingsAutosaveToggle;
    QAction* actionSettingsAutosaveTime;
    QAction* actionSettingsTaskShowParentsLevel;
    QAction* actionSettingsAutoLayoutToggle;
    QAction* actionSettingsWriteBackupFile;
    QAction* actionSettingsUseAnimation;
};


#endif

