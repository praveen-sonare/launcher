/*
 * Copyright (C) 2016 The Qt Company Ltd.
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

#ifndef APPLICATIONLAUNCHER_H
#define APPLICATIONLAUNCHER_H

#include <QTimer>
#include <QtCore/QObject>

#include "launcher.h"

class QTimer;

class ApplicationLauncher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool launching READ isLaunching NOTIFY launchingChanged)
    Q_PROPERTY(QString current READ current WRITE setCurrent NOTIFY currentChanged)
public:
    explicit ApplicationLauncher(const QString &afm_conn_str, QObject *parent = NULL);

    bool isLaunching() const;
    QString current() const;
    Launcher *get_launcher(void) { return m_launcher; }

signals:
    void newAppRequestsToBeVisible(int pid);
    void launchingChanged(bool launching);
    void currentChanged(const QString &current);

public slots:
    int launch(const QString &application);
    void setCurrent(const QString &current);

private:
    void setLaunching(bool launching);

private:
    bool m_launching;
    QString m_current;
    QTimer *m_timeout;
    Launcher *m_launcher;
};

#endif // APPLICATIONLAUNCHER_H
