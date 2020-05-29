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
#include <libhomescreen.hpp>
#include <string>

#include "shell-desktop.h"

using namespace std;

class HomescreenHandler : public QObject
{
    Q_OBJECT
public:
    explicit HomescreenHandler(QObject *parent = 0);
    ~HomescreenHandler();

    void init(int port, const char* token, QString myname);

    Q_INVOKABLE void tapShortcut(QString application_id, QString output_name);
    Q_INVOKABLE void getRunnables(void);

    void onRep(struct json_object* reply_contents);

    static void* myThis;
    static void onRep_static(struct json_object* reply_contents);

signals:
    void initAppList(QString data);
    void appListUpdate(QStringList info);

private:
    LibHomeScreen *mp_hs;
    Shell *aglShell;
    QString m_myname;
};

#endif // HOMESCREENHANDLER_H
