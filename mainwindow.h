#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QObject>
#include <QMap>

class QTextEdit;
class QListWidget;

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

private:
    void saveToFile(QString fileName);
    void loadFromFile(QString fileName);

private:
    QTextEdit* m_edit;
    QString m_notesFile;
    QListWidget *m_notesListWidget;

    QMap<QString, QString> m_notes;
};

#endif // MAINWINDOW_H
