/*
 * Copyright (c) 2017 TOYOTA MOTOR CORPORATION
 * Copyright (c) 2018,2019 TOYOTA MOTOR CORPORATION
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

#ifndef HOMESCREENHANDLER_H
#define HOMESCREENHANDLER_H

#include <QObject>
#include <QQuickWindow>
#include <qlibhomescreen.h>
#include <string>
#include <qlibwindowmanager.h>

using namespace std;

class HomescreenHandler : public QObject
{
    Q_OBJECT
public:
    explicit HomescreenHandler(QObject *parent = 0);
    ~HomescreenHandler();

    void init(int port, const char* token, QLibWindowmanager *qwm, QString myname);

    Q_INVOKABLE void tapShortcut(QString application_id);
    Q_INVOKABLE void getRunnables(void);
    Q_INVOKABLE void hideWindow(QString application_id);
    Q_INVOKABLE void registerShortcut(QString shortcut_id, QString shortcut_name, QString position);
    Q_INVOKABLE int uninstallApplication(QString application_id);
    Q_INVOKABLE void sendAppToMeter(QString application_id);
    Q_INVOKABLE void sendAppToHud(QString application_id);

    void onRep(struct json_object* reply_contents);

    static void* myThis;
    static void onRep_static(struct json_object* reply_contents);
    void setQuickWindow(QQuickWindow *qw);

signals:
    void initAppList(QString data);
    void appListUpdate(QStringList info);
    void updateShortcutList(QStringList shortcut_list);

private:
    void getLauncherArea();
    QLibHomeScreen *mp_qhs;
    QLibWindowmanager *mp_qwm;
    QString m_myname, meter_area, hud_tbt_area;
};

#endif // HOMESCREENHANDLER_H
