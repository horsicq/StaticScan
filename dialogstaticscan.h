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
#ifndef DIALOGSTATICSCAN_H
#define DIALOGSTATICSCAN_H

#include <QDialog>
#include <QThread>
#include "staticscan.h"

namespace Ui
{
class DialogStaticScan;
}

class DialogStaticScan : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStaticScan(QWidget *parent = 0);
    void setData(QString sFileName,SpecAbstract::SCAN_OPTIONS *pOptions,QList<SpecAbstract::SCAN_STRUCT> *pListResult);
    ~DialogStaticScan();

private slots:
    void on_pushButtonCancel_clicked();
    void onCompleted(quint64 nElapsed);
    void onSetProgressMaximum(int nMax);
    void onSetProgressValue(int nValue);
private:
    Ui::DialogStaticScan *ui;
    StaticScan *scan;
    QThread *thread;
    bool bIsRun;
};

#endif // DIALOGSTATICSCAN_H
