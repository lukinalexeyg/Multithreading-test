#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "elementlist.h"
#include "elementmodel.h"
#include "elementdelegate.h"
#include "workermanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ElementList *m_elementList;
    ElementModel *m_elementModel;
    ElementDelegate *m_elementDelegate;
    WorkerManager *m_workerManager;
    Element *m_selectedElement;

private:
    void onListViewPressed(const QModelIndex &index);

    void setSettings();
    void resetSettings();

    void appendElement();
    void editElement();
    bool execEditElementDialog(const QString &title, const QString &okButtonText, QString &userText);
    void removeElement();
    void clearSelection();

    void setElementButtonsEnabled(bool enabled);
};

#endif // MAINWINDOW_H
