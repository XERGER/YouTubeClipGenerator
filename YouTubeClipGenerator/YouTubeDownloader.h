#pragma once
#include <QProcess>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include "Ffmpeg.h"



class YouTubeDownloader : public QObject
{
	Q_OBJECT

public:
	YouTubeDownloader(QString const& ffmpegPath, QObject *parent = nullptr);
	~YouTubeDownloader();

	using MetaData = QJsonObject;

	[[nodiscard]] MetaData downloadMetaData(QUrl const& youtube) const;
	void download(QUrl const& youtube, QString const& output ) const;

	QString const& getPath() const;


private:
	[[nodiscard]] MetaData createModifiedJsonObject(QString const& ytDlJsonReturn) const;

	mutable QProcess consoleApp;

	const QString appDirPath = QCoreApplication::applicationDirPath();
	const QString consolePath = appDirPath + "/youtube-dl/youtube-dl.exe";
	const QString ffmpeg;

};
