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
#include <QProcess>
#include "SkillFileModel.h"
#include "SupportedLanguagesModel.h"
#include "GlobalSettings.h"
#include "LibreTranslateProvider.h"
#include "GTranslateProvider.h"
#include "TranslatedStringsModel.h"

class GlobalSettings;
class SkillFileModel;
class SupportedLanguagesModel;
class LibreTranslateProvider;
class GTranslateProvider;
class TranslatedStringsModel;
class LinguistikaProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString skillPath READ skillPath WRITE setSkillPath NOTIFY skillPathChanged)
    Q_PROPERTY(QString skillId READ skillId WRITE setSkillId NOTIFY skillIdChanged)
    Q_PROPERTY(SkillFileModel *skillFileModel READ skillFileModel CONSTANT)
    Q_PROPERTY(SupportedLanguagesModel *supportedLanguagesModel READ supportedLanguagesModel CONSTANT)
    Q_PROPERTY(TranslatedStringsModel *translatedStringsModel READ translatedStringsModel CONSTANT)
    Q_PROPERTY(QStringList targetLanguages READ targetLanguages NOTIFY targetLanguagesChanged)
    Q_PROPERTY(QString translationProvider READ translationProvider WRITE setTranslationProvider NOTIFY translationProviderChanged)
    Q_PROPERTY(int targetLanguagesCount READ targetLanguagesCount NOTIFY targetLanguagesCountChanged)

public:
    explicit LinguistikaProvider(QObject *parent = nullptr);
    ~LinguistikaProvider();

    Q_INVOKABLE QString skillPath() const;
    Q_INVOKABLE QString skillId() const;

    QString translationProvider() const;
    QStringList targetLanguages() const;
    SkillFileModel *skillFileModel() const;
    SupportedLanguagesModel *supportedLanguagesModel() const;
    TranslatedStringsModel *translatedStringsModel() const;

    void loadLanguagesFromConfig();
    void executeLupdateProcess(const QString &command, const QString &arguments, const QString &filename);

public Q_SLOTS:
    void setSkillPath(const QString &skillPath);
    void setSkillId(const QString &skillId);
    void setTargetLanguage(const QString &targetLanguageCode);
    void removeTargetLanguage(const QString &targetLanguageCode);
    void setTranslationProvider(const QString &translationProvider);
    void generateTranslations();
    void runLupdate();
    void populateLupdateFile(const QString &lupdateFile);
    void buildLreleaseModel(const QString &completedLupdateFile, const QString &languageCode);
    void restart();
    int targetLanguagesCount() const;

Q_SIGNALS:
    void skillPathChanged(const QString &skillPath);
    void skillIdChanged(const QString &skillId);
    void targetLanguagesChanged();
    void translationProviderChanged(const QString &translationProvider);
    void generateTranslationsFinished();
    void runLupdateFinished();
    void lreleaseStarted();
    void lreleaseFinished();
    void targetLanguagesCountChanged();

private:
    QStringList m_targetLanguages;
    QString m_translationProvider;
    QString m_skillPath;
    QString m_skillId;
    QList<QJsonObject> m_releaseModelData;
    QList<QJsonObject> m_lupdateModelData;
    SupportedLanguagesModel *m_supportedLanguagesModel;
    SkillFileModel *m_skillFileModel;
    GlobalSettings *m_globalSettings;
    LibreTranslateProvider *m_libreTranslateProvider;
    GTranslateProvider *m_gTranslateProvider;
    TranslatedStringsModel *m_translatedStringsModel;
};