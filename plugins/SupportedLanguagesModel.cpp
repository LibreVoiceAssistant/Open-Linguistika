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

#include "SupportedLanguagesModel.h"

SupportedLanguagesModel::SupportedLanguagesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(LanguageNameRole, "languageName");
    m_roleNames.insert(LanguageCodeRole, "languageCode");
}

SupportedLanguagesModel::~SupportedLanguagesModel()
{
}

int SupportedLanguagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_languages.count();
}

QVariant SupportedLanguagesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_languages.count())
        return QVariant();

    const QVariantMap &language = m_languages.at(index.row());
    if (role == LanguageNameRole)
        return language.value("languageName");
    else if (role == LanguageCodeRole)
        return language.value("languageCode");
    else
        return QVariant();
}

QHash<int, QByteArray> SupportedLanguagesModel::roleNames() const
{
    return m_roleNames;
}

QVariantMap SupportedLanguagesModel::get(int index) const
{
    if (index < 0 || index >= m_languages.count())
        return QVariantMap();

    return m_languages.at(index);
}

int SupportedLanguagesModel::count() const
{
    return m_languages.count();
}

void SupportedLanguagesModel::add(const QString &name, const QString &code)
{
    QVariantMap language;
    language.insert("languageName", name);
    language.insert("languageCode", code);
    m_languages.append(language);

    emit dataChanged(index(m_languages.count() - 1), index(m_languages.count() - 1));
}

void SupportedLanguagesModel::remove(int index)
{
    if (index < 0 || index >= m_languages.count())
        return;
    beginRemoveRows(QModelIndex(), index, index);
    m_languages.removeAt(index);
    endRemoveRows();
}

void SupportedLanguagesModel::clear()
{
    m_languages.clear();
    emit dataChanged(index(0), index(m_languages.count() - 1));
}