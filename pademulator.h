#ifndef PADEMULATOR_H
#define PADEMULATOR_H

#include <QMainWindow>

#include <QLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolBar>
#include <QToolBox>
#include <QPushButton>
#include <QTabWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QSettings>

class PADEmulator : public QMainWindow
{
    Q_OBJECT

public:
    PADEmulator(QWidget *parent = nullptr);
    ~PADEmulator();

private:
    /*! \brief addProgramMenu Метод формує головне меню програми.
     *  \return Метод повертає вказівник на сформоване меню.
     */
    inline QMenuBar * addProgramMenu();

    /*! \brief addProgramStatusBar Метод формує панель статусу програми.
     *  \return Метод повертає вказівник на сформовану панель статусу.
     */
    inline QStatusBar * addProgramStatusBar();

    /*! \brief addFileToolBar Метод формує панель інструментів роботи з файлами.
     *  \return Метод повертає вказівник на сформоману панель інструментів.
     */
    inline QToolBar * addFileToolBar();

    /*! \brief addControllToolBar Метод формує панель інструментів контролю передачі даних на пристрій.
     *  \return Метод повертає вказівник на сформоману панель інструментів.
     */
    inline QToolBar * addControllToolBar();

    QMetaObject::Connection conStartPause;

    /*! \brief lastDir Останній відкритий шлях до файлу.
     */
    QString lastDir;

    QStandardItemModel *tableItem;

private slots:
    /*! \brief closeProgram Метод обробки виклику закриттям програми.
     */
    void closeProgram();

    void openCSVFile();

};
#endif // PADEMULATOR_H
