// copyright (c) 2017-2021 hors<horsicq@gmail.com>
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
#ifndef DIALOGSTATICSCANPROCESS_H
#define DIALOGSTATICSCANPROCESS_H

#include <QDateTime>
#include <QDialog>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QThread>
#include "staticscanitemmodel.h"

namespace Ui
{
class DialogStaticScanProcess;
}

class DialogStaticScanProcess : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStaticScanProcess(QWidget *pParent=nullptr);
    ~DialogStaticScanProcess();

    void setData(QString sFileName,SpecAbstract::SCAN_OPTIONS *pOptions,SpecAbstract::SCAN_RESULT *pScanResult);
    void setData(QIODevice *pDevice,SpecAbstract::SCAN_OPTIONS *pOptions,SpecAbstract::SCAN_RESULT *pScanResult);
    void setData(QString sDirectoryName,SpecAbstract::SCAN_OPTIONS *pOptions);

    static bool saveResult(QWidget *pParent,StaticScanItemModel *pModel,QString sResultFileName);

private slots:
    void on_pushButtonCancel_clicked();
    void onCompleted(qint64 nElapsed);
    void onSetProgressMaximum(int nValue);
    void onSetProgressValueChanged(int nValue);
    void timerSlot();

signals:
    void scanFileStarted(QString sFileName);
    void scanResult(SpecAbstract::SCAN_RESULT scanResult);

private:
    static const qint32 N_REFRESH_DELAY=1000; // msec TODO Check
    Ui::DialogStaticScanProcess *ui;
    StaticScan *g_pScan;
    QThread *g_pThread;
    bool g_bIsRun;
    QTimer *g_pTimer;
};

#endif // DIALOGSTATICSCANPROCESS_H
