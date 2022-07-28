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

#include "TranslatedStringsModel.h"
#include <QJsonArray>

TranslatedStringsModel::TranslatedStringsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames[StringSourceRole] = "source";
    m_roleNames[StringTranslationRole] = "translation";
}

TranslatedStringsModel::~TranslatedStringsModel()
{
}

int TranslatedStringsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

QHash<int, QByteArray> TranslatedStringsModel::roleNames() const
{
    return m_roleNames;
};

void TranslatedStringsModel::addString(const QString &source, const QVariantMap &translation)
{
    beginResetModel();
    QJsonObject root = m_doc.object();
    QJsonArray array = root.value("strings").toArray();
    QJsonObject string;
    
    for (int i = 0; i < array.count(); i++) {
        QJsonObject obj = array.at(i).toObject();
        if (obj.value("source").toString() == source) {
            string = obj;
            break;
        }
    }

    if (string.isEmpty()) {
        string.insert("source", source);
        QJsonArray translations;
        QJsonObject translationObj;
        translationObj.insert("code", translation.value("code").toString());
        translationObj.insert("translatedString", translation.value("translation").toString());
        translations.append(translationObj);
        string.insert("translation", translations);
        array.append(string);
    } else {
        QJsonArray translations = string.value("translation").toArray();
        bool found = false;
        for (int i = 0; i < translations.count(); i++) {
            QJsonObject obj = translations.at(i).toObject();
            if (obj.value("code").toString() == translation.value("code").toString()) {
                obj.insert("translatedString", translation.value("translation").toString());
                translations.replace(i, obj);
                found = true;
                break;
            }
        }
        if (!found) {
            QJsonObject translationObj;
            translationObj.insert("code", translation.value("code").toString());
            translationObj.insert("translatedString", translation.value("translation").toString());
            translations.append(translationObj);
        }
        string.insert("translation", translations);
        for (int i = 0; i < array.count(); i++) {
            if (array.at(i).toObject().value("source").toString() == source) {
                array.replace(i, string);
                break;
            }
        }
    }
    root.insert("strings", array);
    m_doc.setObject(root);

    m_data = m_doc.object().value("strings").toArray();

    endResetModel();
}

QVariantMap TranslatedStringsModel::get(int index) const
{
    QVariantMap string;
    string.insert("source", m_data[index].toObject().value("source").toString());
    QJsonArray translationArray = m_data[index].toObject().value("translation").toArray();
    QVariantList translationList;
    for (int i = 0; i < translationArray.count(); i++) {
        QVariantMap translation;
        translation.insert("code", translationArray[i].toObject().value("code").toString());
        translation.insert("translation", translationArray[i].toObject().value("translation").toString());
        translationList.append(translation);
    }
    string.insert("translation", translationList);
    return string;
}

QVariant TranslatedStringsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_data.count())
        return QVariant();

    const QJsonObject &string = m_data[index.row()].toObject();
    if (role == StringSourceRole)
        return string.value("source");
    else if (role == StringTranslationRole)
        return string.value("translation");
    else
        return QVariant();
}

void TranslatedStringsModel::updateString(const QString &source, const QString &code, const QString &modifiedString)
{
    beginResetModel();
    QJsonObject root = m_doc.object();
    QJsonArray array = root.value("strings").toArray();
    for (int i = 0; i < array.count(); i++) {
        QJsonObject string = array.at(i).toObject();
        if (string.value("source").toString() == source) {
            QJsonArray translations = string.value("translation").toArray();
            for (int j = 0; j < translations.count(); j++) {
                QJsonObject translation = translations.at(j).toObject();
                if (translation.value("code").toString() == code) {
                    translation.remove("translatedString");
                    translation.insert("translatedString", modifiedString);
                    translations.replace(j, translation);
                }
            }
            string.insert("translation", translations);
            array.replace(i, string);
            break;
        }
    }
    root.insert("strings", array);
    m_doc.setObject(root);
    m_data = m_doc.object().value("strings").toArray();
    endResetModel();
}

QJsonArray TranslatedStringsModel::getModelData() const
{
    return m_data;
}