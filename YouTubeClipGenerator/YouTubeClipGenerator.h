#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_YouTubeClipGenerator.h"

class YouTubeClipGenerator : public QMainWindow
{
    Q_OBJECT

public:
    YouTubeClipGenerator(QWidget *parent = Q_NULLPTR);

private:
    Ui::YouTubeClipGeneratorClass ui;
};
