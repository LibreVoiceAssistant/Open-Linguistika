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

#include "TranslatableItemsModel.h"
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QFile>
#include <QDir>
#include <QDebug>

TranslatableItemsModel::TranslatableItemsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(ItemTextRole, "text");
    m_roleNames.insert(ItemFileRole, "file");
}

TranslatableItemsModel::~TranslatableItemsModel()
{
}

int TranslatableItemsModel::count() const
{
    return m_data.count();
}

int TranslatableItemsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

QHash<int, QByteArray> TranslatableItemsModel::roleNames() const
{
    return m_roleNames;
}

void TranslatableItemsModel::addFiles(const QStringList &files)
{
    beginResetModel();

    for (const QString &file : files) {
        QFile qmlFile(file);
        if (!qmlFile.open(QFile::ReadOnly)) {
            continue;
        }
        QString qml = QString::fromUtf8(qmlFile.readAll());
        qmlFile.close();
        QRegularExpression re("qsTr\\s*\\(\\s*\"(.*)\"\\s*\\)");
        QRegularExpressionMatchIterator i = re.globalMatch(qml);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QString text = match.captured(1);
            QVariantMap item;
            item.insert("text", text);
            item.insert("file", file);
            m_data.append(item);
        }
    }

    endResetModel();
}

QVariant TranslatableItemsModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= m_data.count()) {
        return QVariant();
    }

    QVariantMap translatableData = m_data[index.row()];
    if (role == ItemTextRole) {
        return translatableData["text"];
    } else if (role == ItemFileRole) {
        return translatableData["file"];
    }
    return QVariant();
}

QVariantMap TranslatableItemsModel::get(int index) const
{
    if (index >= m_data.count())
        return QVariantMap();

    return m_data.value(index);
}

QList<QVariantMap> TranslatableItemsModel::translatableItems() const
{
    return m_data;
}

void TranslatableItemsModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}