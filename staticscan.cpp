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
            parentId.filetype=SpecAbstract::RECORD_FILETYPE_UNKNOWN;
            parentId.filepart=SpecAbstract::RECORD_FILEPART_HEADER;
            _process(&file,pListResult,0,file.size(),parentId,pOptions);

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

QString StaticScan::getEngineVersion()
{
    return SSE_VERSION;
}

void StaticScan::_process(QIODevice *pDevice,QList<SpecAbstract::SCAN_STRUCT> *pList,qint64 nOffset,qint64 nSize,SpecAbstract::ID parentId,SpecAbstract::SCAN_OPTIONS *pOptions)
{
    SubDevice sd(pDevice,nOffset,nSize);

    if(sd.open(QIODevice::ReadOnly))
    {
        QSet<QBinary::FILE_TYPES> stTypes=QBinary::getFileTypes(&sd);

        if(stTypes.contains(QBinary::FILE_TYPE_PE32)||stTypes.contains(QBinary::FILE_TYPE_PE64))
        {
            SpecAbstract::PEINFO_STRUCT pe_info=SpecAbstract::getPEInfo(&sd,parentId,pOptions);

            pList->append(pe_info.basic_info.listDetects);

            if(pOptions->bScanOverlay)
            {
                if(pe_info.nOverlaySize)
                {
                    SpecAbstract::ID _parentId=pe_info.basic_info.id;
                    _parentId.filepart=SpecAbstract::RECORD_FILEPART_OVERLAY;
                    _process(pDevice,pList,pe_info.nOverlayOffset,pe_info.nOverlaySize,_parentId,pOptions);
                }
            }
        }
        else if(stTypes.contains(QBinary::FILE_TYPE_ELF32)||stTypes.contains(QBinary::FILE_TYPE_ELF64))
        {
            SpecAbstract::ELFINFO_STRUCT elf_info=SpecAbstract::getELFInfo(&sd,parentId,pOptions);

            pList->append(elf_info.basic_info.listDetects);
        }
        else if(stTypes.contains(QBinary::FILE_TYPE_MSDOS))
        {
            SpecAbstract::MSDOSINFO_STRUCT msdos_info=SpecAbstract::getMSDOSInfo(&sd,parentId,pOptions);

            pList->append(msdos_info.basic_info.listDetects);
        }
        else
        {
            SpecAbstract::BINARYINFO_STRUCT binary_info=SpecAbstract::getBinaryInfo(&sd,parentId,pOptions);

            pList->append(binary_info.basic_info.listDetects);
        }

        sd.close();
    }

}




