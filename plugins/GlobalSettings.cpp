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

#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QStandardPaths>
#include <QFileSystemWatcher>
#include "GlobalSettings.h"

GlobalSettings::GlobalSettings(QObject *parent) 
    : QObject(parent)
{
    m_fileWatcher = new QFileSystemWatcher(this);
    m_standardConfigPaths = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    m_settingsFilePath = m_standardConfigPaths.first() + QStringLiteral("/linguistika.json");
    m_settingsFileXDGPath = m_standardConfigPaths.last() + QStringLiteral("/linguistika.json");
    m_fileWatcher->addPath(m_settingsFilePath);
    m_fileWatcher->addPath(m_settingsFileXDGPath);

    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &GlobalSettings::loadSettings);
    loadSettings();
}

QList<QVariantMap> GlobalSettings::supportedLanguages() const
{
    return m_supportedLanguages;
}

void GlobalSettings::addLanguage(const QString &name, const QString &code)
{
    QVariantMap language;
    language["name"] = name;
    language["code"] = code;
    m_supportedLanguages.append(language);

    QFile settingsFile(m_settingsFilePath);
    if (!settingsFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument settingsDocument = QJsonDocument::fromJson(settingsFile.readAll());
    settingsFile.close();
    if (settingsDocument.isNull()) {
        return;
    }
    QJsonObject settingsObject = settingsDocument.object();
    if (!settingsObject.contains("languages")) {
        settingsObject["languages"] = QJsonArray();
    }
    QJsonArray languagesArray = settingsObject["languages"].toArray();
    bool found = false;
    for (const QJsonValue &languageValue : languagesArray) {
        QJsonObject languageObject = languageValue.toObject();
        if (languageObject["code"].toString() == code) {
            found = true;
            break;
        }
    }
    if (!found) {
        QJsonObject languageObject;
        languageObject["name"] = name;
        languageObject["code"] = code;
        languagesArray.append(languageObject);
        settingsObject["languages"] = languagesArray;
        settingsDocument.setObject(settingsObject);
        if (!settingsFile.open(QIODevice::WriteOnly)) {
            return;
        }
        settingsFile.write(settingsDocument.toJson());
        settingsFile.close();
    }

    emit supportedLanguagesChanged();
}

void GlobalSettings::loadSettings()
{
   if(QFile::exists(m_settingsFilePath)) {
        QFile settingsFile(m_settingsFilePath);
        if(settingsFile.open(QIODevice::ReadOnly)) {
            m_settings = QJsonDocument::fromJson(settingsFile.readAll());
            settingsFile.close();
        }
   } else if(QFile::exists(m_settingsFileXDGPath)) {
        QFile settingsFile(m_settingsFileXDGPath);
        if(settingsFile.open(QIODevice::ReadOnly)) {
            m_settings = QJsonDocument::fromJson(settingsFile.readAll());
            settingsFile.close();
        }
   } else {
        QList<QVariantMap> languages;
        languages.append(QVariantMap({{"languageName", "German"}, {"languageCode", "de"}}));
        languages.append(QVariantMap({{"languageName", "French"}, {"languageCode", "fr"}}));
        languages.append(QVariantMap({{"languageName", "Spanish"}, {"languageCode", "es"}}));
        languages.append(QVariantMap({{"languageName", "Italian"}, {"languageCode", "it"}}));
        languages.append(QVariantMap({{"languageName", "Portuguese"}, {"languageCode", "pt"}}));
        languages.append(QVariantMap({{"languageName", "Dutch"}, {"languageCode", "nl"}}));

        QJsonObject settingsObject;
        QJsonArray languagesArray;
        for (int i = 0; i < languages.size(); i++) {
            QJsonObject languageObject;
            languageObject["name"] = languages[i]["languageName"].value<QString>();
            languageObject["code"] = languages[i]["languageCode"].value<QString>();
            languagesArray.append(languageObject);
        }
        settingsObject["languages"] = languagesArray;

        QJsonDocument settingsDocument;
        settingsDocument.setObject(settingsObject);
        QFile settingsFile(m_settingsFilePath);
        if(settingsFile.open(QIODevice::WriteOnly)) {
            settingsFile.write(settingsDocument.toJson());
            settingsFile.close();
        }
   }

   m_supportedLanguages.clear();
   QJsonArray supportedLanguages = m_settings["languages"].toArray();
   for(const QJsonValue &language : supportedLanguages) {
         QVariantMap languageMap = language.toObject().toVariantMap();
         m_supportedLanguages.append(languageMap);
   }

   emit supportedLanguagesChanged();
}