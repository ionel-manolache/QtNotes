#include "mainwindow.h"

#include <QAction>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QListView>
#include <QMenuBar>
#include <QSplitter>
#include <QTextEdit>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDateTime>

#include <QAbstractItemModel>

#include <QDebug>

#include "notesmodel.h"

bool isNullOrEmpty(const QString &string)
{
    return string.isNull() || string.isEmpty();
}

MainWindow::MainWindow(const QString& defaultNotesFile, QWidget *parent)
    : QMainWindow(parent)
    , m_notesFile(defaultNotesFile)
{
    QMenuBar* mBar = menuBar();
    QMenu* fileMenu = mBar->addMenu(tr("&File"));

    QAction* newAction = fileMenu->addAction(tr("&New"));
    connect(newAction, &QAction::triggered, this, &MainWindow::onNew);

    //QAction* openAction = fileMenu->addAction(tr("&Open"));
    //connect(openAction, &QAction::triggered, this, &MainWindow::onOpen);

    //QAction* saveAsAction = fileMenu->addAction(tr("&Save As..."));
    //connect(saveAsAction, &QAction::triggered, this, &MainWindow::onSaveAs);

    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setMargin(2);
    mainWidget->setLayout(layout);

    QSplitter* splitter = new QSplitter(mainWidget);
    mainWidget->layout()->addWidget(splitter);


    m_foldersListView = new QListView(this);
    m_notesListView = new QListView(this);
    m_foldersModel = new FoldersModel();
    m_notesModel = new NotesModel();
    m_foldersListView->setModel(m_foldersModel);
    m_notesListView->setModel(m_notesModel);
    splitter->addWidget(m_foldersListView);
    splitter->addWidget(m_notesListView);
    m_edit = new QTextEdit(this);
    m_edit->setWordWrapMode(QTextOption::WordWrap);
    m_edit->setTabStopDistance(40);
    splitter->addWidget(m_edit);

    //connect(m_notesListView, &QListView::currentItemChanged, this, &MainWindow::onCurrentItemChanged);
    connect(m_notesListView, &QListView::clicked, this, &MainWindow::onNotesListClicked);

    splitter->setSizes(QList<int>() << 150 << 150 << 500);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 0);
    splitter->setStretchFactor(2, 1);
    splitter->setHandleWidth(4);

    if (!isNullOrEmpty(m_notesFile)) {
        qDebug() << "Is file loading here???";
        loadFromFile(m_notesFile);
        updateUIAfterLoadingFromFile();
        qDebug() << "Should be loaded now... Models should work...";
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::onNew()
{
    const QString newNoteTitle = QInputDialog::getText(this, tr("Title of note"), tr("Please choose a title for your new note"));
}

void MainWindow::onOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::home().absolutePath());
    if (fileName.isEmpty())
        return;

    loadFromFile(fileName);
    updateUIAfterLoadingFromFile();
}

void MainWindow::onSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), QDir::home().absolutePath());
    if (fileName.isEmpty())
        return;

    saveToFile(fileName);
}

void MainWindow::onNotesListClicked(const QModelIndex &clickedIndex)
{
    if (clickedIndex.row() < m_notesModel->noteList().size()) {
        if (!m_edit->document()->isEmpty()) {
            m_currentNote.setContents(m_edit->document()->toPlainText());
            QString currentName = m_currentNote.name();
            m_notesModel->insertNote(currentName, m_currentNote);
        }

        m_currentNote = m_notesModel->noteList()[clickedIndex.row()];
        m_edit->document()->setPlainText(m_currentNote.contents());
    }
}

void MainWindow::saveToFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QJsonDocument jsonDocument(m_foldersModel->toJson());
    file.write(jsonDocument.toJson());
    file.close();
}

void MainWindow::loadFromFile(QString fileName)
{
    if (isNullOrEmpty(fileName))
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray fileContents = file.readAll();

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileContents, &error);
    if (error.error != QJsonParseError::NoError)
        qDebug() << error.errorString();

    m_foldersModel->fromJson(jsonDocument.object());
    m_notesModel->setNoteList(m_foldersModel->folderList().first().notes());
}

void MainWindow::updateUIAfterLoadingFromFile()
{
    m_currentNote = m_foldersModel->folderList().first().notes().first();
    m_edit->document()->setPlainText(m_currentNote.contents());
}
