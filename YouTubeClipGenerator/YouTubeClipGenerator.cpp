#include "YouTubeClipGenerator.h"

YouTubeClipGenerator::YouTubeClipGenerator(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


//	connect(ui.clipboard_button, &QPushButton::clicked,  this, &YouTubeClipGenerator::downloadYtUrlFromClipboard );
	connect(ui.clip_button, &QPushButton::clicked,		 this, &YouTubeClipGenerator::createClip);
	connect(ui.open_folder, &QPushButton::clicked,		 this, &YouTubeClipGenerator::openSaveDir );


	connect(clipboard, &QClipboard::dataChanged, this, &YouTubeClipGenerator::checkForNewYouTubeLinkAndDownloadIt);
	
	
	//
	//	QCompleter* fileEditCompleter = new QCompleter(dirContents, this);
	//	fileEditCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	//	fileEditCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
	//	ui->lineEdit->setCompleter(fileEditCompleter);
}


struct YouTubeClipGenerator::Video {
	Video(const QString const& path, const QJsonObject& metaData) : metaData(metaData), parentPath(path) {

	}

	const QJsonObject metaData;


	const QString parentPath;

	const QString title = metaData["title"].toString();
	const QString channel = metaData["channel"].toString();

	const QString titleNameForFolder = removeForbbidenFilenameSymbols(title.left(40));

	const QString folderName = removeForbbidenFilenameSymbols(channel) + "/" + titleNameForFolder + + "/";


	const QString folderPath = parentPath.arg(folderName);

	const QString jsonMetaFilename = titleNameForFolder + "-meta.json";
	const QString filename = titleNameForFolder  + ".mp4";

	const QString filePath = folderPath + filename;
	const QString metaFilePath = folderPath + jsonMetaFilename;

	QString createClipPath(QString const& clipName) const {
		return folderPath + clipName;
	}

private:
	static QString removeForbbidenFilenameSymbols(QString value) {
		 value = value.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]")));
		 return value.replace(" ", "_");
	}

};

void YouTubeClipGenerator::checkForNewYouTubeLinkAndDownloadIt()
{


	if (QUrl url = { QApplication::clipboard()->text() };
			!url.isEmpty() && url.isValid() ) {

				if (url.host() == "youtube.com")
				{
					QMetaObject::invokeMethod(this, "downloadYoutubeVideo", Qt::QueuedConnection, Q_ARG(QUrl const&, url));
				}
				else if( url.host() == "youtu.be" )
				{
					const QUrlQuery query{url};

					if (query.hasQueryItem("t"))
					{
						const auto startOffsetSeconds = query.queryItemValue("t").toInt();
						const QTime startStamp =
						[startOffsetSeconds]	{
							QTime time{0,0};
							return time.addSecs(startOffsetSeconds);
						}();

						ui.start->setTime(startStamp);
					}
					QMetaObject::invokeMethod(this, "downloadYoutubeVideo", Qt::QueuedConnection, Q_ARG(QUrl const&, url));
				}

			
		}
	
}


void YouTubeClipGenerator::openSaveDir() const
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(video()->folderPath));
}


void YouTubeClipGenerator::playFinishSound() const
{
	QSound::play(":/YouTubeClipGenerator/sound/download_finished.wav");

}

void YouTubeClipGenerator::downloadYoutubeVideo(QUrl const& url) const
{

	if (auto data = downloader.downloadMetaData(url); !data.isEmpty()) {
		video().reset(new Video(fullPath, data));
		ui.file_name_label->setText(video()->title);
		createMetaDataFileAndFolder(*video());

		downloader.download(url, video()->filePath );
		playFinishSound();
	}


}

void YouTubeClipGenerator::createMetaDataFileAndFolder(Video const& video) const
{
	createFolder(video.folderPath);
	createJsonMetaFile(video.metaFilePath, QJsonDocument{ video.metaData } );
}

void YouTubeClipGenerator::createClip( ) const
{

	ffmpeg.clip(
		video()->filePath,
		video()->createClipPath(ui.name_input->text()),
		ui.start->time(),
		ui.end->time()
	);
}

std::unique_ptr<YouTubeClipGenerator::Video>& YouTubeClipGenerator::video() const
{
	static std::unique_ptr<YouTubeClipGenerator::Video> video = nullptr;
	return video;
}

void YouTubeClipGenerator::createFolder(QString const& folderPath) const
{
	if (QDir dir; !dir.exists(folderPath) )
		dir.mkpath(folderPath);
}

void YouTubeClipGenerator::createJsonMetaFile(QString const& path, QJsonDocument const& document) const
{ 
	if (QFile file(path); file.open(QIODevice::ReadWrite) )
		file.write(document.toJson());
}


