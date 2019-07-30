#include "foldersmodel.h"

#include <QJsonArray>
#include <QJsonObject>

#include "folder.h"

#include <QDebug>

FoldersModel::FoldersModel()
{
}

FoldersModel::FoldersModel(QList<Folder> folderList)
{
    for (const Folder& folder : folderList)
        m_folderMap.insert(folder.name(), folder);
}

void FoldersModel::setFolderList(QList<Folder> folderList)
{
    beginResetModel();

    for (const Folder& folder : folderList)
        m_folderMap.insert(folder.name(), folder);

    endResetModel();
}

QList<Folder> FoldersModel::folderList() const
{
    return m_folderMap.values();
}

int FoldersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_folderMap.size();
}

QVariant FoldersModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.isValid())
        return folderList()[index.row()].name();

    return QVariant();
}

void FoldersModel::fromJson(const QJsonObject& json)
{
    if (json.contains("folders") && json["folders"].isArray()) {
        QJsonArray folders = json["folders"].toArray();
        qDebug() << "found folders" << folders.size();
        for (QJsonArray::iterator i = folders.begin(); i < folders.end(); ++i) {
            QJsonObject folder = (*i).toObject();
            Folder f(folder);
            m_folderMap.insert(f.name(), f);
        }
    }
}
QJsonObject FoldersModel::toJson() const
{
    QJsonObject json;

    QJsonArray foldersArray;
    for (const Folder &folder : m_folderMap.values())
        foldersArray.append(folder.toJson());

    json["folders"] = foldersArray;

    return json;
}
