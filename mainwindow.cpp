#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QMenuBar>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QLayout>
#include <QObject>

#include <QSplitter>
#include <QListWidget>

#include <QFile>
#include <QTextStream>

#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>

#include <QStringListModel>

#include <QInputDialog>

bool isNullOrEmpty(const QString &string)
{
    return string.isNull() || string.isEmpty();
}

MainWindow::MainWindow(const QString& defaultNotesFile, QWidget *parent)
    : QMainWindow(parent)
    , m_notesFile(defaultNotesFile)
{
    //QMenuBar* mBar = menuBar();
    //QMenu* fileMenu = mBar->addMenu(QStringLiteral("&File"));

    //QAction* newAction = fileMenu->addAction(QStringLiteral("&New"));
    //connect(newAction, &QAction::triggered, this, &MainWindow::onNew);

    //QAction* openAction = fileMenu->addAction(QStringLiteral("&Open"));
    //connect(openAction, &QAction::triggered, this, &MainWindow::onOpen);

    //QAction* saveAsAction = fileMenu->addAction(QStringLiteral("&Save As..."));
    //connect(saveAsAction, &QAction::triggered, this, &MainWindow::onSaveAs);

    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setMargin(2);
    mainWidget->setLayout(layout);

    QSplitter* splitter = new QSplitter(mainWidget);
    mainWidget->layout()->addWidget(splitter);

    m_notesListWidget = new QListWidget(this);
    splitter->addWidget(m_notesListWidget);
    m_edit = new QTextEdit(this);
    m_edit->setWordWrapMode(QTextOption::WordWrap);
    m_edit->setTabStopDistance(40);
    splitter->addWidget(m_edit);

    splitter->setSizes(QList<int>() << 150 << 500);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setHandleWidth(4);

    if (!isNullOrEmpty(m_notesFile))
        loadFromFile(m_notesFile);
}

MainWindow::~MainWindow()
{
    if (!isNullOrEmpty(m_notesFile))
        saveToFile(m_notesFile);
}

void MainWindow::onNew()
{
    const QString newNoteTitle = QInputDialog::getText(this, QStringLiteral("Title of note"), QStringLiteral("Title of note"));
    QListWidgetItem* newItem = new QListWidgetItem();
    newItem->setText(newNoteTitle);
    m_notesListWidget->addItem(newItem);
    m_edit->clear();
}

void MainWindow::onOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Open file"), QDir::home().absolutePath());
    if (fileName.isEmpty())
        return;

    loadFromFile(fileName);
}

void MainWindow::onSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("Save file"), QDir::home().absolutePath());
    if (fileName.isEmpty())
        return;

    saveToFile(fileName);
}

void MainWindow::saveToFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    {
        xmlWriter.writeStartElement("notes");
        {
            xmlWriter.writeStartElement("note");
            {
                xmlWriter.writeAttribute("title", "NoteTitle"); //TODO get Title from left sidebar (QListWidget)
                for (const QString& line : m_edit->document()->toPlainText().split("\n")) {
                    xmlWriter.writeTextElement(QStringLiteral("line"), line);
                }
            }
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndDocument();
    file.close();
}

void MainWindow::loadFromFile(QString fileName)
{
    if (isNullOrEmpty(fileName))
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QXmlStreamReader xmlReader(&file);
    if (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("notes")) {
            xmlReader.readNextStartElement();
            if (xmlReader.name() == QStringLiteral("note")) {
                QString title = xmlReader.attributes().value("title").toString();
                // TODO: Save title of note
                QString note;
                while (xmlReader.readNextStartElement() && xmlReader.name() == QStringLiteral("line"))
                    note += xmlReader.readElementText() + QStringLiteral("\n");
                m_edit->document()->setPlainText(note);
            } else {
                qDebug() << "Should have been 'note' " << xmlReader.name();
            }
        } else {
            qDebug() << "Should have been 'notes' " << xmlReader.name();
        }
    } else {
        qDebug() << "Start element not found?";
    }
}
