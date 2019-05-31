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

#include <QFileInfo>
#include "homescreenhandler.h"
#include <functional>
#include <QProcess>
#include <dirent.h>
#include <stdio.h>
#include "hmi-debug.h"
#include "afm_user_daemon_proxy.h"

extern org::AGL::afm::user *afm_user_daemon_proxy;

#define BUF_SIZE 1024
void* HomescreenHandler::myThis = 0;

HomescreenHandler::HomescreenHandler(QObject *parent) :
    QObject(parent),
    mp_qhs(NULL)
{
}

HomescreenHandler::~HomescreenHandler()
{
    if (mp_qhs != NULL) {
        delete mp_qhs;
    }
}

void HomescreenHandler::init(int port, const char *token, QLibWindowmanager *qwm, QString myname)
{
    myThis = this;
    mp_qwm = qwm;
    m_myname = myname;

    mp_qhs = new QLibHomeScreen();
    mp_qhs->init(port, token);
    mp_qhs->registerCallback(nullptr, HomescreenHandler::onRep_static);

    mp_qhs->set_event_handler(QLibHomeScreen::Event_ShowWindow,[this](json_object *object){
        HMI_DEBUG("Launcher","Surface launcher got Event_ShowWindow\n");
        mp_qwm->activateWindow(m_myname, "fullscreen");
    });

    mp_qhs->set_event_handler(QLibHomeScreen::Event_AppListChanged,[this](json_object *object){
        HMI_DEBUG("Launcher","laucher: appListChanged [%s]\n", json_object_to_json_string(object));

        struct json_object *obj_param, *obj_oper, *obj_data;
        if(json_object_object_get_ex(object, "parameter", &obj_param)
        && json_object_object_get_ex(obj_param, "operation", &obj_oper)
        && json_object_object_get_ex(obj_param, "data", &obj_data)) {
            QString oper = json_object_get_string(obj_oper);
            if(oper == "uninstall") {
                /* { "application_id": "launcher",
                 *   "type": "application-list-changed",
                 *   "parameter":{
                 *      "operation": "uninstall",
                 *      "data": "onstestapp@0.1"
                 *   }
                 * } */
                QString id = json_object_get_string(obj_data);
                QStringList info;
                // icon, name, id
                info << "" << "" << id;
                emit appListUpdate(info);
            }
            else if(oper == "install") {
                /* { "application_id": "launcher",
                 *   "type": "application-list-changed",
                 *   "parameter": {
                 *      "operation": "install",
                 *      "data": {
                 *          "description":"This is a demo onstestapp application",
                 *          "name": "onstestapp",
                 *          "shortname": "",
                 *          "id": "onstestapp@0.1",
                 *          "version": "0.1",
                 *          "author": "Qt",
                 *          "author-email": "",
                 *          "width": "",
                 *          "height": "",
                 *          "icon": "\/var\/local\/lib\/afm\/applications\/onstestapp\/0.1\/icon.svg",
                 *          "http-port": 31022
                 *      }
                 *    }
                 * } */
                struct json_object *obj_icon, *obj_name, *obj_id;
                if(json_object_object_get_ex(obj_data, "icon", &obj_icon)
                && json_object_object_get_ex(obj_data, "name", &obj_name)
                && json_object_object_get_ex(obj_data, "id", &obj_id)) {
                    QString icon = json_object_get_string(obj_icon);
                    QString name = json_object_get_string(obj_name);
                    QString id = json_object_get_string(obj_id);
                    QStringList info;
                    // icon, name, id
                    info << icon << name << id;
                    emit appListUpdate(info);
                }
            }
            else {
                HMI_ERROR("Launcher","error operation.\n");
            }
        }
    });

    mp_qhs->set_event_handler(QLibHomeScreen::Event_UpdateShortcut,[this](json_object *object){
        HMI_DEBUG("Launcher","Surface launcher got Event_UpdateShortcut\n");
        json_object *obj_p = json_object_object_get(object, "parameter");
        json_object *obj_array = json_object_object_get(obj_p, "shortcut");
        QStringList shortcut_list;
        for(int i = 0; i < 3; i++)
        {
            shortcut_list << QString(QLatin1String(json_object_get_string(json_object_object_get(json_object_array_get_idx(obj_array, i), "shortcut_id"))));
            shortcut_list << QString(QLatin1String(json_object_get_string(json_object_object_get(json_object_array_get_idx(obj_array, i), "shortcut_name"))));
        }
        HMI_DEBUG("Launcher","SEvent_UpdateShortcut id1 = %s", shortcut_list.at(2).toStdString().c_str());
        emit updateShortcutList(shortcut_list);
    });
}

void HomescreenHandler::tapShortcut(QString application_id)
{
    HMI_DEBUG("Launcher","tapShortcut %s", application_id.toStdString().c_str());
    struct json_object* j_json = json_object_new_object();
    struct json_object* value;
    value = json_object_new_string("normal.full");
    json_object_object_add(j_json, "area", value);

    mp_qhs->showWindow(application_id.toStdString().c_str(), j_json);
}

void HomescreenHandler::onRep_static(struct json_object* reply_contents)
{
    static_cast<HomescreenHandler*>(HomescreenHandler::myThis)->onRep(reply_contents);
}

void HomescreenHandler::onRep(struct json_object* reply_contents)
{
    HMI_DEBUG("launcher","HomeScreen onReply %s", json_object_to_json_string(reply_contents));
    struct json_object *obj_res, *obj_verb;
    if(json_object_object_get_ex(reply_contents, "response", &obj_res)
    && json_object_object_get_ex(obj_res, "verb", &obj_verb)
    && !strcasecmp("getRunnables", json_object_get_string(obj_verb))) {
        struct json_object *obj_data;
        if(json_object_object_get_ex(obj_res, "data", &obj_data)) {
            HMI_DEBUG("launcher","HomescreenHandler emit initAppList");
            QString data = json_object_to_json_string(obj_data);
            emit initAppList(data);
        }
    }
}

void HomescreenHandler::hideWindow(QString application_id)
{
    mp_qhs->hideWindow(application_id.section('@', 0, 0).toStdString().c_str());
}

void HomescreenHandler::registerShortcut(QString shortcut_id, QString shortcut_name, QString position)
{
//    struct json_object* j_obj = json_object_new_object();
//    struct json_object* val_id = json_object_new_string(shortcut_id.toStdString().c_str());
//    struct json_object* val_name = json_object_new_string(shortcut_name.toStdString().c_str());
//    struct json_object* val_position = json_object_new_string(position.toStdString().c_str());
//    json_object_object_add(j_obj, "shortcut_id", val_id);
//    json_object_object_add(j_obj, "shortcut_name", val_name);
//    json_object_object_add(j_obj, "position", val_position);

    mp_qhs->registerShortcut(shortcut_id, shortcut_name, position);
}

int HomescreenHandler::uninstallApplication(QString application_id)
{
    int result = -1;
    HMI_DEBUG("launcher","Application uninstall %s.", application_id.toStdString().c_str());

    result = afm_user_daemon_proxy->uninstall(application_id).value().toInt();
    HMI_DEBUG("launcher","ApplicationUninstall pid: %d.", result);

//    QProcess *process = new QProcess();
//    QString command = "/usr/bin/pkill " + application_id.section('@', 0, 0);
//    HMI_DEBUG("launcher", "command is %s", command.toStdString().c_str());
//    process->start(command);

    return result;
}

void HomescreenHandler::sendAppToMeter(QString application_id)
{
    HMI_DEBUG("Launcher","sendAppToMeter %s", application_id.toStdString().c_str());
    struct json_object* j_json = json_object_new_object();
    struct json_object* value;
    value = json_object_new_string("master.split.sub");
    json_object_object_add(j_json, "area", value);

    mp_qhs->showWindow(application_id.section('@', 0, 0).toStdString().c_str(), j_json);
}

void HomescreenHandler::getRunnables(void)
{
    mp_qhs->getRunnables();
}

void HomescreenHandler::setQuickWindow(QQuickWindow *qw)
{
    mp_qhs->setQuickWindow(qw);
}
