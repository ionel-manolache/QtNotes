#include "notesmodel.h"

#include <QJsonArray>
#include <QJsonObject>

#include "note.h"

#include <QDebug>

NotesModel::NotesModel()
{
}

NotesModel::NotesModel(QList<Note> noteList)
{
    for (const Note& note : noteList)
        m_notesMap.insert(note.name(), note);
}

void NotesModel::setNoteList(QList<Note> noteList)
{
    beginResetModel();

    for (const Note& note : noteList)
        m_notesMap.insert(note.name(), note);

    endResetModel();
}

QList<Note> NotesModel::noteList() const
{
    return m_notesMap.values();
}

void NotesModel::insertNote(const QString &noteName, const Note& note)
{
    m_notesMap.insert(noteName, note);
}

Note NotesModel::getNote(const QString &noteName)
{
    if (m_notesMap.contains(noteName))
        return m_notesMap[noteName];

    return Note();
}

int NotesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_notesMap.size();
}

QVariant NotesModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole && index.isValid()) {
        return noteList()[index.row()].name();
    }

    return QVariant();
}
