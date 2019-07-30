#ifndef FOLDER_H
#define FOLDER_H

#include <QList>

#include "note.h"

class Folder
{
public:
    explicit Folder(const QString& name = QString());
    explicit Folder(const QJsonObject& json);

    void setNotes(const QList<Note>& notes) { m_notes = notes; }
    void addNote(const Note& note) { m_notes.append(note); }
    void setName(const QString& name) { m_name = name; }

    QList<Note> notes() const { return m_notes; }
    Note noteAt(int index) const { return m_notes[index]; }
    QString name() const { return m_name; }

    void fromJson(const QJsonObject &json);
    QJsonObject toJson() const;

private:
    QList<Note> m_notes;
    QString m_name;
};

#endif // FOLDER_H
