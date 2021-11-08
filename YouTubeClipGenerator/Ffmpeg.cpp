#include "Ffmpeg.h"

Ffmpeg::Ffmpeg(QObject* parent)
	: QObject(parent)
{
	consoleApp.setProgram(consolePath);
}

void Ffmpeg::clip(QString const& source, QString const& output, int startOffset, int duration) const
{
	QStringList arguments;

	arguments << "-ss" << QString::number(startOffset) << "-t" << QString::number(duration)
		<< "-i" << source << "-c" << "copy" << (output + ".mp4");

	qDebug() << arguments;

	consoleApp.setArguments(arguments);
	consoleApp.start();

	consoleApp.waitForFinished(-1);

	const QString standardOutput(consoleApp.readAllStandardOutput());
	const QString error(consoleApp.readAllStandardError());

	qDebug() << standardOutput;
	qDebug() << error;
}

void Ffmpeg::clip(QString const& source, QString const& output, QTime const& start, QTime const& end)  const
{
	const int secondsStart = QTime(0, 0).secsTo(start);
	const int secondsEnd = QTime(0, 0).secsTo(end);

	clip(source, output, secondsStart, secondsEnd - secondsStart);
}

QString const& Ffmpeg::getPath() const
{
	return consolePath;
}

