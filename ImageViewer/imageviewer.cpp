#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <QFileDialog>
#include <QMouseEvent>
#include <QTime>
#include <QMessageBox>

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);

    this->ui->previousButton->setEnabled(false);
    this->ui->nextButton->setEnabled(false);
    this->ui->startSlideShowButton->setEnabled(false);
    this->ui->stopSlideShowButton->setEnabled(false);
    this->ui->delayComboBox->setEnabled(false);
    this->ui->delayLabel->setEnabled(false);
    this->ui->browseButton->setEnabled(true);

    this->ui->actionBrowse->setEnabled(true);
    this->ui->actionPrevious->setEnabled(false);
    this->ui->actionNext->setEnabled(false);
    this->ui->actionStart_SlideShow->setEnabled(false);
    this->ui->actionStop_SlideShow->setEnabled(false);
    this->ui->shuffleCheckBox->setEnabled(false);

    slideShowActive = false;
    sessionActive = false;
    m_bShuffle = false;

    QStringList list = (QStringList()<<"5 seconds"<<"10 seconds"<<"15 seconds"<<"20 seconds"<<"25 seconds"<<"30 seconds");
    this->ui->delayComboBox->addItems(list);

    connect(this->ui->actionBrowse, SIGNAL(triggered()), this, SLOT(on_browseButton_clicked()));
    connect(this->ui->actionPrevious, SIGNAL(triggered()), this, SLOT(on_previousButton_clicked()));
    connect(this->ui->actionNext, SIGNAL(triggered()), this, SLOT(on_nextButton_clicked()));
    connect(this->ui->actionStart_SlideShow, SIGNAL(triggered()), this, SLOT(on_startSlideShowButton_clicked()));
    connect(this->ui->actionStop_SlideShow, SIGNAL(triggered()), this, SLOT(on_stopSlideShowButton_clicked()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(closeApplication()));
    connect(this->ui->actionAbout, SIGNAL(triggered()), this, SLOT(callAboutFunction()));

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::on_browseButton_clicked()
{
    QFileDialog *fileDialog = new QFileDialog;
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setOption(QFileDialog::ShowDirsOnly);
    fileDialog->setViewMode(QFileDialog::Detail);

    int result = fileDialog->exec();

    if(result)
    {
        folderString = fileDialog->selectedFiles()[0];
        dirFunction();
    }
    else
    {
        fileDialog->close();
    }
}

void ImageViewer::dirFunction()
{
    QDir folder(folderString);
    folderString = folder.absolutePath();
    folder.setNameFilters(QStringList()<<"*.jpg"<<"*.png"<<"*.bmp"<<"*.gif");
    fileList = folder.entryList();

    fileCount = fileList.count();

    if(fileCount > 1)
    {
        this->ui->previousButton->setEnabled(false);
        this->ui->nextButton->setEnabled(true);
        this->ui->startSlideShowButton->setEnabled(true);

        this->ui->actionPrevious->setEnabled(false);
        this->ui->actionNext->setEnabled(true);
        this->ui->actionStart_SlideShow->setEnabled(true);
        this->ui->shuffleCheckBox->setEnabled(true);
    }
    else if(fileCount == 1)
    {
        this->ui->previousButton->setEnabled(false);
        this->ui->nextButton->setEnabled(false);

        this->ui->actionPrevious->setEnabled(false);
        this->ui->actionNext->setEnabled(false);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("No compatible pictures in this folder...");
        msgBox.exec();

        return;
    }

    index = 0;

    showImage();
}

void ImageViewer::showImage()
{
    sessionActive = true;

    if(index > 0)
    {
        this->ui->previousButton->setEnabled(true);
        this->ui->nextButton->setEnabled(true);

        this->ui->actionPrevious->setEnabled(true);
        this->ui->actionNext->setEnabled(true);
    }
    else if(index == 0 && fileCount > 0)
    {
        this->ui->previousButton->setEnabled(false);
        this->ui->nextButton->setEnabled(true);

        this->ui->actionPrevious->setEnabled(false);
        this->ui->actionNext->setEnabled(true);
    }

    if(index == fileCount-1)
    {
        this->ui->nextButton->setEnabled(false);

        this->ui->actionNext->setEnabled(false);
    }

    if(fileCount > 1 && slideShowActive == false)
    {
        this->ui->delayComboBox->setEnabled(true);
        this->ui->delayLabel->setEnabled(true);
    }

    QPixmap pix(folderString + "/" + fileList[index]);

    if((pix.width() > this->width()-50) || (pix.height() > this->height()-75))
    {
        this->ui->imageLabel->setPixmap(pix.scaled((this->width()-50), (this->height()-75), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
    {
        this->ui->imageLabel->setPixmap(pix);
    }

    this->ui->imageLabel->setAlignment(Qt::AlignCenter);

    this->ui->fileNameLabel->setText(fileList[index]);
}

void ImageViewer::on_previousButton_clicked()
{
    if(index > 0)
    {
        index--;
        showImage();
    }
}

void ImageViewer::on_nextButton_clicked()
{
    if(index < fileCount-1)
    {
        this->ui->previousButton->setEnabled(true);

        this->ui->actionPrevious->setEnabled(true);

        index++;
        showImage();
    }
}

void ImageViewer::on_startSlideShowButton_clicked()
{
   slideShowActive = true;

   this->ui->startSlideShowButton->setEnabled(false);
   this->ui->stopSlideShowButton->setEnabled(true);
   this->ui->delayComboBox->setEnabled(false);
   this->ui->browseButton->setEnabled(false);
   this->ui->delayLabel->setEnabled(false);

   this->ui->actionBrowse->setEnabled(false);
   this->ui->actionStart_SlideShow->setEnabled(false);
   this->ui->actionStop_SlideShow->setEnabled(true);

   timer = new QTimer(this);

   connect(timer, SIGNAL(timeout()), this, SLOT(slideShowFunction()));
   timer->start(delay * 1000);
}

void ImageViewer::slideShowFunction()
{
    this->ui->delayComboBox->setEnabled(false);
    this->ui->delayLabel->setEnabled(false);

    if(m_bShuffle == false)
    {
        if(index < fileCount-1)
        {
            index++;
        }
        else
        {
            index = 0;
        }
    }
    else
    {
        index = qrand()%fileCount;
    }

    showImage();
}

void ImageViewer::on_stopSlideShowButton_clicked()
{
    slideShowActive = false;

    this->ui->startSlideShowButton->setEnabled(true);
    this->ui->stopSlideShowButton->setEnabled(false);
    this->ui->delayComboBox->setEnabled(true);
    this->ui->browseButton->setEnabled(true);
    this->ui->delayLabel->setEnabled(true);

    this->ui->actionBrowse->setEnabled(true);
    this->ui->actionStart_SlideShow->setEnabled(true);
    this->ui->actionStop_SlideShow->setEnabled(false);

    timer->stop();
}

void ImageViewer::on_delayComboBox_currentIndexChanged(int index)
{
    delay = (index+1)*5;
}

void ImageViewer::showMaximized()
{
    QMessageBox msgBox;
    msgBox.setText("Full Screen Mode Called...");
    msgBox.exec();
}

void ImageViewer::changeEvent(QEvent* i_ptrStatechangeEvent)
{
    if(i_ptrStatechangeEvent->type() == QEvent::WindowStateChange)
    {
//        showMaximized();
        adjustWindowElements();
    }
}

void ImageViewer::adjustWindowElements()
{
    QSize windowSize = this->size();
    int width = windowSize.width();
    int height = windowSize.height();

    this->ui->imageLabel->QLabel::setFixedWidth(width-50);
    this->ui->imageLabel->QLabel::setFixedHeight(height-75);

    this->ui->fileNameLabel->QLabel::setFixedWidth(width-20);

    if(sessionActive == true)
    {
        showImage();
    }
}

void ImageViewer::closeApplication()
{
    this->close();
}

void ImageViewer::callAboutFunction()
{
    aboutWindow = new About();
    aboutWindow->show();
}

void ImageViewer::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F10)
    {
        if(this->ui->nextButton->isEnabled())
        {
            on_nextButton_clicked();
        }
    }
    else if(event->key() == Qt::Key_F9)
    {
        if(this->ui->nextButton->isEnabled())
        {
            on_previousButton_clicked();
        }
    }
}

void ImageViewer::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   QSize windowSize = this->size();
   int width = windowSize.width();
   int height = windowSize.height();

   this->ui->imageLabel->QLabel::setFixedWidth(width-50);
   this->ui->imageLabel->QLabel::setFixedHeight(height-75);

   this->ui->fileNameLabel->QLabel::setFixedWidth(width-20);

   if(sessionActive == true)
   {
       showImage();
   }
}

void ImageViewer::on_shuffleCheckBox_toggled(bool checked)
{
    m_bShuffle = checked;
}
