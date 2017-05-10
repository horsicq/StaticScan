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
#include "staticscan.h"

StaticScan::StaticScan(QObject *parent) : QObject(parent)
{
    bIsStop=false;
}

void StaticScan::setData(QString sFileName, SpecAbstract::SCAN_OPTIONS *pOptions,QList<SpecAbstract::SCAN_STRUCT> *pListResult)
{
    this->sFileName=sFileName;
    this->pOptions=pOptions;
    this->pListResult=pListResult;
}

void StaticScan::process()
{
    bIsStop=false;

    QFile file;

    if(sFileName!="")
    {
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadOnly))
        {
            QElapsedTimer timer;
            timer.start();
            SpecAbstract::ID parentId;
            parentId.nOffset=-1;
            parentId.filetype=SpecAbstract::RECORD_FILETYPE_UNKNOWN;

            _process(&file,pListResult,0,parentId);

            emit completed(timer.elapsed());

            file.close();
        }
    }
}

void StaticScan::stop()
{
    bIsStop=true;
}

QList<SpecAbstract::SCAN_STRUCT> StaticScan::process(QString sFileName, SpecAbstract::SCAN_OPTIONS *pOptions)
{
    QList<SpecAbstract::SCAN_STRUCT> listResult;
    StaticScan scan;
    scan.setData(sFileName,pOptions,&listResult);
    scan.process();

    return listResult;
}

QList<SpecAbstract::SCAN_STRUCT> StaticScan::process(QIODevice *pDevice, SpecAbstract::SCAN_OPTIONS *pOptions)
{
    Q_UNUSED(pOptions);
    QList<SpecAbstract::SCAN_STRUCT> listResult;
    StaticScan scan;
    SpecAbstract::ID parentId;
    parentId.nOffset=-1;
    parentId.filetype=SpecAbstract::RECORD_FILETYPE_UNKNOWN;
    scan._process(pDevice,&listResult,0,parentId);

    return listResult;
}

void StaticScan::_process(QIODevice *pDevice,QList<SpecAbstract::SCAN_STRUCT> *pList,qint64 nStartOffset,SpecAbstract::ID parentId)
{
    Q_UNUSED(nStartOffset);
    Q_UNUSED(parentId);
    QSet<QBinary::FILE_TYPES> setTypes=QBinary::getFileTypes(pDevice);

    if(setTypes.contains(QBinary::FILE_TYPE_BINARY))
    {
        //        SpecAbstract::SCAN_STRUCT record={0};

        //        record.id.filetype=SpecAbstract::RECORD_FILETYPE_BINARY;
        //        record.id.nOffset=nStartOffset;
        //        record.nSize=pDevice->size();
        //        record.parentId=parentId;
        //        record.type=SpecAbstract::RECORD_TYPE_UNKNOWN;
        //        record.name=SpecAbstract::RECORD_NAME_UNKNOWN;
        //        record.sVersion="";
        //        record.parentId=parentId;

        //        pList->append(record);

    }

    if(setTypes.contains(QBinary::FILE_TYPE_PE32)||setTypes.contains(QBinary::FILE_TYPE_PE64))
    {
        SpecAbstract::PEINFO_STRUCT pe_info=SpecAbstract::getPEInfo(pDevice);

        pList->append(pe_info.listDetects);
    }
}




