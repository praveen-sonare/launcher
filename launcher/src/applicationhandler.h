#ifndef APPLICATIONHANDLER_H
#define APPLICATIONHANDLER_H

#include <QObject>
#include <QDebug>
#include <string>

#include "launcher.h"

using namespace std;

class ApplicationHandler : public QObject
{
	Q_OBJECT
public:
	explicit ApplicationHandler(QObject *parent = 0, Launcher *launcher = 0);
	~ApplicationHandler();

	//void init(int port, const char* token, QLibWindowmanager *qwm, QString myname);
	//Q_INVOKABLE void tapShortcut(QString application_id);
	Q_INVOKABLE size_t getRunnables(void);

	//void onRep(struct json_object* reply_contents);
	//static void* myThis;
	//static void onRep_static(struct json_object* reply_contents);

signals:
	void initAppList(QString data);
	void appListUpdate(QStringList info);

private:
	QString m_myname;
	Launcher *m_launcher;
};

#endif // APPLICATIONHANDLER_H
