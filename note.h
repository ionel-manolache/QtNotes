#ifndef NOTE_H
#define NOTE_H

#include <QString>
#include <QDateTime>

#include <QJsonObject>

class Note
{
public:
    explicit Note();
    explicit Note(const QString& name, const QString& contents);
    explicit Note(const QString& name, const QString& contents, const QString& createdDate, const QString& modifiedDate);
    explicit Note(const QJsonObject &json);

    void setName(const QString& name) { m_name = name; }
    void setContents(const QString& contents) { m_contents = contents; }
    void setCreatedDate(const QDateTime& created) { m_created = created; }
    void setModifiedDate(const QDateTime& modified) { m_modified = modified; }

    QString name() const { return m_name; }
    QString contents() const { return m_contents; }
    QDateTime createdDate() const { return m_created; }
    QDateTime modifiedDate() const { return m_modified; }

    void fromJson(const QJsonObject &json);
    QJsonObject toJson() const;

    bool operator<(const Note& other) const;

private:
    QString m_name;
    QString m_contents;
    QDateTime m_created;
    QDateTime m_modified;
};

#endif // NOTE_H
