#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QObject>
#include <QMap>

#include "note.h"

class QTextEdit;
class QListView;

class FoldersModel;
class NotesModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QString &defaultNotesFile, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNew();
    void onOpen();
    void onSaveAs();

    //void onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

private:
    void onNotesListClicked(const QModelIndex &clickedIndex);
    void saveToFile(QString fileName);
    void loadFromFile(QString fileName);

    void updateUIAfterLoadingFromFile();

private:
    QTextEdit* m_edit;
    QString m_notesFile;

    QListView *m_foldersListView;
    QListView *m_notesListView;
    FoldersModel *m_foldersModel;
    NotesModel *m_notesModel;
    Note m_currentNote;
};

#endif // MAINWINDOW_H
