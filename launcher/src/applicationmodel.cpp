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

#include "applicationmodel.h"
#include "appinfo.h"

#include "hmi-debug.h"

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

#include "afm_user_daemon_proxy.h"

extern org::AGL::afm::user *afm_user_daemon_proxy;

class ApplicationModel::Private
{
public:
    Private();
    void loadAppInfo();

    QList<AppInfo> data, data_temp;
};

namespace {
    QString get_icon_name(QJsonObject const &i)
    {
        QString icon = i["name"].toString().toLower();

        if ( !QFile::exists(QString(":/images/%1_active.png").arg(icon)) ||
             !QFile::exists(QString(":/images/%1_inactive.png").arg(icon)) )
        {
            icon = "blank";
        }
        return icon;
    }
}

ApplicationModel::Private::Private()
{
    loadAppInfo();
    int times = 22 / this->data_temp.size();
    if(22 % this->data_temp.size() > 0) {
        ++times;
    }
    while(times > 0) {
        for(auto i = 0; i < this->data_temp.size(); ++i) {
            this->data.append(this->data_temp.at(i));
        }
        --times;
    }
}

void ApplicationModel::Private::loadAppInfo()
{
    QString apps = afm_user_daemon_proxy->runnables(QStringLiteral(""));
    QJsonDocument japps = QJsonDocument::fromJson(apps.toUtf8());
    int appCount = 0;
    int tachometerIndex = 0;
    for (auto const &app : japps.array()) {
        QJsonObject const &jso = app.toObject();
        auto const name = jso["name"].toString();
        auto const id = jso["id"].toString();
        auto const icon = get_icon_name(jso);

        // Hide HomeScreen icon itself
        if (name != "launcher" &&
            name != "homescreen" &&
            name != "HomeScreen" &&
            !name.contains("OnScreen", Qt::CaseInsensitive)) {
            this->data_temp.append(AppInfo(icon, name, id));
        }
        if (name == "tachometer") {
            HMI_DEBUG("launcher", "tachometer index:%d", tachometerIndex);
            tachometerIndex = appCount - 1;
        }
        appCount++;

        HMI_DEBUG("launcher","name: %s icon: %s id: %s.", name.toStdString().c_str(), icon.toStdString().c_str(), id.toStdString().c_str());
    }
    this->data_temp.swap(tachometerIndex, 4);
}

ApplicationModel::ApplicationModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new Private())
{
}

ApplicationModel::~ApplicationModel()
{
    delete this->d;
}

int ApplicationModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return this->d->data.count();
}

QVariant ApplicationModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    if (!index.isValid())
        return ret;

    switch (role) {
    case Qt::DecorationRole:
        ret = this->d->data[index.row()].iconPath();
        break;
    case Qt::DisplayRole:
        ret = this->d->data[index.row()].name();
        break;
    case Qt::UserRole:
        ret = this->d->data[index.row()].id();
        break;
    default:
        break;
    }

    return ret;
}

QHash<int, QByteArray> ApplicationModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DecorationRole] = "icon";
    roles[Qt::DisplayRole] = "name";
    roles[Qt::UserRole] = "id";
    return roles;
}

QString ApplicationModel::id(int i) const
{
    return data(index(i), Qt::UserRole).toString();
}

QString ApplicationModel::name(int i) const
{
    return data(index(i), Qt::DisplayRole).toString();
}

void ApplicationModel::move(int from, int to)
{
    QModelIndex parent;
    if (to < 0 || to > rowCount()) return;
    if (from < to) {
        if (!beginMoveRows(parent, from, from, parent, to + 1)) {
            HMI_NOTICE("launcher","from : %d, to : %d. false.", from, to);
            return;
        }
        d->data.move(from, to);
        endMoveRows();
    } else if (from > to) {
        if (!beginMoveRows(parent, from, from, parent, to)) {
            HMI_NOTICE("launcher","from : %d, to : %d. false.", from, to);
            return;
        }
        d->data.move(from, to);
        endMoveRows();
    } else {
        HMI_NOTICE("launcher","from : %d, to : %d. false.", from, to);
    }
}
