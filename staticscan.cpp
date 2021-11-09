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
#include "staticscan.h"

StaticScan::StaticScan(QObject *pParent) : QObject(pParent)
{
    g_bIsStop=false;
    g_pOptions=nullptr;
    g_pScanResult=nullptr;
    g_currentStats={};
    g_pElapsedTimer=nullptr;
    g_scanType=SCAN_TYPE_DEVICE;
    g_pDevice=nullptr;
}

void StaticScan::setData(QString sFileName, SpecAbstract::SCAN_OPTIONS *pOptions,SpecAbstract::SCAN_RESULT *pScanResult)
{
    this->g_sFileName=sFileName;
    this->g_pOptions=pOptions;
    this->g_pScanResult=pScanResult;

    this->g_scanType=SCAN_TYPE_FILE;
}

void StaticScan::setData(QIODevice *pDevice, SpecAbstract::SCAN_OPTIONS *pOptions, SpecAbstract::SCAN_RESULT *pScanResult)
{
    this->g_pDevice=pDevice;
    this->g_pOptions=pOptions;
    this->g_pScanResult=pScanResult;

    this->g_scanType=SCAN_TYPE_DEVICE;
}

void StaticScan::setData(char *pData, qint32 nDataSize, SpecAbstract::SCAN_OPTIONS *pOptions, SpecAbstract::SCAN_RESULT *pScanResult)
{
    this->g_pData=pData;
    this->g_nDataSize=nDataSize;
    this->g_pOptions=pOptions;
    this->g_pScanResult=pScanResult;

    this->g_scanType=SCAN_TYPE_MEMORY;
}

void StaticScan::setData(QString sDirectoryName, SpecAbstract::SCAN_OPTIONS *pOptions)
{
    this->g_sDirectoryName=sDirectoryName;
    this->g_pOptions=pOptions;

    this->g_scanType=SCAN_TYPE_DIRECTORY;
}

void StaticScan::process()
{
    g_pElapsedTimer=new QElapsedTimer;
    g_pElapsedTimer->start();

    g_currentStats.nTotal=0;
    g_currentStats.nCurrent=0;

    g_bIsStop=false;

    if(this->g_scanType==SCAN_TYPE_FILE)
    {
        if((g_pScanResult)&&(g_sFileName!=""))
        {
            g_currentStats.sStatus=tr("File scan");

            emit scanFileStarted(g_sFileName);

            *g_pScanResult=scanFile(g_sFileName);

            emit scanResult(*g_pScanResult);
        }
    }
    else if(this->g_scanType==SCAN_TYPE_DEVICE)
    {
        if(g_pDevice)
        {
            g_currentStats.sStatus=tr("Device scan");

            *g_pScanResult=scanDevice(g_pDevice);

            emit scanResult(*g_pScanResult);
        }
    }
    else if(this->g_scanType==SCAN_TYPE_MEMORY)
    {
        g_currentStats.sStatus=tr("Memory scan");

        *g_pScanResult=scanMemory(g_pData,g_nDataSize);

        emit scanResult(*g_pScanResult);
    }
    else if(this->g_scanType==SCAN_TYPE_DIRECTORY)
    {
        if(g_sDirectoryName!="")
        {
            g_currentStats.sStatus=tr("Directory scan");
            QList<QString> listFileNames;

            XBinary::FFOPTIONS ffoptions={};
            ffoptions.bSubdirectories=g_pOptions->bSubdirectories;
            ffoptions.pbIsStop=&g_bIsStop;
            ffoptions.pnNumberOfFiles=&(g_currentStats.nTotal);
            ffoptions.pListFileNames=&listFileNames;

            XBinary::findFiles(g_sDirectoryName,&ffoptions);

            g_currentStats.nTotal=listFileNames.count();

            for(qint32 i=0;(i<g_currentStats.nTotal)&&(!g_bIsStop);i++)
            {
                g_currentStats.nCurrent=i+1;
                g_currentStats.sStatus=listFileNames.at(i);

                emit scanFileStarted(g_currentStats.sStatus);

                SpecAbstract::SCAN_RESULT _scanResult=scanFile(g_currentStats.sStatus);

                emit scanResult(_scanResult);
            }
        }
    }

    emit completed(g_pElapsedTimer->elapsed());
    delete g_pElapsedTimer;
    g_pElapsedTimer=nullptr;

    g_bIsStop=false;
}

void StaticScan::stop()
{
    g_bIsStop=true;
}

SpecAbstract::SCAN_RESULT StaticScan::processDevice(QIODevice *pDevice, SpecAbstract::SCAN_OPTIONS *pOptions)
{
    SpecAbstract::SCAN_RESULT result={0};
    StaticScan scan;
    scan.setData(pDevice,pOptions,&result);
    scan.process();

    return result;
}

SpecAbstract::SCAN_RESULT StaticScan::processFile(QString sFileName, SpecAbstract::SCAN_OPTIONS *pOptions)
{
    SpecAbstract::SCAN_RESULT result={0};
    StaticScan scan;
    scan.setData(sFileName,pOptions,&result);
    scan.process();

    return result;
}

SpecAbstract::SCAN_RESULT StaticScan::processMemory(char *pData, qint32 nDataSize, SpecAbstract::SCAN_OPTIONS *pOptions)
{
    SpecAbstract::SCAN_RESULT result={0};
    StaticScan scan;
    scan.setData(pData,nDataSize,pOptions,&result);
    scan.process();

    return result;
}

QString StaticScan::getEngineVersion()
{
    return SSE_VERSION;
}

StaticScan::STATS StaticScan::getCurrentStats()
{
    if(g_pElapsedTimer)
    {
        if(g_pElapsedTimer->isValid())
        {
            g_currentStats.nElapsed=g_pElapsedTimer->elapsed();
        }
        else
        {
            g_currentStats.nElapsed=0;
        }
    }

    return g_currentStats;
}

void StaticScan::_process(QIODevice *pDevice,SpecAbstract::SCAN_RESULT *pScanResult,qint64 nOffset,qint64 nSize,SpecAbstract::ID parentId,SpecAbstract::SCAN_OPTIONS *pOptions)
{
    SpecAbstract::scan(pDevice,pScanResult,nOffset,nSize,parentId,pOptions,true,&g_bIsStop);
}

SpecAbstract::SCAN_RESULT StaticScan::scanFile(QString sFileName)
{
    SpecAbstract::SCAN_RESULT result={0};

    if(sFileName!="")
    {
        QFile file;
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadOnly))
        {
            result=scanDevice(&file);

            file.close();
        }
    }

    return result;
}

SpecAbstract::SCAN_RESULT StaticScan::scanDevice(QIODevice *pDevice)
{
    SpecAbstract::SCAN_RESULT result={0};

    SpecAbstract::ID parentId={0};
    parentId.fileType=XBinary::FT_UNKNOWN;
    parentId.filePart=SpecAbstract::RECORD_FILEPART_HEADER;
    _process(pDevice,&result,0,pDevice->size(),parentId,g_pOptions);

    return result;
}

SpecAbstract::SCAN_RESULT StaticScan::scanMemory(char *pData, qint32 nSize)
{
    SpecAbstract::SCAN_RESULT result={0};

    QBuffer buffer;

    buffer.setData(pData,nSize);

    if(buffer.open(QIODevice::ReadOnly))
    {
        result=scanDevice(&buffer);

        buffer.close();
    }

    return result;
}
