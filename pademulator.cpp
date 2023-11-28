#include "pademulator.h"

#include <QDebug>

PADEmulator::PADEmulator(QWidget *parent)
    : QMainWindow(parent)
{
    /* Налаштування головного вікна. */
    setMinimumSize(800, 600);
    setBaseSize(1024, 768);
    setWindowIcon(QIcon(":/TWL.png"));


    /* Налаштування меню програми. */
    setMenuBar(addProgramMenu());

    /* Налаштування статус бару програми. */
    setStatusBar(addProgramStatusBar());

    /* Налаштування панелей програми. */
    addToolBar(addFileToolBar());
    addToolBar(addControllToolBar());

    /* Інтерфейс програми. */
    QWidget *mainWidget = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout;


    /* Панель з попередніми налаштуваннями підключень. */
    QToolBox *dataSourceToolBox = new QToolBox;
    dataSourceToolBox->setMinimumWidth(100);
    dataSourceToolBox->setMaximumWidth(300);

    QWidget *filesToolBox = new QWidget;
    QWidget *SQLToolBox = new QWidget;
    QWidget *JSONToolBox = new QWidget;
    QWidget *emuToolBox = new QWidget;

    dataSourceToolBox->addItem(filesToolBox, QIcon(":/images/white/file_open.png"), tr("Files"));
    dataSourceToolBox->addItem(SQLToolBox, QIcon(":/images/white/lan.png"), tr("SQL"));
    dataSourceToolBox->addItem(JSONToolBox, QIcon(":/images/white/lan.png"), tr("JSON"));
    dataSourceToolBox->addItem(emuToolBox, QIcon(":/images/white/tune.png"), tr("EMU"));

    /* Панель закладок з відкритими таблицями даних. */
    QTabWidget * openedTables = new QTabWidget;
    openedTables->setTabsClosable(true);
    /* Зв'язка Сигнал/Слот для закриття вкладки. */
    connect(openedTables, &QTabWidget::tabCloseRequested,
            [openedTables](int index) {
                openedTables->removeTab(index);
            });

    for (int tab = 1; tab <= 6; ++tab) {
        QTableView *tableView = new QTableView(openedTables);
        openedTables->addTab(tableView, tr(QString("New table %1").arg(tab).toLatin1()));
        /*  */
//        tableView->setModel();
    }


    /* Організовуєм рухомий розділювач. */
    QSplitter *vertSplit = new QSplitter(Qt::Horizontal, mainWidget);

    vertSplit->addWidget(dataSourceToolBox);
    vertSplit->addWidget(openedTables);

    vertSplit->setSizes(QList<int> {200, mainWidget->width() - 200});

    /* Додаєм віджети в Layout. */
    mainLayout->addWidget(vertSplit);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

PADEmulator::~PADEmulator()
{
    /* Відключаєм коннекти. */
    disconnect(conStartPause);
}

QMenuBar * PADEmulator::addProgramMenu()
{
    QMenuBar *menu = new QMenuBar(this);

    menu->setNativeMenuBar(true);
    /* File menu */
    QMenu *fileMenu = new QMenu(tr("&File"), menu);

    fileMenu->addAction(QIcon::fromTheme("document-open", QIcon(":/images/white/file_open.png")), tr("&Open"), QKeySequence::Open, this, &PADEmulator::openCSVFile);
    fileMenu->addAction(QIcon::fromTheme("document-save", QIcon(":/images/white/save.png")), tr("&Save"), QKeySequence::Save);
    fileMenu->addAction(QIcon::fromTheme("document-save-as", QIcon(":/images/white/save_as.png")), tr("Save As..."), QKeySequence::SaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(QIcon::fromTheme("window-close", QIcon(":/images/white/exit.png")), tr("&Exit"), QKeySequence::Quit, this, &PADEmulator::closeProgram);

    menu->addMenu(fileMenu);
    menu->addAction(tr("&Connection"));

    /* Options menu */
    QMenu *optionsMenu = new QMenu(tr("&Options"), menu);
    optionsMenu->addAction(QIcon(":/images/white/language.png"), tr("&Language"));
    optionsMenu->addSeparator();
    optionsMenu->addAction(QIcon(":/images/white/settings.png"), tr("&Options"));
    menu->addMenu(optionsMenu);

    /* Help menu */
    QMenu *helpMenu = new QMenu(tr("&Help"), menu);
    helpMenu->addAction(QIcon::fromTheme("help-contents", QIcon(":/images/white/help.png")), tr("Help"));
    helpMenu->addAction(QIcon::fromTheme("help-about", QIcon(":/images/white/info.png")), tr("About"));
    menu->addMenu(helpMenu);

    return menu;
}

QStatusBar * PADEmulator::addProgramStatusBar()
{
    QStatusBar *statusBar = new QStatusBar(this);

    QLabel *statusIcon = new QLabel(tr(""), statusBar);
    QPixmap pix;
    pix.load(":/images/ok.png");
    statusIcon->setPixmap(pix);
    statusIcon->setAlignment(Qt::AlignCenter);
    statusIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QLabel *statusLabel = new QLabel(tr(""), statusIcon);
    statusLabel->setFrameShape(QFrame::Box);
    statusLabel->setFrameShadow(QFrame::Raised);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setScaledContents(true);
    statusLabel->setMinimumWidth(150);
    statusLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QLabel *adapterLabel = new QLabel(statusIcon);
    adapterLabel->setFrameShape(QFrame::Box);
    adapterLabel->setFrameShadow(QFrame::Raised);
    adapterLabel->setToolTip(tr("Adapter"));
    adapterLabel->setAlignment(Qt::AlignCenter);
    adapterLabel->setMinimumWidth(130);
    adapterLabel->setScaledContents(true);
    adapterLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    statusBar->addWidget(statusIcon);
    statusBar->addWidget(statusLabel);
    statusBar->addWidget(adapterLabel);

    return statusBar;
}

QToolBar * PADEmulator::addFileToolBar()
{
    QToolBar *fileTools = new QToolBar(this);

    QPushButton *openFileButt = new QPushButton(fileTools);
    openFileButt->setIcon(QIcon::fromTheme("document-open", QIcon(":/images/white/file_open.png")));
    connect(openFileButt, &QPushButton::pressed, this, &PADEmulator::openCSVFile);

    QPushButton *openSaveButt = new QPushButton(fileTools);
    openSaveButt->setIcon(QIcon::fromTheme("document-save", QIcon(":/images/white/save.png")));

    QPushButton *openSaveAsButt = new QPushButton(fileTools);
    openSaveAsButt->setIcon(QIcon::fromTheme("document-save-as", QIcon(":/images/white/save_as.png")));

    fileTools->addWidget(openFileButt);
    fileTools->addWidget(openSaveButt);
    fileTools->addWidget(openSaveAsButt);

    return fileTools;
}

QToolBar * PADEmulator::addControllToolBar()
{
    QToolBar *controlTools = new QToolBar(this);

    QPushButton *buttFastRewind = new QPushButton(controlTools);
    buttFastRewind->setIcon(QIcon(":/images/white/fast_rewind.png"));

    QPushButton *buttRewind = new QPushButton(controlTools);
    buttRewind->setIcon(QIcon(":/images/white/rewind.png"));

    QPushButton *buttStop = new QPushButton(controlTools);
    buttStop->setIcon(QIcon(":/images/white/stop.png"));

    QPushButton *buttPlay = new QPushButton(controlTools);
    buttPlay->setIcon(QIcon(":/images/white/play.png"));
    buttPlay->setCheckable(true);
    /* Підключення функцій кнопок. */
    conStartPause = QObject::connect(buttPlay, &QPushButton::clicked, [buttPlay](bool isCheckd) {
        if(isCheckd) { buttPlay->setIcon(QIcon(":/images/white/pause.png")); }
        else { buttPlay->setIcon(QIcon(":/images/white/play.png")); }
    });

    QPushButton *buttForward = new QPushButton(controlTools);
    buttForward->setIcon(QIcon(":/images/white/forward.png"));

    QPushButton *buttFastForward = new QPushButton(controlTools);
    buttFastForward->setIcon(QIcon(":/images/white/fast_forward.png"));

    QPushButton *buttReplay = new QPushButton(controlTools);
    buttReplay->setIcon(QIcon(":/images/white/replay.png"));
    buttReplay->setCheckable(true);

    controlTools->addWidget(buttFastRewind);
    controlTools->addWidget(buttRewind);
    controlTools->addWidget(buttStop);
    controlTools->addWidget(buttPlay);
    controlTools->addWidget(buttForward);
    controlTools->addWidget(buttFastForward);
    controlTools->addSeparator();
    controlTools->addWidget(buttReplay);


    return controlTools;
}

void PADEmulator::closeProgram()
{
    /* Додамо дії перед закриттям програми. */


    /* Закриєм програму */
    close();
}

void PADEmulator::openCSVFile()
{
    QFileDialog fileOpenDialog(this);
    fileOpenDialog.setFileMode(QFileDialog::AnyFile);
    fileOpenDialog.setDefaultSuffix("csv");
    fileOpenDialog.setNameFilters({"CSV files (*.csv)","All files (*.*)"});
    if(lastDir.isEmpty()) { lastDir = QDir::homePath(); }
    fileOpenDialog.setDirectory(lastDir);

    QStringList files;
    if(fileOpenDialog.exec()) {
        files = fileOpenDialog.selectedFiles();
        lastDir = QDir(fileOpenDialog.directory()).absolutePath();

        QFile oFile(files.first());
        if(oFile.open(QIODevice::ReadOnly)){

        }
    }
}
