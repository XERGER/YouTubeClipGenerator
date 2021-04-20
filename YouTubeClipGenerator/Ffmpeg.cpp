#include "Ffmpeg.h"

Ffmpeg::Ffmpeg( QObject *parent)
	: QObject(parent)
{
	
}


void Ffmpeg::clip(QString const& source, QString const& output,  int startOffset, int duration) const
{
	QStringList arguments;

	arguments << "-ss" << QString::number(startOffset) << "-i" << source    << "-t" << QString::number(duration) << (output + ".mp4");

	qDebug() << arguments;

	QProcess* consoleApp = new QProcess;
	consoleApp->setProgram(consolePath);
	consoleApp->setArguments(arguments);
	consoleApp->start();
	
	
	QObject::connect(consoleApp, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
		
		consoleApp , & QObject::deleteLater);





	//consoleApp.waitForFinished();

	//const QString standardOutput(consoleApp.readAllStandardOutput());
	//const QString error(consoleApp.readAllStandardError());
	//
	//qDebug() << standardOutput;
	//qDebug() << error;
}

void Ffmpeg::clip(QString const& source, QString const& output, QTime const& start, QTime const& end)  const
{
	const int secondsStart = QTime(0, 0).secsTo(start);
	const int secondsEnd   = QTime(0, 0).secsTo(end);

	clip(source, output, secondsStart, secondsEnd - secondsStart);
}

QString const& Ffmpeg::getPath() const
{
	return consolePath;
}

