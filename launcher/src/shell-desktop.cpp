/*
 * Copyright (c) 2020 Collabora Ltd.
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
#include <QDebug>
#include "shell-desktop.h"
#include <qpa/qplatformnativeinterface.h>
#include <stdio.h>

static struct wl_output *
getWlOutput(QScreen *screen)
{
        QPlatformNativeInterface *native = qApp->platformNativeInterface();

        void *output = native->nativeResourceForScreen("output", screen);
        return static_cast<struct ::wl_output*>(output);
}

void Shell::activate_app(QWindow *win, const QString &app_id)
{
	QScreen *screen = nullptr;
	struct wl_output *output;

	if (!win || !win->screen()) {
		qDebug() << "window or screen, not available, using the first one available";
		screen = qApp->screens().first();
	} else {
		screen = win->screen();
	}

	if (!screen) {
		qDebug() << "screen not available?";
		return;
	}

	output = getWlOutput(screen);
	qDebug() << "will activate app: " << app_id;
	agl_shell_desktop_activate_app(this->shell.get(),
			       app_id.toStdString().c_str(), NULL, output);
}
