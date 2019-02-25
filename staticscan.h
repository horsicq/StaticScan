// copyright (c) 2017-2019 hors<horsicq@gmail.com>
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
#ifndef STATICSCAN_H
#define STATICSCAN_H

#include <QObject>
#include <QElapsedTimer>
#include <QMutex>
#include <QTimer>
#include "specabstract.h"

#define SSE_VERSION "1.04"

class StaticScan : public QObject
{
    Q_OBJECT
public:
    struct STATS
    {
        qint32 nTotal;
        quint64 nCurrent;
        qint64 nElapsed;
        QString sStatus;
    };
    explicit StaticScan(QObject *parent=nullptr);
    void setData(QString sFileName,SpecAbstract::SCAN_OPTIONS *pOptions,SpecAbstract::SCAN_RESULT *pScanResult);
    void setData(QString sFileName,SpecAbstract::SCAN_OPTIONS *pOptions);
    static SpecAbstract::SCAN_RESULT processFile(QString sFileName,SpecAbstract::SCAN_OPTIONS *pOptions);
    static QString getEngineVersion();
    STATS getCurrentStats();
private:
    void _process(QIODevice *pDevice, SpecAbstract::SCAN_RESULT *pScanResult, qint64 nOffset, qint64 nSize, SpecAbstract::ID parentId, SpecAbstract::SCAN_OPTIONS *pOptions,int nLevel=0);
    SpecAbstract::SCAN_RESULT scanFile(QString sFileName);

signals:
    void completed(qint64 nElapsedTime);
    void scanResult(SpecAbstract::SCAN_RESULT scanResult);
public slots:
    void process();
    void stop();

private:
    QString sFileName;
    SpecAbstract::SCAN_OPTIONS *pOptions;
    SpecAbstract::SCAN_RESULT *pScanResult;
    bool bIsStop;
    STATS currentStats;
    QElapsedTimer *pElapsedTimer;
};

#endif // STATICSCAN_H
