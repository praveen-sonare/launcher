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
#include <qpa/qplatformnativeinterface.h>

#include <QDBusMetaType>
#include <QDBusArgument>

#include <wayland-client.h>
#include "wayland-agl-shell-desktop-client-protocol.h"

#include "applicationlauncher.h"
#include "applicationmodel.h"
#include "applicationhandler.h"
#include "shell-desktop.h"
#include "appinfo.h"

#define CONNECT_STR "unix:/run/platform/apis/ws/afm-main"

// this and the agl-shell extension should be added in some kind of a wrapper
// for easy usage
static void global_add(void *data, struct wl_registry *reg, uint32_t name,
		const char *interface, uint32_t version)
{
	struct agl_shell_desktop **shell =
		static_cast<struct agl_shell_desktop **>(data);

	if (strcmp(interface, agl_shell_desktop_interface.name) == 0) {
		*shell = static_cast<struct agl_shell_desktop *>(
				wl_registry_bind(reg, name, &agl_shell_desktop_interface, version)
				);
	}
}

static void global_remove(void *data, struct wl_registry *reg, uint32_t id)
{
	(void) data;
	(void) reg;
	(void) id;
}

static const struct wl_registry_listener registry_listener = {
	global_add,
	global_remove,
};

static struct agl_shell_desktop *
register_agl_shell_desktop(void)
{
	struct wl_display *wl;
	struct wl_registry *registry;
	struct agl_shell_desktop *shell = nullptr;

	QPlatformNativeInterface *native = qApp->platformNativeInterface();

	wl = static_cast<struct wl_display *>(native->nativeResourceForIntegration("display"));
	registry = wl_display_get_registry(wl);

	wl_registry_add_listener(registry, &registry_listener, &shell);
	// Roundtrip to get all globals advertised by the compositor
	wl_display_roundtrip(wl);
	wl_registry_destroy(registry);

	return shell;
}



int main(int argc, char *argv[])
{
    QString myname = QString("launcher");
    struct agl_shell_desktop *ashell = nullptr;
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
    ashell = register_agl_shell_desktop();

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

    if (ashell) {
	    std::shared_ptr<struct agl_shell_desktop> shell{ashell, agl_shell_desktop_destroy};
	    Shell *agl_shell = new Shell(shell, &a);
	    engine.rootContext()->setContextProperty("shell", agl_shell);
    }

    engine.load(QUrl(QStringLiteral("qrc:/Launcher.qml")));

    homescreenHandler->getRunnables();

    return a.exec();
}
