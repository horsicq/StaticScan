/* Copyright (c) 2017-2026 hors<horsicq@gmail.com>
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
#ifndef DIALOGSTATICSCANPROCESS_H
#define DIALOGSTATICSCANPROCESS_H

#include <QDateTime>
#include <QDialog>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QThread>

#include "scanitemmodel.h"
#include "staticscan.h"
#include "xdialogprocess.h"

class DialogStaticScanProcess : public XDialogProcess {
    Q_OBJECT

public:
    explicit DialogStaticScanProcess(QWidget *pParent = nullptr);
    ~DialogStaticScanProcess();

    void setData(const QString &sFileName, XScanEngine::SCAN_OPTIONS *pOptions, XScanEngine::SCAN_RESULT *pScanResult);
    void setData(QIODevice *pDevice, XScanEngine::SCAN_OPTIONS *pOptions, XScanEngine::SCAN_RESULT *pScanResult);
    void setData(const QString &sDirectoryName, XScanEngine::SCAN_OPTIONS *pOptions);
    static bool saveResult(QWidget *pParent, ScanItemModel *pModel, const QString &sResultFileName);  // TODO move to scanitemmodel.h

signals:
    void scanFileStarted(QString sFileName);
    void scanResult(XScanEngine::SCAN_RESULT scanResult);

private:
    StaticScan *g_pScan;
    QThread *g_pThread;
};

#endif  // DIALOGSTATICSCANPROCESS_H
