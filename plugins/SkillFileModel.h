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

#pragma once

#include <QAbstractListModel>
#include <QStringList>
#include <QObject>
#include <QDir>
#include <QFile>
#include "TranslatableItemsModel.h"

class TranslatableItemsModel;
class SkillFileModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(TranslatableItemsModel *translatableItemsModel READ translatableItemsModel CONSTANT)

public:
    SkillFileModel(QObject *parent = nullptr);
    ~SkillFileModel();

    enum SkillFileRoles {
        SkillFileNameRole = Qt::UserRole + 1,
        SkillFilePathRole,
        SkillFileIdRole
    };

    Q_ENUM(SkillFileRoles)

    int count() const;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariantMap get(int index) const;
    void load(const QString &path);
    
    QList<QVariantMap> translatableItems() const;

    TranslatableItemsModel *translatableItemsModel() const;
    void clear();

private:
    QHash<int, QByteArray> m_roleNames;
    QList<QVariantMap> m_data;

    TranslatableItemsModel *m_translatableItemsModel;
};