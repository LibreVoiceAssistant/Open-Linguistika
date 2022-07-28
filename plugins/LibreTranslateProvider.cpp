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

#include "LibreTranslateProvider.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

LibreTranslateProvider::LibreTranslateProvider(QObject *parent)
    : QObject(parent)
{
}

LibreTranslateProvider::~LibreTranslateProvider()
{
}

QString LibreTranslateProvider::translate(const QString &text, const QString &sourceLanguage, const QString &targetLanguage)
{   
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl("https://libretranslate.de/translate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString textToSend = text;
    textToSend = textToSend.toLower();
    textToSend[0] = textToSend[0].toUpper();

    QNetworkReply *reply = manager->post(request, QJsonDocument(QJsonObject({
        {"q", textToSend},
        {"source", sourceLanguage},
        {"target", targetLanguage},
        {"format", "text"}
    })).toJson());

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray data = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);
    QJsonObject jsonObject = jsonDocument.object();
    QString translatedText = jsonObject["translatedText"].toString();
    reply->deleteLater();
    manager->deleteLater();

    return translatedText;
}