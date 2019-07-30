#ifndef FOLDERSMODEL_H
#define FOLDERSMODEL_H

#include <QAbstractListModel>

#include "folder.h"

class FoldersModel : public QAbstractListModel
{
public:
    explicit FoldersModel();
    explicit FoldersModel(QList<Folder> folderList);

    void setFolderList(QList<Folder> folderList);
    QList<Folder> folderList() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fromJson(const QJsonObject& json);
    QJsonObject toJson() const;
private:
    QMap<QString, Folder> m_folderMap;
};

#endif // FOLDERSMODEL_H
