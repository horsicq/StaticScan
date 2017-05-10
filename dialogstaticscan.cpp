// Copyright (c) 2017 hors<horsicq@gmail.com>
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

    scan=new StaticScan;
    thread=new QThread;

    scan->moveToThread(thread);

    connect(thread, SIGNAL(started()), scan, SLOT(process()));
    connect(scan, SIGNAL(completed(quint64)), this, SLOT(onCompleted(quint64)));
    connect(scan, SIGNAL(setProgressMaximum(int)), this, SLOT(onSetProgressMaximum(int)));
    connect(scan, SIGNAL(setProgressValue(int)), this, SLOT(onSetProgressValue(int)));

    bIsRun=false;
}

void DialogStaticScan::setData(QString sFileName, SpecAbstract::SCAN_OPTIONS *pOptions, QList<SpecAbstract::SCAN_STRUCT> *pListResult)
{
    bIsRun=true;
    scan->setData(sFileName,pOptions,pListResult);
    thread->start();
}

DialogStaticScan::~DialogStaticScan()
{
    if(bIsRun)
    {
        scan->stop();
    }

    thread->quit();
    thread->wait();

    delete ui;

    delete thread;
    delete scan;
}

void DialogStaticScan::on_pushButtonCancel_clicked()
{
    if(bIsRun)
    {
        scan->stop();
        bIsRun=false;
    }
}

void DialogStaticScan::onCompleted(quint64 nElapsed)
{
    Q_UNUSED(nElapsed);
    // TODO
    this->close();
}

void DialogStaticScan::onSetProgressMaximum(int nMax)
{
    ui->progressBarTotal->setMaximum(nMax);
}

void DialogStaticScan::onSetProgressValue(int nValue)
{
    ui->progressBarTotal->setValue(nValue);
}
