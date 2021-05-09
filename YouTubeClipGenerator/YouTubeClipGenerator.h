#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_YouTubeClipGenerator.h"
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QSound>
#include <QUrlQuery>
#include "YouTubeDownloader.h"
#include <QMetaObject>

class YouTubeClipGenerator : public QMainWindow
{
    Q_OBJECT

public:
    YouTubeClipGenerator(QWidget *parent = Q_NULLPTR);


public slots:

    void openSaveDir() const;
    void playFinishSound() const;
    void createClip() const;
    void checkForNewYouTubeLinkAndDownloadIt();
    void downloadYoutubeVideo(QUrl const& url)  const;

private:
    struct Video;
    std::unique_ptr<Video>& video() const;


	void createFolder(QString const& folderPath) const;

    void createMetaDataFileAndFolder(Video const& video) const;
    void createJsonMetaFile(QString const& path, QJsonDocument const& document) const;


private:
    Ui::YouTubeClipGeneratorClass ui;

    const Ffmpeg ffmpeg;
    const YouTubeDownloader downloader{ ffmpeg.getPath() };

	const QString appDirPath = QCoreApplication::applicationDirPath();
	const QString fullPath = appDirPath + "/output/%1";
    const QString jsonFileName = "meta.json";
    const QClipboard* clipboard = QGuiApplication::clipboard();
};
