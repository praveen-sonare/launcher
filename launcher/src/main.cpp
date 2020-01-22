/*
 * Copyright (C) 2016, 2017 Mentor Graphics Development (Deutschland) GmbH
 * Copyright (c) 2018 TOYOTA MOTOR CORPORATION
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QGuiApplication>
#include <QCommandLineParser>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQml/qqml.h>
#include <QQuickWindow>
#include <QThread>
#include <QUrlQuery>

#include <QDBusMetaType>
#include <QDBusArgument>

#include "applicationlauncher.h"
#include "applicationmodel.h"
#include "applicationhandler.h"
#include "appinfo.h"

#define CONNECT_STR "unix:/run/platform/apis/ws/afm-main"

int main(int argc, char *argv[])
{
    QString myname = QString("launcher");
    QGuiApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addPositionalArgument("port", a.translate("main", "port for binding"));
    parser.addPositionalArgument("secret", a.translate("main", "secret for binding"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(a);
    QStringList positionalArguments = parser.positionalArguments();

    int port = 1700;
    QString token = "wm";

    if (positionalArguments.length() == 2) {
        port = positionalArguments.takeFirst().toInt();
        token = positionalArguments.takeFirst();
    }

    // import C++ class to QML
    qmlRegisterType<ApplicationModel>("AppModel", 1, 0, "ApplicationModel");

    // DBus
    qDBusRegisterMetaType<AppInfo>();
    qDBusRegisterMetaType<QList<AppInfo>>();

    ApplicationLauncher *launcher =
	    new ApplicationLauncher(CONNECT_STR, &a);
    ApplicationHandler *homescreenHandler =
	    new ApplicationHandler(nullptr, launcher->get_launcher());

    QUrl bindingAddress;
    bindingAddress.setScheme(QStringLiteral("ws"));
    bindingAddress.setHost(QStringLiteral("localhost"));
    bindingAddress.setPort(port);
    bindingAddress.setPath(QStringLiteral("/api"));

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("token"), token);
    bindingAddress.setQuery(query);

    // mail.qml loading
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(QStringLiteral("homescreenHandler"), homescreenHandler);
    engine.rootContext()->setContextProperty(QStringLiteral("launcher"), launcher);
    engine.load(QUrl(QStringLiteral("qrc:/Launcher.qml")));

    homescreenHandler->getRunnables();

    return a.exec();
}
