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

#include "GTranslateProvider.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>

GTranslateProvider::GTranslateProvider(QObject *parent)
    : QObject(parent)
{
}

GTranslateProvider::~GTranslateProvider()
{
}

QString GTranslateProvider::translate(const QString &text, const QString &sourceLanguage, const QString &targetLanguage)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl("https://translate.googleapis.com/translate_a/single?client=gtx&sl=" + sourceLanguage + "&tl=" + targetLanguage + "&dt=t&q=" + text));
    QNetworkReply *reply = manager->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray data = reply->readAll();
    QString utf8Data = QString::fromUtf8(data);
    utf8Data.remove(0, 1);
    utf8Data.remove(utf8Data.length() - 1, 1);
    QStringList stringList = utf8Data.split(",");
    QString sanitizeText = stringList.first();
    sanitizeText.remove(0, 3);
    sanitizeText.remove(sanitizeText.length() - 1, 1);
    QString translatedText = sanitizeText;
    return translatedText;
}