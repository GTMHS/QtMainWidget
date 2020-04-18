#include "Config.h"
#include <QtCore/QtCore>
#include <QDebug>

Config::Config()
{	
	m_qstrFileName = QCoreApplication::applicationDirPath() + "/Config/conf.ini";
	//if (qstrfilename.isEmpty())
	//{
	//	m_qstrFileName = QCoreApplication::applicationDirPath() + "/Src/conf.ini";
	//}
	//else
	//{
	//	m_qstrFileName = qstrfilename;
	//}

	m_psetting = new QSettings(m_qstrFileName, QSettings::IniFormat);
	qDebug() << m_qstrFileName;
}
Config::~Config()
{
	delete m_psetting;
	m_psetting = 0;
}
void Config::Set(QString qstrnodename, QString qstrkeyname, QVariant qvarvalue)
{
	m_psetting->setIniCodec("UTF8");
	m_psetting->setValue(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname), qvarvalue);
}

QVariant Config::Get(QString qstrnodename, QString qstrkeyname)
{
	QVariant qvar = m_psetting->value(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname));
	return qvar;
}
