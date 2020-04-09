#include <QFileInfo>
#include <functional>

#include "applicationhandler.h"


ApplicationHandler::ApplicationHandler(QObject *parent, Launcher *launcher) :
	QObject(parent)
{
	m_launcher = launcher;
}

ApplicationHandler::~ApplicationHandler()
{
}

size_t
ApplicationHandler::getRunnables(void)
{
	QString data;
	size_t runnables = 0;

	if (m_launcher == nullptr)
		return runnables;

	runnables = m_launcher->get_list_runnables(data);
	emit initAppList(data);

	return runnables;
}

int
ApplicationHandler::launch(const QString &app_id)
{
	if (m_launcher) {
		return m_launcher->start(app_id);
	}

	return -1;
}
