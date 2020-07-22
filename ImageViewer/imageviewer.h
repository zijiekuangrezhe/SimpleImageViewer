#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <qtimer.h>
#include "about.h"

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();

private slots:
    void on_browseButton_clicked();
    void dirFunction();
    void showImage();

    void on_previousButton_clicked();
    void on_nextButton_clicked();

    void on_startSlideShowButton_clicked();
    void slideShowFunction();
    void on_stopSlideShowButton_clicked();
    void on_delayComboBox_currentIndexChanged(int index);

    void showMaximized();
    void changeEvent(QEvent* i_ptrStatechangeEvent);
    void adjustWindowElements();

    void callAboutFunction();
    void closeApplication();

    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent* event);

    void on_shuffleCheckBox_toggled(bool checked);

private:
    Ui::ImageViewer *ui;

    QString folderString;
    QStringList fileList;
    int index;
    int fileCount;
    QTimer *timer;
    int delay;
    bool slideShowActive;
    bool sessionActive;
    bool m_bShuffle;

    About *aboutWindow;
};

#endif // IMAGEVIEWER_H
