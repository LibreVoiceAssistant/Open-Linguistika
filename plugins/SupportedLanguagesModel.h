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

class SupportedLanguagesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SupportedLanguagesModel(QObject *parent = nullptr);
    ~SupportedLanguagesModel();

    enum LanguageRoles {
        LanguageNameRole = Qt::UserRole + 1,
        LanguageCodeRole
    };

    Q_ENUMS(LanguageRoles)
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariantMap get(int index) const;
    int count() const;

    Q_INVOKABLE void clear();
    Q_INVOKABLE void add(const QString &name, const QString &code);
    Q_INVOKABLE void remove(int index);

private:
    QHash<int, QByteArray> m_roleNames;
    QList<QVariantMap> m_languages;
};