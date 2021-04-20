#pragma once

#include <QObject>
#include <QProcess>
#include <QCoreApplication>
#include <QDebug>
#include <QTime>

class Ffmpeg : public QObject
{
	Q_OBJECT

public:
	Ffmpeg( QObject *parent = nullptr);

	void clip(QString const& source, QString const& output, QTime const& start, QTime const& end) const;

	QString const& getPath() const;

private:

	void clip(QString const& source, QString const& output, int startOffset, int duration) const;



	const QString appDirPath = QCoreApplication::applicationDirPath();
	const QString consolePath = appDirPath + "/ffmpeg/ffmpeg.exe";


};
