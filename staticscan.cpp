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
#include "staticscan.h"

StaticScan::StaticScan(QObject *pParent) : QObject(pParent)
{
    g_pOptions=nullptr;
    g_pScanResult=nullptr;
    g_scanType=SCAN_TYPE_UNKNOWN;
    g_pDevice=nullptr;
    g_pData=nullptr;
    g_pPdStruct=nullptr;
}

void StaticScan::setData(QString sFileName,SpecAbstract::SCAN_OPTIONS *pOptions,SpecAbstract::SCAN_RESULT *pScanResult,XBinary::PDSTRUCT *pPdStruct)
{
    this->g_sFileName=sFileName;
    this->g_pOptions=pOptions;
    this->g_pScanResult=pScanResult;
    this->g_pPdStruct=pPdStruct;

    this->g_scanType=SCAN_TYPE_FILE;
}

void StaticScan::setData(QIODevice *pDevice,SpecAbstract::SCAN_OPTIONS *pOptions,SpecAbstract::SCAN_RESULT *pScanResult,XBinary::PDSTRUCT *pPdStruct)
{
    this->g_pDevice=pDevice;
    this->g_pOptions=pOptions;
    this->g_pScanResult=pScanResult;
    this->g_pPdStruct=pPdStruct;

    this->g_scanType=SCAN_TYPE_DEVICE;
}

void StaticScan::setData(char *pData,qint32 nDataSize,SpecAbstract::SCAN_OPTIONS *pOptions,SpecAbstract::SCAN_RESULT *pScanResult,XBinary::PDSTRUCT *pPdStruct)
{
    this->g_pData=pData;
    this->g_nDataSize=nDataSize;
    this->g_pOptions=pOptions;
    this->g_pScanResult=pScanResult;
    this->g_pPdStruct=pPdStruct;

    this->g_scanType=SCAN_TYPE_MEMORY;
}

void StaticScan::setData(QString sDirectoryName,SpecAbstract::SCAN_OPTIONS *pOptions,XBinary::PDSTRUCT *pPdStruct)
{
    this->g_sDirectoryName=sDirectoryName;
    this->g_pOptions=pOptions;
    this->g_pPdStruct=pPdStruct;

    this->g_scanType=SCAN_TYPE_DIRECTORY;
}

void StaticScan::process()
{
    XBinary::PDSTRUCT pdStructEmpty={};
    XBinary::PDSTRUCT *pPdStruct=g_pPdStruct;

    if(!pPdStruct)
    {
        pPdStruct=&pdStructEmpty;
    }

    QElapsedTimer scanTimer;
    scanTimer.start();

    pPdStruct->pdRecordOpt.bIsValid=true;

    if(this->g_scanType==SCAN_TYPE_FILE)
    {
        if((g_pScanResult)&&(g_sFileName!=""))
        {
            pPdStruct->sStatus=tr("File scan");
            pPdStruct->pdRecordOpt.sStatus=g_sFileName;

            emit scanFileStarted(g_sFileName);

            *g_pScanResult=scanFile(g_sFileName,pPdStruct);

            emit scanResult(*g_pScanResult);
        }
    }
    else if(this->g_scanType==SCAN_TYPE_DEVICE)
    {
        if(g_pDevice)
        {
            pPdStruct->sStatus=tr("Device scan");
            pPdStruct->pdRecordOpt.sStatus=XBinary::getDeviceFileName(g_pDevice);

            *g_pScanResult=scanDevice(g_pDevice,pPdStruct);

            emit scanResult(*g_pScanResult);
        }
    }
    else if(this->g_scanType==SCAN_TYPE_MEMORY)
    {
        pPdStruct->sStatus=tr("Memory scan");

        *g_pScanResult=scanMemory(g_pData,g_nDataSize,pPdStruct);

        emit scanResult(*g_pScanResult);
    }
    else if(this->g_scanType==SCAN_TYPE_DIRECTORY)
    {
        if(g_sDirectoryName!="")
        {
            pPdStruct->sStatus=tr("Directory scan");
            QList<QString> listFileNames;

            XBinary::findFiles(g_sDirectoryName,&listFileNames,g_pOptions->bSubdirectories,0,pPdStruct);

            pPdStruct->pdRecordObj.bIsValid=true;

            pPdStruct->pdRecordObj.nTotal=listFileNames.count();

            for(qint32 i=0;(i<pPdStruct->pdRecordObj.nTotal)&&(!(pPdStruct->bIsStop));i++)
            {
                pPdStruct->pdRecordObj.nCurrent=i+1;
                pPdStruct->pdRecordObj.sStatus=listFileNames.at(i);

                emit scanFileStarted(pPdStruct->pdRecordObj.sStatus);

                SpecAbstract::SCAN_RESULT _scanResult=scanFile(pPdStruct->pdRecordObj.sStatus,pPdStruct);

                emit scanResult(_scanResult);
            }

            if(!(pPdStruct->bIsStop))
            {
                pPdStruct->pdRecordObj.bSuccess=true;
            }

            pPdStruct->pdRecordObj.bFinished=true;
        }
    }

    if(!(pPdStruct->bIsStop))
    {
        pPdStruct->pdRecordOpt.bSuccess=true;
    }

    pPdStruct->pdRecordOpt.bFinished=true;

    emit completed(scanTimer.elapsed());
}

SpecAbstract::SCAN_RESULT StaticScan::processDevice(QIODevice *pDevice, SpecAbstract::SCAN_OPTIONS *pOptions, XBinary::PDSTRUCT *pPdStruct)
{
    SpecAbstract::SCAN_RESULT result={0};
    StaticScan scan;
    scan.setData(pDevice,pOptions,&result,pPdStruct);
    scan.process();

    return result;
}

SpecAbstract::SCAN_RESULT StaticScan::processFile(QString sFileName, SpecAbstract::SCAN_OPTIONS *pOptions, XBinary::PDSTRUCT *pPdStruct)
{
    SpecAbstract::SCAN_RESULT result={0};
    StaticScan scan;
    scan.setData(sFileName,pOptions,&result,pPdStruct);
    scan.process();

    return result;
}

SpecAbstract::SCAN_RESULT StaticScan::processMemory(char *pData, qint32 nDataSize, SpecAbstract::SCAN_OPTIONS *pOptions, XBinary::PDSTRUCT *pPdStruct)
{
    SpecAbstract::SCAN_RESULT result={0};
    StaticScan scan;
    scan.setData(pData,nDataSize,pOptions,&result,pPdStruct);
    scan.process();

    return result;
}

QString StaticScan::getEngineVersion()
{
    return SSE_VERSION;
}

//StaticScan::STATS StaticScan::getCurrentStats()
//{
//    if(g_pElapsedTimer)
//    {
//        if(g_pElapsedTimer->isValid())
//        {
//            g_currentStats.nElapsed=g_pElapsedTimer->elapsed();
//        }
//        else
//        {
//            g_currentStats.nElapsed=0;
//        }
//    }

//    if(g_pOptions)
//    {
//        g_currentStats.sStatus2=g_pOptions->sStatus;
//    }

//    return g_currentStats;
//}

void StaticScan::_process(QIODevice *pDevice, SpecAbstract::SCAN_RESULT *pScanResult, qint64 nOffset, qint64 nSize, XBinary::SCANID parentId, SpecAbstract::SCAN_OPTIONS *pOptions, XBinary::PDSTRUCT *pPdStruct)
{
    XBinary::PDSTRUCT pdStructEmpty={};

    if(!pPdStruct)
    {
        pPdStruct=&pdStructEmpty;
    }

    SpecAbstract::scan(pDevice,pScanResult,nOffset,nSize,parentId,pOptions,true,pPdStruct);
}

SpecAbstract::SCAN_RESULT StaticScan::scanFile(QString sFileName, XBinary::PDSTRUCT *pPdStruct)
{
    SpecAbstract::SCAN_RESULT result={0};

    if(sFileName!="")
    {
        QFile file;
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadOnly))
        {
            result=scanDevice(&file,pPdStruct);

            file.close();
        }
    }

    return result;
}

SpecAbstract::SCAN_RESULT StaticScan::scanDevice(QIODevice *pDevice, XBinary::PDSTRUCT *pPdStruct)
{
    SpecAbstract::SCAN_RESULT result={0};

    XBinary::SCANID parentId={0};
    parentId.fileType=XBinary::FT_UNKNOWN;
    parentId.filePart=XBinary::FILEPART_HEADER;
    _process(pDevice,&result,0,pDevice->size(),parentId,g_pOptions,pPdStruct);

    return result;
}

SpecAbstract::SCAN_RESULT StaticScan::scanMemory(char *pData, qint32 nSize, XBinary::PDSTRUCT *pPdStruct)
{
    SpecAbstract::SCAN_RESULT result={0};

    QBuffer buffer;

    buffer.setData(pData,nSize);

    if(buffer.open(QIODevice::ReadOnly))
    {
        result=scanDevice(&buffer,pPdStruct);

        buffer.close();
    }

    return result;
}
