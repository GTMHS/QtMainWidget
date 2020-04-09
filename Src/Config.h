#pragma once
#include <QVariant>
#include <QSettings>


class Config
{
public:
	//Config(QString qstrfilename = "");
	Config();

	virtual ~Config(void);
	void Set(QString, QString, QVariant);
	QVariant Get(QString, QString);
private:
	QString m_qstrFileName;
	QSettings *m_psetting;
};

