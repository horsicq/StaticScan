// copyright (c) 2017-2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "dialogstaticscan.h"
#include "ui_dialogstaticscan.h"

DialogStaticScan::DialogStaticScan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStaticScan)
{
    ui->setupUi(this);

    pScan=new StaticScan;
    pThread=new QThread;

    pScan->moveToThread(pThread);

    connect(pThread, SIGNAL(started()), pScan, SLOT(process()));
    connect(pScan, SIGNAL(completed(qint64)), this, SLOT(onCompleted(qint64)));
    connect(pScan, SIGNAL(scanFileStarted(QString)),this,SIGNAL(scanFileStarted(QString)),Qt::DirectConnection);
    connect(pScan, SIGNAL(scanResult(SpecAbstract::SCAN_RESULT)),this,SIGNAL(scanResult(SpecAbstract::SCAN_RESULT)),Qt::DirectConnection);

    pTimer=new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));

    bIsRun=false;
}

void DialogStaticScan::setData(QString sFileName, SpecAbstract::SCAN_OPTIONS *pOptions, SpecAbstract::SCAN_RESULT *pScanResult)
{
    bIsRun=true;
    pScan->setData(sFileName,pOptions,pScanResult);
    pThread->start();
    pTimer->start(1000); // 1 sec TODO const
    ui->progressBarTotal->setMaximum(0);
}

void DialogStaticScan::setData(QIODevice *pDevice, SpecAbstract::SCAN_OPTIONS *pOptions, SpecAbstract::SCAN_RESULT *pScanResult)
{
    bIsRun=true;
    pScan->setData(pDevice,pOptions,pScanResult);
    pThread->start();
    pTimer->start(1000); // 1 sec TODO const
    ui->progressBarTotal->setMaximum(0);
}

void DialogStaticScan::setData(QString sDirectoryName, SpecAbstract::SCAN_OPTIONS *pOptions)
{
    bIsRun=true;
    pScan->setData(sDirectoryName,pOptions);
    pThread->start();
    pTimer->start(1000); // 1 sec TODO const
    ui->progressBarTotal->setMaximum(100);
}

DialogStaticScan::~DialogStaticScan()
{
    if(bIsRun)
    {
        pScan->stop();
    }

    pTimer->stop();

    pThread->quit();
    pThread->wait();

    delete ui;

    delete pThread;
    delete pScan;
}

bool DialogStaticScan::saveResult(QWidget *pParent, StaticScanItemModel *pModel, QString sResultFileName)
{
    bool bResult=false;

    if(pModel)
    {
        QString sFilter;
        sFilter+=QString("%1 (*.txt)").arg(tr("Text documents"));
        QString sFileName=QFileDialog::getSaveFileName(pParent,tr("Save result"),sResultFileName,sFilter);

        if(!sFileName.isEmpty())
        {
            QFile file;
            file.setFileName(sFileName);

            if(file.open(QIODevice::ReadWrite))
            {
                QString sText=pModel->toFormattedString();

                file.write(sText.toLatin1().data());

                file.close();

                bResult=true;
            }
        }
    }

    return bResult;
}

void DialogStaticScan::on_pushButtonCancel_clicked()
{
    if(bIsRun)
    {
        pScan->stop();
        pTimer->stop();
        bIsRun=false;
    }
}

void DialogStaticScan::onCompleted(qint64 nElapsed)
{
    Q_UNUSED(nElapsed)
    // TODO
    bIsRun=false;
    this->close();
}

void DialogStaticScan::onSetProgressMaximum(int nValue)
{
    ui->progressBarTotal->setMaximum(nValue);
}

void DialogStaticScan::onSetProgressValue(int nValue)
{
    ui->progressBarTotal->setValue(nValue);
}

void DialogStaticScan::timerSlot()
{
    StaticScan::STATS stats=pScan->getCurrentStats();

    ui->labelTotal->setText(QString::number(stats.nTotal));
    ui->labelCurrent->setText(QString::number(stats.nCurrent));
    ui->labelCurrentStatus->setText(stats.sStatus);

    if(stats.nTotal)
    {
        ui->progressBarTotal->setValue((int)((stats.nCurrent*100)/stats.nTotal));
    }

    QDateTime dt;
    dt.setMSecsSinceEpoch(stats.nElapsed);
    QString sDateTime=dt.time().addSecs(-60*60).toString("hh:mm:ss");

    ui->labelTime->setText(sDateTime);
}
