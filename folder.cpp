#include "folder.h"

#include <QJsonArray>
#include <QJsonObject>

#include <QDebug>

Folder::Folder(const QString& name)
    : m_name(name)
{
}

Folder::Folder(const QJsonObject& json)
{
    fromJson(json);
}

void Folder::fromJson(const QJsonObject &json)
{
    if (json.contains("name") && json["name"].isString())
        m_name = json["name"].toString();

    if (json.contains("notes") && json["notes"].isArray()) {
        QJsonArray notesArray = json["notes"].toArray();
        m_notes.clear();
        m_notes.reserve(notesArray.size());
        for (int i = 0; i < notesArray.size(); ++i) {
            QJsonObject noteObject = notesArray[i].toObject();
            Note note(noteObject);
            m_notes.append(note);
        }
    }
}

QJsonObject Folder::toJson() const
{
    QJsonObject json;
    json["name"] = m_name;

    QJsonArray notesArray;
    for (const Note& note : m_notes)
        notesArray.append(note.toJson());

    json["notes"] = notesArray;

    return json;
}
