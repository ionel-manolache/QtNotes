#include "note.h"

#include <QDebug>

static const QString date_format = QStringLiteral("yyyy-MM-dd hh:mm:ss");

Note::Note() { }

Note::Note(const QString& name, const QString& contents)
    : m_name(name)
    , m_contents(contents)
    , m_created(QDateTime::currentDateTime())
    , m_modified(QDateTime::currentDateTime())
{
}

Note::Note(const QString& name, const QString& contents, const QString& createdDate, const QString& modifiedDate)
    : m_name(name)
    , m_contents(contents)
    , m_created(QDateTime::fromString(createdDate, date_format))
    , m_modified(QDateTime::fromString(modifiedDate, date_format))
{
}

Note::Note(const QJsonObject &json)
{
    fromJson(json);
}

void Note::fromJson(const QJsonObject &json)
{
    if (json.contains("name") && json["name"].isString())
        m_name = json["name"].toString();

    if (json.contains("contents") && json["contents"].isString())
        m_contents = json["contents"].toString();

    if (json.contains("created") && json["created"].isString())
        m_created = QDateTime::fromString(json["created"].toString(), date_format);

    if (json.contains("modified") && json["modified"].isString())
        m_modified = QDateTime::fromString(json["modified"].toString(), date_format);
}

QJsonObject Note::toJson() const
{
    QJsonObject json;
    json["name"] = m_name;
    json["contents"] = m_contents;
    json["created"] = m_created.toString(date_format);
    json["modified"] = m_modified.toString(date_format);
    return json;
}

bool Note::operator<(const Note& other) const
{
    if (m_modified.isValid() && other.m_modified.isValid())
        return m_modified < other.m_modified;

    if (m_created.isValid() && other.m_created.isValid())
        return m_created < other.m_created;

    if (m_created.isValid() && other.m_modified.isValid())
        return m_created < other.m_modified;

    if (m_modified.isValid() && other.m_created.isValid())
        return m_modified < other.m_created;

    return false;
}
