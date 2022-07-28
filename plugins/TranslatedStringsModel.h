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

#include <QObject>
#include <QAbstractListModel>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class TranslatedStringsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum StringRoles {
        StringSourceRole = Qt::UserRole + 1,
        StringTranslationRole,
    };

    Q_ENUMS(StringRoles);

    TranslatedStringsModel(QObject *parent = nullptr);
    ~TranslatedStringsModel();

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addString(const QString &source, const QVariantMap &translation);

    Q_INVOKABLE QVariantMap get(int index) const;
    Q_INVOKABLE void updateString(const QString &source, const QString &code, const QString &modifiedString);

    QJsonArray getModelData() const;

private:
    QHash<int, QByteArray> m_roleNames;
    QJsonArray m_data;
    QJsonDocument m_doc;
};