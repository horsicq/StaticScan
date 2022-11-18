/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "dialogstaticscanprocess.h"

DialogStaticScanProcess::DialogStaticScanProcess(QWidget *pParent)
    : XDialogProcess(pParent)
{
    g_pScan = new StaticScan;
    g_pThread = new QThread;

    g_pScan->moveToThread(g_pThread);

    connect(g_pThread, SIGNAL(started()), g_pScan, SLOT(process()));
    connect(g_pScan, SIGNAL(completed(qint64)), this, SLOT(onCompleted(qint64)));
    connect(g_pScan, SIGNAL(scanFileStarted(QString)), this, SIGNAL(scanFileStarted(QString)), Qt::DirectConnection);
    connect(g_pScan, SIGNAL(scanResult(SpecAbstract::SCAN_RESULT)), this, SIGNAL(scanResult(SpecAbstract::SCAN_RESULT)), Qt::DirectConnection);
}

void DialogStaticScanProcess::setData(QString sFileName, SpecAbstract::SCAN_OPTIONS *pOptions, SpecAbstract::SCAN_RESULT *pScanResult)
{
    g_pScan->setData(sFileName, pOptions, pScanResult, getPdStruct());
    g_pThread->start();
}

void DialogStaticScanProcess::setData(QIODevice *pDevice, SpecAbstract::SCAN_OPTIONS *pOptions, SpecAbstract::SCAN_RESULT *pScanResult)
{
    g_pScan->setData(pDevice, pOptions, pScanResult, getPdStruct());
    g_pThread->start();
}

void DialogStaticScanProcess::setData(QString sDirectoryName, SpecAbstract::SCAN_OPTIONS *pOptions)
{
    g_pScan->setData(sDirectoryName, pOptions, getPdStruct());
    g_pThread->start();
}

DialogStaticScanProcess::~DialogStaticScanProcess()
{
    stop();
    waitForFinished();

    g_pThread->quit();
    g_pThread->wait();

    //    g_pThread->deleteLater(); // TODO
    //    g_pScan->deleteLater(); // TODO

    delete g_pThread;
    delete g_pScan;
}

bool DialogStaticScanProcess::saveResult(QWidget *pParent, ScanItemModel *pModel, QString sResultFileName)
{
    bool bResult = false;

    if (pModel) {
        QString sFilter;
        sFilter += QString("%1 (*.txt)").arg(tr("Text documents"));
        QString sFileName = QFileDialog::getSaveFileName(pParent, tr("Save result"), sResultFileName, sFilter);

        if (!sFileName.isEmpty()) {
            QFile file;
            file.setFileName(sFileName);

            if (file.open(QIODevice::ReadWrite)) {
                QString sText = pModel->toFormattedString();

                file.write(sText.toUtf8().data());

                file.close();

                bResult = true;
            }
        }
    }

    return bResult;
}
