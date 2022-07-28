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

#include "LinguistikaProvider.h"
#include "LibreTranslateProvider.h"
#include "GTranslateProvider.h"
#include <QProcess>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>


LinguistikaProvider::LinguistikaProvider(QObject *parent)
    : QObject(parent)
{
    m_skillFileModel = new SkillFileModel(this);
    m_supportedLanguagesModel = new SupportedLanguagesModel(this);
    m_globalSettings = new GlobalSettings(this);
    m_libreTranslateProvider = new LibreTranslateProvider(this);
    m_gTranslateProvider = new GTranslateProvider(this);
    m_translatedStringsModel = new TranslatedStringsModel(this);

    connect(m_globalSettings, &GlobalSettings::supportedLanguagesChanged, this, &LinguistikaProvider::loadLanguagesFromConfig);
    loadLanguagesFromConfig();
}

LinguistikaProvider::~LinguistikaProvider()
{
}

QString LinguistikaProvider::skillPath() const
{
    return m_skillPath;
}

QString LinguistikaProvider::skillId() const
{
    return m_skillId;
}

void LinguistikaProvider::setSkillPath(const QString &skillPath)
{
    if (m_skillPath == skillPath)
        return;

    m_skillPath = skillPath;
    m_skillFileModel->load(skillPath);
    emit skillPathChanged(skillPath);

    QStringList skillPathParts = skillPath.split("/");
    m_skillId = skillPathParts.last();
}

void LinguistikaProvider::setSkillId(const QString &skillId)
{
    if (m_skillId == skillId)
        return;

    m_skillId = skillId;
    emit skillIdChanged(skillId);
}

SkillFileModel *LinguistikaProvider::skillFileModel() const
{
    return m_skillFileModel;
}

SupportedLanguagesModel *LinguistikaProvider::supportedLanguagesModel() const
{
    return m_supportedLanguagesModel;
}

TranslatedStringsModel *LinguistikaProvider::translatedStringsModel() const
{
    return m_translatedStringsModel;
}

void LinguistikaProvider::loadLanguagesFromConfig()
{
    m_supportedLanguagesModel->clear();
    QList<QVariantMap> languages = m_globalSettings->supportedLanguages();
    for (const QVariantMap &language : languages) {
        m_supportedLanguagesModel->add(language.value("name").toString(), language.value("code").toString());
    }
}

void LinguistikaProvider::executeLupdateProcess(const QString &command, const QString &arguments, const QString &filename)
{
    QString argumentsWithoutQuotes = arguments;
    argumentsWithoutQuotes.remove(QChar('\"'));
    argumentsWithoutQuotes.remove(QChar('\''));

    QProcess *process = new QProcess(this);
    process->start(command, QStringList() << argumentsWithoutQuotes.split(' '));
    process->waitForFinished();

    populateLupdateFile(filename);
}

QStringList LinguistikaProvider::targetLanguages() const
{
    return m_targetLanguages;
}

void LinguistikaProvider::setTargetLanguage(const QString &targetLanguageCode)
{
    if (m_targetLanguages.contains(targetLanguageCode))
        return;

    m_targetLanguages.append(targetLanguageCode);
    emit targetLanguagesChanged();
} 

void LinguistikaProvider::removeTargetLanguage(const QString &targetLanguageCode)
{
    if (!m_targetLanguages.contains(targetLanguageCode))
        return;

    m_targetLanguages.removeOne(targetLanguageCode);
    emit targetLanguagesChanged();
}

QString LinguistikaProvider::translationProvider() const
{
    return m_translationProvider;
}

void LinguistikaProvider::setTranslationProvider(const QString &translationProvider)
{
    if (m_translationProvider == translationProvider)
        return;

    m_translationProvider = translationProvider;
    emit translationProviderChanged(translationProvider);
}

void LinguistikaProvider::generateTranslations()
{
    QList<QVariantMap> translatableItems = m_skillFileModel->translatableItems();
    QStringList translatableStrings;

    for (const QVariantMap &translatableItem : translatableItems) {
        translatableStrings.append(translatableItem.value("text").toString());
    }

    QList<QVariantMap> translations;
    for (const QString &translatableString : translatableStrings) {
        for (const QString &targetLanguageCode : m_targetLanguages) {
            if (m_translationProvider == "gTranslate") {
                QString translation = m_gTranslateProvider->translate(translatableString, QStringLiteral("en"), targetLanguageCode);
                QVariantMap translationMap;
                translationMap.insert("string", translatableStrings);
                translationMap.insert("translation", translation);
                translationMap.insert("targetLanguageCode", targetLanguageCode);
                translations.append(translationMap);

                QVariantMap translationWordMap;
                translationWordMap.insert("code", targetLanguageCode);
                translationWordMap.insert("translation", translation);
                m_translatedStringsModel->addString(translatableString, translationWordMap);

            } else if (m_translationProvider == "libreTranslate") {
                QString translation = m_libreTranslateProvider->translate(translatableString, QStringLiteral("en"), targetLanguageCode);
                QVariantMap translationMap;
                translationMap.insert("string", translatableStrings);
                translationMap.insert("translation", translation);
                translationMap.insert("targetLanguageCode", targetLanguageCode);
                translations.append(translationMap);

                QVariantMap translationWordMap;
                translationWordMap.insert("code", targetLanguageCode);
                translationWordMap.insert("translation", translation);
                m_translatedStringsModel->addString(translatableString, translationWordMap); 

            } else if (m_translationProvider == "manualTranslate") {
                QVariantMap translationMap;
                translationMap.insert("string", translatableStrings);
                translationMap.insert("translation", "");
                translationMap.insert("targetLanguageCode", targetLanguageCode);
                translations.append(translationMap);

                QVariantMap translationWordMap;
                translationWordMap.insert("code", targetLanguageCode);
                translationWordMap.insert("translation", "");
                m_translatedStringsModel->addString(translatableString, translationWordMap);
            }
        }
    }

    emit generateTranslationsFinished();
}

void LinguistikaProvider::runLupdate()
{
    m_releaseModelData.clear();

    emit lreleaseStarted();

    QList<QVariantMap> translatableItems = m_skillFileModel->translatableItems();
    QStringList translatableFiles;
    QStringList translatableStrings;

    for (const QVariantMap &translatableItem : translatableItems) {
        translatableFiles.append(translatableItem.value("file").toString());
        translatableStrings.append(translatableItem.value("text").toString());
    }

    QDir skillPathDir(m_skillPath);
    QDir uiDir(skillPathDir.absoluteFilePath("ui"));
    QDir translationDir(uiDir.absoluteFilePath("translations"));

    if (!translationDir.exists()) {
        translationDir.mkpath(".");
    }

    for (const QString &translatableFile : translatableFiles) {
        QString qml_filename = translatableFile.split("/").last();
        QString qml_file_location = translatableFile.split("/").first();
        QString qml_filename_without_extension = qml_filename.split(".").first();

        for (const QString &targetLanguageCode : m_targetLanguages) {
            QString lupdate_ts_filename =  qml_filename_without_extension + "_" + targetLanguageCode;
            QString lupdateCommand = "lupdate";
            QString lupdateArguments = QString("%1/%2 -ts %3/%4.ts").arg(uiDir.absolutePath()).arg(qml_filename).arg(translationDir.absolutePath()).arg(lupdate_ts_filename);
            QString lupdateFile = QStringLiteral("%1/%2.ts").arg(translationDir.absolutePath()).arg(lupdate_ts_filename); 
            
            if(m_lupdateModelData.count() > 0) {
                bool languageCodeFound = false;
                for (const QJsonObject &lupdateModelData : m_lupdateModelData) {
                    QJsonObject lupdateModelDataObject = lupdateModelData;
                    QString lupdateModelDataLanguageCode = lupdateModelDataObject.value("languageCode").toString();
                    if (lupdateModelDataLanguageCode == targetLanguageCode) {
                        languageCodeFound = true;
                        QJsonArray lupdateModelDataFiles = lupdateModelDataObject.value("files").toArray();
                        lupdateModelDataFiles.append(lupdateFile);
                        lupdateModelDataObject.insert("files", lupdateModelDataFiles);
                        m_lupdateModelData.replace(m_lupdateModelData.indexOf(lupdateModelData), lupdateModelDataObject);
                    }
                }
                if (!languageCodeFound) {
                    QJsonObject newLupdateModelDataObject;
                    newLupdateModelDataObject.insert("languageCode", targetLanguageCode);
                    QJsonArray newLupdateModelDataFiles;
                    newLupdateModelDataFiles.append(lupdateFile);
                    newLupdateModelDataObject.insert("files", newLupdateModelDataFiles);
                    m_lupdateModelData.append(newLupdateModelDataObject);
                }
            } else {
                QJsonObject newLupdateModelDataObject;
                newLupdateModelDataObject.insert("languageCode", targetLanguageCode);
                QJsonArray newLupdateModelDataFiles;
                newLupdateModelDataFiles.append(lupdateFile);
                newLupdateModelDataObject.insert("files", newLupdateModelDataFiles);
                m_lupdateModelData.append(newLupdateModelDataObject);
            }
            
            executeLupdateProcess(lupdateCommand, lupdateArguments, lupdateFile);
        }
    }
}

void LinguistikaProvider::populateLupdateFile(const QString &lupdateFile)
{
    QDir skillPathDir(m_skillPath);
    QDir uiDir(skillPathDir.absoluteFilePath("ui"));
    QDir translationDir(uiDir.absoluteFilePath("translations"));

    QString lupdateFileCode = lupdateFile.split("_").last().split(".").first();
    QJsonArray completedTranslationData = m_translatedStringsModel->getModelData();
    QFile lupdateFileHandle(lupdateFile);
    lupdateFileHandle.open(QIODevice::ReadOnly);
    QDomDocument lupdateFileDom;
    lupdateFileDom.setContent(&lupdateFileHandle);
    lupdateFileHandle.close();

    QDomNodeList messageElements = lupdateFileDom.elementsByTagName("message");
    for (int i = 0; i < messageElements.count(); i++) {
        QDomElement messageElement = messageElements.at(i).toElement();
        QDomElement sourceElement = messageElement.firstChildElement("source");
        QString source = sourceElement.text();
        QDomElement translationElement = messageElement.firstChildElement("translation");
        QString translation = translationElement.text();
        QString languageFileCode = messageElement.attribute("language");
        for (const QJsonValue &completedTranslation : completedTranslationData) {
            QJsonObject completedTranslationObject = completedTranslation.toObject();
            QString completedTranslationSource = completedTranslationObject.value("source").toString();
            QJsonArray translations = completedTranslationObject.value("translation").toArray();
            if (source == completedTranslationSource) {
                for (const QJsonValue &translationValue : translations) {
                    QJsonObject translationObject = translationValue.toObject();
                    QString translationCode = translationObject.value("code").toString();
                    QString translationString = translationObject.value("translatedString").toString();
                    if (translationCode == lupdateFileCode) {
                        QDomElement newTranslationElement = lupdateFileDom.createElement("translation");
                        QDomText newTranslationText = lupdateFileDom.createTextNode(translationString);
                        newTranslationElement.appendChild(newTranslationText);
                        messageElement.replaceChild(newTranslationElement, translationElement);
                    }
                }
            }
        }
    }

    lupdateFileHandle.open(QIODevice::WriteOnly);
    QTextStream lupdateFileStream(&lupdateFileHandle);
    lupdateFileDom.save(lupdateFileStream, 4);
    lupdateFileHandle.close();

    buildLreleaseModel(lupdateFile, lupdateFileCode);
}

void LinguistikaProvider::buildLreleaseModel(const QString &completedLupdateFile, const QString &languageCode)
{
    if(m_releaseModelData.count() > 0) {
        bool languageCodeFound = false;
        for (const QJsonObject &releaseModelData : m_releaseModelData) {
            QJsonObject releaseModelDataObject = releaseModelData;
            QString releaseModelDataLanguageCode = releaseModelDataObject.value("languageCode").toString();
            if (releaseModelDataLanguageCode == languageCode) {
                languageCodeFound = true;
                QJsonArray releaseModelDataFiles = releaseModelDataObject.value("files").toArray();
                releaseModelDataFiles.append(completedLupdateFile);
                releaseModelDataObject.insert("files", releaseModelDataFiles);
                m_releaseModelData.replace(m_releaseModelData.indexOf(releaseModelData), releaseModelDataObject);
            }
        }
        if (!languageCodeFound) {
            QJsonObject newReleaseModelDataObject;
            newReleaseModelDataObject.insert("languageCode", languageCode);
            QJsonArray newReleaseModelDataFiles;
            newReleaseModelDataFiles.append(completedLupdateFile);
            newReleaseModelDataObject.insert("files", newReleaseModelDataFiles);
            m_releaseModelData.append(newReleaseModelDataObject);
        }
    } else {
        QJsonObject newReleaseModelDataObject;
        newReleaseModelDataObject.insert("languageCode", languageCode);
        QJsonArray newReleaseModelDataFiles;
        newReleaseModelDataFiles.append(completedLupdateFile);
        newReleaseModelDataObject.insert("files", newReleaseModelDataFiles);
        m_releaseModelData.append(newReleaseModelDataObject);
    }

    if (m_releaseModelData.count() == m_lupdateModelData.count()) {
        QString lreleaseCommand = "lrelease";
        QString lreleaseArguments;
        for (const QJsonObject &releaseModelData : m_releaseModelData) {
            QJsonObject releaseModelDataObject = releaseModelData;
            QString releaseModelDataLanguageCode = releaseModelDataObject.value("languageCode").toString();
            if (releaseModelDataLanguageCode == languageCode) {
                QJsonArray releaseModelDataFiles = releaseModelDataObject.value("files").toArray();
                for (const QJsonValue &releaseModelDataFile : releaseModelDataFiles) {
                    QString releaseModelDataFileString = releaseModelDataFile.toString();
                    lreleaseArguments.append(QStringLiteral(" %1").arg(releaseModelDataFileString));
                }
            }
        }

        int numberOfFiles_for_languageCode_releaseModelData = 0;
        for (const QJsonObject &releaseModelData : m_releaseModelData) {
            QJsonObject releaseModelDataObject = releaseModelData;
            QString releaseModelDataLanguageCode = releaseModelDataObject.value("languageCode").toString();
            if (releaseModelDataLanguageCode == languageCode) {
                QJsonArray releaseModelDataFiles = releaseModelDataObject.value("files").toArray();
                numberOfFiles_for_languageCode_releaseModelData = releaseModelDataFiles.count();
            }
        }

        int numberOfFiles_for_languageCode_lupdateModelData = 0;
        for (const QJsonObject &lupdateModelData : m_lupdateModelData) {
            QJsonObject lupdateModelDataObject = lupdateModelData;
            QString lupdateModelDataLanguageCode = lupdateModelDataObject.value("languageCode").toString();
            if (lupdateModelDataLanguageCode == languageCode) {
                QJsonArray lupdateModelDataFiles = lupdateModelDataObject.value("files").toArray();
                numberOfFiles_for_languageCode_lupdateModelData = lupdateModelDataFiles.count();
            }
        }

        if(numberOfFiles_for_languageCode_releaseModelData == numberOfFiles_for_languageCode_lupdateModelData) {
            QString lreleaseOutputFile = m_skillId + "_" + languageCode + ".qm";
            QDir skillPathDir(m_skillPath);
            QDir uiDir(skillPathDir.absoluteFilePath("ui"));
            QDir translationDir(uiDir.absoluteFilePath("translations"));

            QString lreleaseOutputFilePath = translationDir.absolutePath() + "/" + lreleaseOutputFile;
            QString lreleaseCommandLineArgs = QStringLiteral("%1 -qm %2").arg(lreleaseArguments, lreleaseOutputFilePath);

            QProcess lreleaseProcess;
            lreleaseProcess.setProcessChannelMode(QProcess::MergedChannels);
            lreleaseProcess.setWorkingDirectory(translationDir.absolutePath());
            QString shlreleasecCommand = QStringLiteral("sh");
            QStringList shlreleasecCommandArgs;
            shlreleasecCommandArgs << "-c" << lreleaseCommand + " " + lreleaseCommandLineArgs;

            lreleaseProcess.start(shlreleasecCommand, shlreleasecCommandArgs);
            lreleaseProcess.waitForFinished();
            QString lreleaseOutput = lreleaseProcess.readAll();

            emit lreleaseFinished();
        }
    }
}

void LinguistikaProvider::restart()
{
    m_skillId = "";
    m_skillPath = "";
    m_lupdateModelData.clear();
    m_releaseModelData.clear();
    m_targetLanguages.clear();
    m_skillFileModel->clear();
}

int LinguistikaProvider::targetLanguagesCount() const
{
    return m_targetLanguages.count();
}