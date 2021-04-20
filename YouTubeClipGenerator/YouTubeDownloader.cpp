#include "YouTubeDownloader.h"



YouTubeDownloader::YouTubeDownloader(QString const& ffmpegPath, QObject* parent /*= nullptr*/) : QObject(parent), ffmpeg(ffmpegPath)
{
	consoleApp.setProgram(consolePath);
}

YouTubeDownloader::~YouTubeDownloader()
{
}

YouTubeDownloader::MetaData YouTubeDownloader::downloadMetaData(QUrl const& youtube) const
{
	QJsonObject retn;

	consoleApp.setArguments( {"--dump-json",	youtube.toString() } );
	consoleApp.start();
	consoleApp.waitForFinished();

	const QString output(consoleApp.readAllStandardOutput());
	const QString error(consoleApp.readAllStandardError());

	if (error.isEmpty()) {
		retn = createModifiedJsonObject(output);
	}

	return retn;
}

void YouTubeDownloader::download(QUrl const& youtube, QString const& output) const
{
	consoleApp.setArguments({
			"--ffmpeg-location",
			ffmpeg,
			"-f bestvideo+bestaudio",
			"--merge-output-format",
			"mp4",
			youtube.toString(),
			"--output",
			output
		}
	);

	consoleApp.start();
	consoleApp.waitForFinished();
}

 QJsonObject YouTubeDownloader::createModifiedJsonObject( QString const& ytDlJsonReturn ) const
{
	QJsonDocument doc = QJsonDocument::fromJson(ytDlJsonReturn.toUtf8());
	QJsonObject retn;

	if (!doc.isNull() && doc.isObject()) {
			auto fullObject = doc.object();
			static const QStringList acceptedKeys { "id", "channel", "channel_url",  "uploader", "title", "categories",   "tags" };

			for (auto const& key : fullObject.keys()) {
				if (acceptedKeys.contains(key)){
					auto value = fullObject.value(key);
					retn[key] = value;
				}
			}
	}

	return retn;
}

QString const& YouTubeDownloader::getPath() const
{
	return consolePath;
}

