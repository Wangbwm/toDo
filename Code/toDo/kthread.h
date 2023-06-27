#ifndef KTHREAD_H
#define KTHREAD_H

#include <QThread>
#include "ktask.h"
#include "kservice.h"
class KThread : public QThread {
	Q_OBJECT
public:
	explicit KThread(QString username, QObject* parent = nullptr) : QThread(parent), m_username(username) {}

signals:
	void queryFinished(QList<KTask> tasks);

protected:
	void run() override {
		QList<KTask> result;
		KService* service = new KService(this);
		result = service->selectAllTasks(m_username);
		delete service;
		emit queryFinished(result);
	}

private:
	QString m_username;
};

#endif // KTHREAD_H