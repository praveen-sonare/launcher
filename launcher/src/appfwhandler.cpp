/*
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

#include <unistd.h>
#include <sys/types.h>
#include "appfwhandler.h"
#include "hmi-debug.h"

AppFwHandler* AppFwHandler::myself = nullptr;

// called when pws hangsup
static void _on_pws_hangup(void *closure)
{
    if(AppFwHandler::myself)
        AppFwHandler::myself->on_pws_hangup();
}

static void _on_pws_reply(void *closure, void *request, struct json_object *obj, const char *error, const char *info)
{
    HMI_DEBUG("AppFwHandler", "%s called,error=[%s], info=[%s], obj=[%s]", __FUNCTION__, error, info, json_object_to_json_string(obj));
    if(json_object_is_type(obj, json_type_object)) {
        struct json_object *obj1, *obj2, *obj3;
        json_object_object_get_ex(obj, "icon", &obj1);
        json_object_object_get_ex(obj, "name", &obj2);
        json_object_object_get_ex(obj, "id", &obj3);
        if(json_object_is_type(obj3, json_type_null))
            return;
        QString icon = json_object_get_string(obj1);
        QString name = json_object_get_string(obj2);
        QString id = json_object_get_string(obj3);
        QStringList info;
        info << icon << name << id;
        emit AppFwHandler::myself->applistupdate(info);
    }
}

static void _on_pws_event_broadcast(void *closure, const char *event_name, struct json_object *data)
{
    HMI_DEBUG("AppFwHandler", "%s called,event=%s, [%s]", __FUNCTION__, event_name, json_object_to_json_string(data));
    QStringList list = QString(event_name).split('/');
    if(list[0] == "afm-main" && list[1] == "application-list-changed") {
        struct json_object *obj1, *obj2;
        json_object_object_get_ex(data, "operation", &obj1);
        json_object_object_get_ex(data, "data", &obj2);
        QString oper = json_object_get_string(obj1);
        QString id = json_object_get_string(obj2);

        if(oper == "uninstall") {
            QStringList info;
            // icon, name, id
            info << "" << "" << id;
            emit AppFwHandler::myself->applistupdate(info);
        }
        else if (oper == "install") {
            // call state
            AppFwHandler::myself->detail(id);
        }
        else {
            HMI_DEBUG("AppFwHandler","data error");
        }
    }
}

// the callback interface for pws
static struct afb_proto_ws_client_itf pws_itf = {
    .on_reply = _on_pws_reply,
    .on_event_create = nullptr,
    .on_event_remove = nullptr,
    .on_event_subscribe = nullptr,
    .on_event_unsubscribe = nullptr,
    .on_event_push = nullptr,
    .on_event_broadcast = _on_pws_event_broadcast,
};

AppFwHandler::AppFwHandler(const char* appname, QObject *parent) : QObject(parent)
{
    myself = this;
    int uid = getuid();
    QString _uid;
    if(uid == 0)
        _uid = QString('0');
    else
        _uid = QString(uid);

    m_sessionid = _uid + QString(appname);
    m_uri = "unix:/run/user/" + _uid + "/apis/ws/afm-main";
    HMI_NOTICE("AppFwHandler","m_uri=%s, m_sessionid=%s", m_uri.toStdString().c_str(), m_sessionid.toStdString().c_str());
}

int AppFwHandler::init(void)
{
    // get default loop
    int rc = sd_event_default(&m_evloop);
    if(rc < 0)
    {
        HMI_ERROR("AppFwHandler", "can't create event loop");
        return 1;
    }

    // connect to framework
    if (!try_connect_pws()) {
        HMI_ERROR("connection to %s failed: %m\n", m_uri.toStdString().c_str());
        return 1;
    }
//    runnables();
    return 0;
}

int AppFwHandler::runnables(void)
{
    int ret = 1;
    if(call(__FUNCTION__, "{\"info\":\"test my guess\"}") < 0)
        ret = 0;
    return ret;
}

int AppFwHandler::detail(QString id)
{
    int ret = 1;
    HMI_DEBUG("AppFwHandler", "detail id is %s\n", id.toStdString().c_str());
    if(call(__FUNCTION__, id.toStdString().c_str()) < 0)
        ret = 0;
    return ret;
}

int AppFwHandler::try_connect_pws(void)
{
    m_pws = afb_ws_client_connect_api(m_evloop, m_uri.toStdString().c_str(), &pws_itf, NULL);
    if (m_pws == nullptr) {
        HMI_ERROR("AppFwHandler", "connection to %s failed!\n", m_uri.toStdString().c_str());
        return 0;
    }
    afb_proto_ws_on_hangup(m_pws, _on_pws_hangup);
    return 1;
}

void AppFwHandler::on_pws_hangup(void)
{
    struct afb_proto_ws *apw = m_pws;
    m_pws = nullptr;
    afb_proto_ws_unref(apw);
    attempt_connect_pws(10);
}

void AppFwHandler::attempt_connect_pws(int count)
{
    if(m_time != nullptr) {
        HMI_NOTICE("AppFwHandler", "attempt_connect_pws retrying!\n");
        return;
    }
    if(count > 0)
        m_retry = count;
    else
        return;

    m_time = new QTimer(this);
    connect(m_time, SIGNAL(timeout()), this, SLOT(connect_pws_timer_slot()));
    m_time->start(5000);
}

void AppFwHandler::connect_pws_timer_slot(void)
{
    --m_retry;
    int ret = try_connect_pws();
    if(ret) {
        m_retry = 0;
        disconnect(m_time, 0, 0, 0);
        delete m_time;
        m_time = nullptr;
    }
    else {
        if(m_retry > 0)
            m_time->start(5000);
    }
}

int AppFwHandler::call(const char *verb, const char *object)
{
    static int num = 0;
    if(verb == nullptr) {
        HMI_NOTICE("AppFwHandler", "parameter is null!\n");
        return 0;
    }
    num++;

    QString key = QString(num) + ':' + QString(verb);
    enum json_tokener_error jerr;
    struct json_object *obj = json_tokener_parse_verbose(object, &jerr);
    if (jerr != json_tokener_success)
        obj = json_object_new_string(object);

    int rc = afb_proto_ws_client_call(m_pws, verb, obj, m_sessionid.toStdString().c_str(), key.toLatin1().data(), NULL);
    json_object_put(obj);
    if (rc < 0) {
        HMI_ERROR("AppFwHandler", "calling %s(%s) failed!\n", verb, object);
    }

    return rc;
}
