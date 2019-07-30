#ifndef NOTESMODEL_H
#define NOTESMODEL_H

#include <QAbstractListModel>

#include "note.h"

class NotesModel : public QAbstractListModel
{
public:
    explicit NotesModel();
    explicit NotesModel(QList<Note> noteList);

    void setNoteList(QList<Note> noteList);
    QList<Note> noteList() const;

    void insertNote(const QString &noteName, const Note& note);
    Note getNote(const QString &noteName);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QMap<QString, Note> m_notesMap;
};

#endif // NOTESMODEL_H
