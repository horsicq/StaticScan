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
#include "staticscan.h"

StaticScan::StaticScan(QObject *parent) : QObject(parent)
{
    bIsStop=false;
    _pOptions=nullptr;
    _pScanResult=nullptr;
    currentStats={};
    pElapsedTimer=nullptr;

    scanType=SCAN_TYPE_DEVICE;
}

void StaticScan::setData(QString sFileName, SpecAbstract::SCAN_OPTIONS *pOptions,SpecAbstract::SCAN_RESULT *pScanResult)
{
    this->_sFileName=sFileName;
    this->_pOptions=pOptions;
    this->_pScanResult=pScanResult;

    this->scanType=SCAN_TYPE_FILE;
}

void StaticScan::setData(QIODevice *pDevice, SpecAbstract::SCAN_OPTIONS *pOptions, SpecAbstract::SCAN_RESULT *pScanResult)
{
    this->_pDevice=pDevice;
    this->_pOptions=pOptions;
    this->_pScanResult=pScanResult;

    this->scanType=SCAN_TYPE_DEVICE;
}

void StaticScan::setData(QString sDirectoryName, SpecAbstract::SCAN_OPTIONS *pOptions)
{
    this->_sDirectoryName=sDirectoryName;
    this->_pOptions=pOptions;

    this->scanType=SCAN_TYPE_DIRECTORY;
}

void StaticScan::process()
{
    pElapsedTimer=new QElapsedTimer;
    pElapsedTimer->start();

    currentStats.nTotal=0;
    currentStats.nCurrent=0;

    bIsStop=false;

    if(this->scanType==SCAN_TYPE_FILE)
    {
        if((_pScanResult)&&(_sFileName!=""))
        {
            currentStats.sStatus=tr("File scan");

            emit scanFileStarted(_sFileName);

            *_pScanResult=scanFile(_sFileName);

            emit scanResult(*_pScanResult);
        }
    }
    else if(this->scanType==SCAN_TYPE_DIRECTORY)
    {
        if(_sDirectoryName!="")
        {
            currentStats.sStatus=tr("Directory scan");
            QList<QString> listFiles;

            XBinary::FFOPTIONS ffoptions={};
            ffoptions.bSubdirectories=_pOptions->bSubdirectories;
            ffoptions.pbIsStop=&bIsStop;
            ffoptions.pnNumberOfFiles=&(currentStats.nTotal);
            ffoptions.pListFiles=&listFiles;

            XBinary::findFiles(_sDirectoryName,&ffoptions);

            currentStats.nTotal=listFiles.count();

            for(int i=0; (i<currentStats.nTotal)&&(!bIsStop); i++)
            {
                currentStats.nCurrent=i+1;
                currentStats.sStatus=listFiles.at(i);

                emit scanFileStarted(currentStats.sStatus);

                SpecAbstract::SCAN_RESULT _scanResult=scanFile(currentStats.sStatus);

                emit scanResult(_scanResult);
            }
        }
    }
    else if(this->scanType==SCAN_TYPE_DEVICE)
    {
        if(_pDevice)
        { 
            currentStats.sStatus=tr("Device scan");

            *_pScanResult=scanDevice(_pDevice);

            emit scanResult(*_pScanResult);
        }
    }

    emit completed(pElapsedTimer->elapsed());
    delete pElapsedTimer;
    pElapsedTimer=nullptr;

    bIsStop=false;
}

void StaticScan::stop()
{
    bIsStop=true;
}

SpecAbstract::SCAN_RESULT StaticScan::processFile(QString sFileName, SpecAbstract::SCAN_OPTIONS *pOptions)
{
    SpecAbstract::SCAN_RESULT scanResult;
    StaticScan scan;
    scan.setData(sFileName,pOptions,&scanResult);
    scan.process();

    return scanResult;
}

QString StaticScan::getEngineVersion()
{
    return SSE_VERSION;
}

StaticScan::STATS StaticScan::getCurrentStats()
{
    if(pElapsedTimer)
    {
        currentStats.nElapsed=pElapsedTimer->elapsed();
    }

    return currentStats;
}

void StaticScan::_process(QIODevice *pDevice,SpecAbstract::SCAN_RESULT *pScanResult,qint64 nOffset,qint64 nSize,SpecAbstract::ID parentId,SpecAbstract::SCAN_OPTIONS *pOptions)
{
    SpecAbstract::scan(pDevice,pScanResult,nOffset,nSize,parentId,pOptions,true);
}

SpecAbstract::SCAN_RESULT StaticScan::scanFile(QString sFileName)
{
    SpecAbstract::SCAN_RESULT result={0};

    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadOnly))
    {
        result=scanDevice(&file);

        file.close();
    }

    return result;
}

SpecAbstract::SCAN_RESULT StaticScan::scanDevice(QIODevice *pDevice)
{
    SpecAbstract::SCAN_RESULT result={0};

    SpecAbstract::ID parentId={0};
    parentId.filetype=SpecAbstract::RECORD_FILETYPE_UNKNOWN;
    parentId.filepart=SpecAbstract::RECORD_FILEPART_HEADER;
    _process(pDevice,&result,0,pDevice->size(),parentId,_pOptions);

    return result;
}
