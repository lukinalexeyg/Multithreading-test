#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "ui_editelementdialog.h"

#include <QMessageBox>
#include <QRandomGenerator>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_selectedElement(nullptr)
{
    ui->setupUi(this);

    setWindowTitle(qAppName());

    ui->spinBox_threadsCount->setMaximum(WorkerManager::maxWorkersCount);
    ui->spinBox_threadsCount->setValue(WorkerManager::defaultWorkersCount);
    ui->spinBox_idleDuration->setMaximum(Worker::maxIdleDuration);
    ui->spinBox_idleDuration->setValue(Worker::defaultIdleDuration);

    setElementButtonsEnabled(false);

    m_elementList = ElementListPtr::create();
    m_elementModel = new ElementModel(this);
    m_elementDelegate = new ElementDelegate;

    ui->listView->setModel(m_elementModel);
    ui->listView->setItemDelegate(m_elementDelegate);

    connect(ui->listView, &QListView::pressed, this, &MainWindow::onListViewPressed);

    connect(m_elementList.get(), &ElementList::appended,  m_elementModel, &ElementModel::append);
    connect(m_elementList.get(), &ElementList::edited,    m_elementModel, &ElementModel::edit);
    connect(m_elementList.get(), &ElementList::removed,   m_elementModel, &ElementModel::remove);

    m_workerManager = new WorkerManager(m_elementList);
    m_workerManager->m_thread = new QThread(this);
    connect(m_workerManager->m_thread, &QThread::finished, m_workerManager, &QObject::deleteLater);
    m_workerManager->moveToThread(m_workerManager->m_thread);
    m_workerManager->m_thread->start();
    QMetaObject::invokeMethod(m_workerManager, "reset", Qt::ConnectionType::QueuedConnection);

    connect(ui->pushButton_appendElement,   &QPushButton::released, this,               &MainWindow::appendElement);
    connect(ui->pushButton_editElement,     &QPushButton::released, this,               &MainWindow::editElement);
    connect(ui->pushButton_removeElement,   &QPushButton::released, this,               &MainWindow::removeElement);
    connect(ui->pushButton_clearSelection,  &QPushButton::released, this,               &MainWindow::clearSelection);

    connect(ui->pushButton_setSettings,     &QPushButton::released, this,               &MainWindow::setSettings);
    connect(ui->pushButton_resetSettings,   &QPushButton::released, this,               &MainWindow::resetSettings);

    connect(ui->pushButton_start,           &QPushButton::released, m_workerManager,    &WorkerManager::start);
    connect(ui->pushButton_stop,            &QPushButton::released, m_workerManager,    &WorkerManager::stop);
    connect(ui->pushButton_exit,            &QPushButton::released, this,               [] { qApp->exit(); } );
}



MainWindow::~MainWindow()
{
    delete ui;
    delete m_elementDelegate;

    Worker::stopThread(m_workerManager->m_thread, 3000);
}



void MainWindow::onListViewPressed(const QModelIndex &index)
{
    const int row = index.row();
    const ElementPtr element = m_elementList->get(row);

    if (m_selectedElement == nullptr && element != nullptr)
        m_selectedElement = element;

    else if (m_selectedElement != nullptr && element != nullptr && element != m_selectedElement) {
        m_elementList->release(m_selectedElement);
        m_selectedElement = element;
    }

    else if (m_selectedElement != nullptr && element == nullptr) {
        ui->listView->setCurrentIndex(QModelIndex().siblingAtRow(-1));
        m_elementList->release(m_selectedElement);
        m_selectedElement = nullptr;
    }

    else if (m_selectedElement != nullptr && element == m_selectedElement) {
        m_elementList->release(m_selectedElement);
        ui->listView->setCurrentIndex(QModelIndex().siblingAtRow(-1));
        m_selectedElement = nullptr;
    }

    else if (m_selectedElement == nullptr && element == nullptr)
        ui->listView->setCurrentIndex(QModelIndex().siblingAtRow(-1));

    setElementButtonsEnabled(m_selectedElement != nullptr);
}



void MainWindow::setSettings()
{
    QMetaObject::invokeMethod(m_workerManager,
                              "set",
                              Qt::ConnectionType::QueuedConnection,
                              Q_ARG(int, ui->spinBox_threadsCount->value()),
                              Q_ARG(int, ui->spinBox_idleDuration->value())
                              );
}



void MainWindow::resetSettings()
{
    ui->spinBox_threadsCount->setValue(WorkerManager::defaultWorkersCount);
    ui->spinBox_idleDuration->setValue(Worker::defaultIdleDuration);
    QMetaObject::invokeMethod(m_workerManager, "reset", Qt::ConnectionType::QueuedConnection);
}



void MainWindow::appendElement()
{
    QString userText;

    if (execEditElementDialog(QStringLiteral("Adding an element"), QStringLiteral("Add"), userText) == QDialog::Accepted)
        Worker::appendElement(m_elementList, userText);
}



void MainWindow::editElement()
{
    QString userText;

    if (execEditElementDialog(QStringLiteral("Editing an element"), QStringLiteral("Edit"), userText) == QDialog::Accepted) {
        m_selectedElement->setUserText(userText);
        m_elementList->set(m_selectedElement);
    }
}



bool MainWindow::execEditElementDialog(const QString &title, const QString &okButtonText, QString &userText)
{
    QDialog *dialog = new QDialog(this, Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    Ui::EditElementDialog *editElementDialogUi = new Ui::EditElementDialog;
    editElementDialogUi->setupUi(dialog);

    dialog->setWindowTitle(title);
    if (m_selectedElement != nullptr)
        editElementDialogUi->textEdit->setText(m_selectedElement->userText());
    editElementDialogUi->buttonBox->button(QDialogButtonBox::Ok)->setText(okButtonText);
    editElementDialogUi->buttonBox->button(QDialogButtonBox::Cancel)->setText(QStringLiteral("Cancel"));

    const bool dialogResult = dialog->exec();
    userText = editElementDialogUi->textEdit->toPlainText().simplified();

    dialog->deleteLater();
    delete editElementDialogUi;

    return dialogResult;
}



void MainWindow::removeElement()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(QStringLiteral("Deleting an element"));
    messageBox.setIcon(QMessageBox::Question);
    messageBox.setText(QStringLiteral("Delete the selected element from the list?"));
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Ok);

    if (messageBox.exec() == QMessageBox::Ok) {
        m_elementList->remove(m_selectedElement);
        ui->listView->setCurrentIndex(QModelIndex().siblingAtRow(-1));
        m_selectedElement = nullptr;
        setElementButtonsEnabled(false);
    }
}



void MainWindow::clearSelection()
{
    if (m_selectedElement != nullptr) {
        ui->listView->setCurrentIndex(QModelIndex().siblingAtRow(-1));
        m_elementList->release(m_selectedElement);
        m_selectedElement = nullptr;
    }

    else
        ui->listView->setCurrentIndex(QModelIndex().siblingAtRow(-1));

    setElementButtonsEnabled(false);
}



void MainWindow::setElementButtonsEnabled(const bool enabled)
{
    ui->pushButton_editElement->setEnabled(enabled);
    ui->pushButton_removeElement->setEnabled(enabled);
    ui->pushButton_clearSelection->setEnabled(enabled);
}
