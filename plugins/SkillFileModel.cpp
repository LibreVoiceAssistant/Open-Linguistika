/*
 * Copyright 2022 Aditya Mehra <Aix.m@outlook.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "SkillFileModel.h"
#include <QDirIterator>

SkillFileModel::SkillFileModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_translatableItemsModel = new TranslatableItemsModel(this);
    m_roleNames[SkillFileNameRole] = "skillFileName";
    m_roleNames[SkillFilePathRole] = "skillFilePath";
    m_roleNames[SkillFileIdRole] = "skillFileId";
}

SkillFileModel::~SkillFileModel()
{
}

int SkillFileModel::count() const
{
    return m_data.count();
}

int SkillFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

TranslatableItemsModel *SkillFileModel::translatableItemsModel() const
{
    return m_translatableItemsModel;
}

QVariant SkillFileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= m_data.count()) {
        return QVariant();
    }

    QVariantMap skillFile = m_data[index.row()];
    if (role == SkillFileNameRole) {
        return skillFile["skillFileName"];
    } else if (role == SkillFilePathRole) {
        return skillFile["skillFilePath"];
    } else if (role == SkillFileIdRole) {
        return skillFile["skillFileId"];
    }
    return QVariant();
}

QHash<int, QByteArray> SkillFileModel::roleNames() const
{
    return m_roleNames;
}

QVariantMap SkillFileModel::get(int index) const
{
    if (index >= m_data.count())
        return QVariantMap();
    return m_data.value(index);
}

void SkillFileModel::load(const QString &path)
{
    m_data.clear();
    QStringList skillFilePaths;

    QDirIterator it(path + "/ui", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        if (it.fileInfo().isFile() && it.fileInfo().suffix() == "qml") {
            skillFilePaths.append(it.fileInfo().absoluteFilePath());
        }
    }

    QString sanitizedPath = path;
    if (path.endsWith('/')) {
        sanitizedPath = path.left(path.length() - 1);
    }
    QString SkillFileId = sanitizedPath.split("/").last();

    beginResetModel();

    for (const QString &skillFilePath : skillFilePaths) {
        QFileInfo fileInfo(skillFilePath);
        QVariantMap skillFile;
        skillFile["skillFileName"] = fileInfo.fileName();
        skillFile["skillFilePath"] = skillFilePath;
        skillFile["skillFileId"] = SkillFileId;
        m_data.append(skillFile);
    }

    endResetModel();
    m_translatableItemsModel->addFiles(skillFilePaths);
}

QList<QVariantMap> SkillFileModel::translatableItems() const
{
    return m_translatableItemsModel->translatableItems();
}

void SkillFileModel::clear()
{
    beginResetModel();
    m_data.clear();
    m_translatableItemsModel->clear();
    endResetModel();
}