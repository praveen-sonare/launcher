/*
 * Copyright (c) 2017 TOYOTA MOTOR CORPORATION
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

#include "shortcutappmodel.h"
#include "hmi-debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#define SHORTCUTKEY_PATH "/var/local/lib/afm/applications/homescreen/0.1/etc/registeredApp.json"

class ShortcutAppModel::Private
{
public:
    Private();

    QList<RegisterApp> data;
};

ShortcutAppModel::Private::Private()
{
}


ShortcutAppModel::ShortcutAppModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new Private())
{
    init();
//    getAppQueue();
}

ShortcutAppModel::~ShortcutAppModel()
{
    delete this->d;
}
void ShortcutAppModel::init()
{
    RegisterApp temp;
    for(int i = 0; i < 3; i++) {
        temp.id = "video@0.1";
        temp.name = "video";
        temp.icon = getIconPath(temp.id);
        if (temp.icon == "") {
            temp.isBlank = true;
        } else {
            temp.isBlank = false;
        }
        d->data.append(temp);
    }
}

int ShortcutAppModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return this->d->data.count();
}

QVariant ShortcutAppModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    if (!index.isValid())
        return ret;

    switch (role) {
    case Qt::DecorationRole:
        ret = this->d->data[index.row()].icon;
        break;
    case Qt::DisplayRole:
        ret = this->d->data[index.row()].name;
        break;
    case Qt::UserRole:
        ret = this->d->data[index.row()].id;
        break;
    default:
        break;
    }

    return ret;
}

QHash<int, QByteArray> ShortcutAppModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DecorationRole] = "icon";
    roles[Qt::DisplayRole] = "name";
    roles[Qt::UserRole] = "id";
    return roles;
}

void ShortcutAppModel::shortcutUpdate(QStringList shortcut_list)
{
    HMI_DEBUG("Launcher", "ShortcutAppModel::shortcutUpdate id1=%s", shortcut_list.at(1).toStdString().c_str());
    RegisterApp temp;
    for(int i = 0; i < d->data.size(); i++) {
        temp.id = shortcut_list.at(2 * i);
        temp.name = shortcut_list.at(2 * i + 1);
        temp.icon = getIconPath(temp.id);
        if (temp.icon == "") {
            temp.isBlank = true;
        } else {
            temp.isBlank = false;
        }
        d->data.replace(i, temp);
    }
    emit updateShortcut();
}

QString ShortcutAppModel::getId(int index) const
{
    return d->data.at(index).id;
}

QString ShortcutAppModel::getName(int index) const
{
    return d->data.at(index).name;
}

QString ShortcutAppModel::getIcon(int index) const
{
    return d->data.at(index).icon;
}

bool ShortcutAppModel::isBlank(int index) const
{
    return d->data.at(index).isBlank;
}

QString ShortcutAppModel::getIconPath(QString id)
{
    QString name = id.section('@', 0, 0);
    QString version = id.section('@', 1, 1);
    QString boardIconPath = "/var/local/lib/afm/applications/" + name + "/" + version + "/icon.svg";
    QString appIconPath = ":/images/TopShortcut/" + name + ".svg";
    if (QFile::exists(boardIconPath)) {
        return "file://" + boardIconPath;
    } else if (QFile::exists(appIconPath)) {
        return appIconPath.section('/', 1, -1);
    }
    return "";
}

void ShortcutAppModel::setAppQueuePoint(QString id, QString name)
{
    app.id = id;
    app.icon = getIconPath(app.id);
    if (app.icon == "") {
        app.isBlank = true;
    } else {
        app.isBlank = false;
    }
    app.name = name;
    d->data.append(app);
}
