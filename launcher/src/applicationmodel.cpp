/*
 * Copyright (C) 2016 The Qt Company Ltd.
 * Copyright (C) 2016, 2017 Mentor Graphics Development (Deutschland) GmbH
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

#include "applicationmodel.h"
#include "appinfo.h"

#include "hmi-debug.h"

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <json_object.h>
#include <unistd.h>

#include "afm_user_daemon_proxy.h"
#define APPLIST_PATH "/var/local/lib/afm/applications/launcher/0.1/etc/applist.json"
extern org::AGL::afm::user *afm_user_daemon_proxy;

class ApplicationModel::Private
{
public:
    Private();

    void appendApp(QString icon, QString name, QString id);
    void addApp(QString icon, QString name, QString id);
    void removeApp(QString id);

    QList<AppInfo> data;
};

namespace {
    QString get_icon_name(QJsonObject const &i)
    {
        QString icon = i["name"].toString().toLower();

        if ( !QFile::exists(QString(":/images/%1_active.svg").arg(icon)) ||
             !QFile::exists(QString(":/images/%1_inactive.svg").arg(icon)) )
        {
            icon = "blank";
        }
        return icon;
    }
}

ApplicationModel::Private::Private()
{
}

void ApplicationModel::Private::addApp(QString icon, QString name, QString id)
{
    HMI_DEBUG("addApp","name: %s icon: %s id: %s.", name.toStdString().c_str(), icon.toStdString().c_str(), id.toStdString().c_str());
    for(int i = 0; i < this->data.size(); ++i) {
        if(this->data[i].id() == id)
            return;
    }

    QString _icon = name.toLower();
    if ( !QFile::exists(QString(":/images/%1_active.svg").arg(_icon)) ||
         !QFile::exists(QString(":/images/%1_inactive.svg").arg(_icon)) )
    {
        _icon = "blank";
    }

    int pos = 0;
    for (pos = 0; pos < this->data.size(); ++pos) {
        if (QString::compare(this->data.at(pos).name(), name, Qt::CaseInsensitive) > 0)
            break;
    }
    this->data.insert(pos, AppInfo(_icon, name, id));
}


void ApplicationModel::Private::appendApp(QString icon, QString name, QString id)
{
    HMI_DEBUG("addApp","name: %s icon: %s id: %s.", name.toStdString().c_str(), icon.toStdString().c_str(), id.toStdString().c_str());
    for(int i = 0; i < this->data.size(); ++i) {
        if(this->data[i].id() == id)
            return;
    }

    QString _icon = name.toLower();
    if ( !QFile::exists(QString(":/images/%1_active.svg").arg(_icon)) ||
         !QFile::exists(QString(":/images/%1_inactive.svg").arg(_icon)) )
    {
        _icon = "blank";
    }

    this->data.append(AppInfo(_icon, name, id));
}

void ApplicationModel::Private::removeApp(QString id)
{
    HMI_DEBUG("removeApp","id: %s.",id.toStdString().c_str());
    for (int i = 0; i < this->data.size(); ++i) {
          if (this->data.at(i).id() == id) {
              this->data.removeAt(i);
              break;
          }
    }
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

QString ApplicationModel::appid(int i) const
{
    QString id = data(index(i), Qt::UserRole).toString();
    return id.split("@")[0];
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

void ApplicationModel::updateApplist(QStringList info)
{
    QString icon = info.at(0);
    QString name = info.at(1);
    QString id = info.at(2);

    beginResetModel();
    if(icon == "") { // uninstall
        d->removeApp(id);
    }
    else {
        // new app
        d->addApp(icon, name, id);
    }
    endResetModel();
}

void ApplicationModel::initAppList(QString data)
{
    HMI_DEBUG("launcher","init application list.");
    beginResetModel();
    QFile file(APPLIST_PATH);
    QJsonDocument japps = QJsonDocument::fromJson(data.toUtf8());
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray allData = file.readAll();
        QString str(allData);
        if(str == "") {
            file.close();

        }
        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

        if(json_error.error != QJsonParseError::NoError)
        {
             HMI_ERROR("Launcher", "connection.json error");
             return;
        }
        HMI_DEBUG("initAppList1","initAppList 1");

        QJsonObject rootObj = jsonDoc.object();
        QJsonArray arrayObj = rootObj.value("applist").toArray();
        QJsonArray newAreaObj;
        HMI_DEBUG("initAppList1","initAppList 2");
        HMI_DEBUG("initAppList1","initAppList size %d",arrayObj.size());
        QJsonObject subObj1;
        subObj1 = arrayObj.at(2).toObject();
        HMI_DEBUG("initAppList1","initAppList iiid=%s",subObj1["id"].toString().toStdString().c_str());
        //check list
        for (auto const &app : japps.array()) {
            QJsonObject const &jso = app.toObject();
            auto const name = jso["name"].toString();
            auto const id = jso["id"].toString();
            bool haveNewApp = false;

            for (int i = 0; i < arrayObj.size(); ++i) {
                QJsonObject subObj = arrayObj.at(i).toObject();
                HMI_DEBUG("initAppList1","initAppList %s",subObj["id"].toString().toStdString().c_str());
                if((id == subObj["id"].toString()) && (name == subObj["name"].toString())) {
                    HMI_DEBUG("initAppList1","initAppList flag--%s",subObj["flag"].toString().toStdString().c_str());
                    subObj.remove("flag");
                    subObj.insert("flag", "1");
                    arrayObj.replace(i, subObj);
                    HMI_DEBUG("initAppList1","initAppList flag---%s",subObj["flag"].toString().toStdString().c_str());
                    haveNewApp = false;
                    break;
                } else {
                    haveNewApp = true;
                }
            }
            if(haveNewApp) {
                QJsonObject newObj;
                newObj.insert("id", id);
                newObj.insert("name", name);
                newObj.insert("flag", "1");
                newAreaObj.append(newObj);
            }
        }
        HMI_DEBUG("initAppList1","initAppList 3");

        //updatelist
        for (int i = 0; i < arrayObj.size(); ++i) {
            QJsonObject subObj = arrayObj.at(i).toObject();
            HMI_DEBUG("initAppList1", "flag==%s", subObj["flag"].toString().toStdString().c_str());
            if(subObj["flag"].toString() == "1") {
                d->appendApp(get_icon_name(subObj), subObj["name"].toString(), subObj["id"].toString());
            }
        }
        HMI_DEBUG("initAppList1","initAppList 4");

        if(newAreaObj.size() > 0) {
            for(int i = 0; i < newAreaObj.size(); ++i) {
                QJsonObject subObj = newAreaObj.at(i).toObject();
                d->appendApp(get_icon_name(subObj), subObj["name"].toString(), subObj["id"].toString());
            }
        }
    } else {
        HMI_ERROR("Launcher", "read applist.json file failed");
        for (auto const &app : japps.array()) {
            QJsonObject const &jso = app.toObject();
            auto const name = jso["name"].toString();
            auto const id = jso["id"].toString();
            auto const icon = get_icon_name(jso);

            d->addApp(icon, name, id);
        }
    }
    HMI_DEBUG("initAppList1","initAppList 5");
    file.close();
    endResetModel();
    recordAppsPosition();
}
void ApplicationModel::recordAppsPosition()
{
    QFile file(APPLIST_PATH);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {

        QJsonObject rootObj;
        QJsonArray arrayObj;
        for(int i = 0; i < d->data.size(); ++i) {
            QJsonObject subObj;
            subObj.insert("id", d->data.at(i).id());
            subObj.insert("name", d->data.at(i).name());
            subObj.insert("flag", "0");
            arrayObj.append(subObj);
        }
        rootObj.insert("applist", arrayObj);
        QJsonDocument jsonDoc;
        jsonDoc.setObject(rootObj);
        file.write(jsonDoc.toJson());

    } else {
        HMI_ERROR("Launcher", "write to applist.json file failed");
    }
    file.flush();
    fsync(file.handle());
    file.close();
}
