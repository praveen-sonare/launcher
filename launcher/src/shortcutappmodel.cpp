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
    getAppQueue();
}

ShortcutAppModel::~ShortcutAppModel()
{
    delete this->d;
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

void ShortcutAppModel::changeShortcut(QString id, QString name, QString position)
{
    for(int i = 0; i < d->data.size(); i++) {
        if(id == d->data.at(i).id) {
            return;
        }
    }
    d->data.removeAt(position.toInt());
    HMI_DEBUG("launcher","parameter.id========%s", id.toStdString().c_str());
    HMI_DEBUG("launcher","parameter.name========%s", name.toStdString().c_str());
    HMI_DEBUG("launcher","parameter.position========%s", position.toStdString().c_str());

    RegisterApp temp;
    temp.id = id;
    temp.name = name;
    temp.icon = temp.icon = getIconPath(temp.id);
    if (temp.icon == "") {
        temp.isBlank = true;
    } else {
        temp.isBlank = false;
    }

    d->data.insert(position.toInt(), temp);
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
        HMI_DEBUG("launcher", "boardIconPath===%s", boardIconPath.toStdString().c_str());
        return "file://" + boardIconPath;
    } else if (QFile::exists(appIconPath)) {
        HMI_DEBUG("launcher", "appIconPath===%s", appIconPath.toStdString().c_str());
        return appIconPath.section('/', 1, -1);
    }
    return "";
}

void ShortcutAppModel::getAppQueue()
{
    QFile file("/home/0/app-data/homescreen/registeredApp.xml");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray line = file.readLine();
        QString str(line);
        if(str == "") {
            file.close();

        }
        QXmlStreamReader reader(&file);
        bool is_first = true;
        while(!reader.atEnd()) {
            if(reader.isStartElement())
            {
                if(reader.name() == "id")
                {
                    app.id = reader.readElementText();
                    app.icon = getIconPath(app.id);
                    if (app.icon == "") {
                        app.isBlank = true;
                    } else {
                        app.isBlank = false;
                    }
                } else if(reader.name() == "name") {
                    app.name = reader.readElementText();
                }
            } else if(reader.isEndElement() && reader.name() == "app") {
                if(is_first) {
                    is_first = false;
                } else {
                    d->data.append(app);
                }
            }
            reader.readNext();
        }
    } else {
        HMI_ERROR("HomeScreen", "registeredApp.xml file open failed");
    }
    file.close();
}
