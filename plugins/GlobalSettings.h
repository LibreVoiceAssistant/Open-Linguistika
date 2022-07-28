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

#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QObject>
#include <QJsonDocument>
#include <QVariantMap>
#include <QFileSystemWatcher>

class GlobalSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QVariantMap> supportedLanguages READ supportedLanguages NOTIFY supportedLanguagesChanged)

public:
    explicit GlobalSettings(QObject *parent=0);

    QList<QVariantMap> supportedLanguages() const;
    void addLanguage(const QString &name, const QString &code);
    void loadSettings();

Q_SIGNALS:
    void supportedLanguagesChanged();

private:
    QJsonDocument m_settings;
    QStringList m_standardConfigPaths;
    QString m_settingsFilePath;
    QString m_settingsFileXDGPath;
    QList<QVariantMap> m_supportedLanguages;
    QFileSystemWatcher *m_fileWatcher;
};

#endif // GLOBALSETTINGS_H