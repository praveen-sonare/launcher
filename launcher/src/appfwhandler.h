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

#ifndef APPFWHANDLER_H
#define APPFWHANDLER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <json-c/json.h>
#include <systemd/sd-event.h>
extern "C" {
#include <afb/afb-proto-ws.h>
#include <afb/afb-ws-client.h>
}

class AppFwHandler : public QObject
{
    Q_OBJECT
public:
    AppFwHandler(const char* appname, QObject *parent = nullptr);
    AppFwHandler(AppFwHandler&) = delete;
    AppFwHandler &operator=(AppFwHandler&) = delete;
    ~AppFwHandler() = default;

    int init(void);
    int runnables(void);
    void on_pws_hangup(void);
    void registerCallback(
        /* can't be NULL */
        void (*on_reply_cb)(void *closure, void *request, struct json_object *obj, const char *error, const char *info),

        /* can be NULL */
        void (*on_event_create_cb)(void *closure, const char *event_name, int event_id) = nullptr,
        void (*on_event_remove_cb)(void *closure, const char *event_name, int event_id) = nullptr,
        void (*on_event_subscribe_cb)(void *closure, void *request, const char *event_name, int event_id) = nullptr,
        void (*on_event_unsubscribe_cb)(void *closure, void *request, const char *event_name, int event_id) = nullptr,
        void (*on_event_push_cb)(void *closure, const char *event_name, int event_id, struct json_object *data) = nullptr,
        void (*on_event_broadcast_cb)(void *closure, const char *event_name, struct json_object *data) = nullptr);

    static AppFwHandler* myself;
signals:
    void applistupdate(struct json_object * obj);

private slots:
    void connect_pws_timer_slot(void);

private:
    int try_connect_pws(void);
    void attempt_connect_pws(int count);
    int call(const char *verb, const char *object);

    QString m_api = "afm-main";
    QString m_uri;
    QString m_sessionid;
    sd_event *m_evloop = nullptr;
    afb_proto_ws *m_pws = nullptr;
    QTimer* m_time;
    int m_retry = 0;
};

#endif // APPFWHANDLER_H
