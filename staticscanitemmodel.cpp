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
#include "staticscanitemmodel.h"

StaticScanItemModel::StaticScanItemModel(QList<SpecAbstract::SCAN_STRUCT> *pListScanStructs, QObject *pParent, int nNumberOfColumns)
    : QAbstractItemModel(pParent)
{
    g_pRootItem=new StaticScanItem(tr("Result"),nullptr,nNumberOfColumns);
    SpecAbstract::SCAN_STRUCT emptySS={};
    g_pRootItem->setScanStruct(emptySS);

    QMap<QString,StaticScanItem *> mapParents;

    qint32 nNumberOfDetects=pListScanStructs->count();

    for(qint32 i=0;i<nNumberOfDetects;i++)
    {
        if(!mapParents.contains(pListScanStructs->at(i).id.sUuid))
        {
            StaticScanItem *_itemParent;

            if(pListScanStructs->at(i).parentId.sUuid=="")
            {
                _itemParent=g_pRootItem;
            }
            else
            {
                _itemParent=mapParents.value(pListScanStructs->at(i).parentId.sUuid);
            }

            QString sParent=SpecAbstract::createTypeString(&pListScanStructs->at(i));

            StaticScanItem *pItemParent=new StaticScanItem(sParent,_itemParent,nNumberOfColumns);
            SpecAbstract::SCAN_STRUCT ss=SpecAbstract::createHeaderScanStruct(&pListScanStructs->at(i));
            pItemParent->setScanStruct(ss);
            _itemParent->appendChild(pItemParent);

            mapParents.insert(pListScanStructs->at(i).id.sUuid,pItemParent);
        }

        StaticScanItem *pItemParent=mapParents.value(pListScanStructs->at(i).id.sUuid);

        QString sItem=SpecAbstract::createResultString2(&pListScanStructs->at(i));
        StaticScanItem *pItem=new StaticScanItem(sItem,pItemParent,nNumberOfColumns);
        pItem->setScanStruct(pListScanStructs->at(i));
        pItemParent->appendChild(pItem);
    }
}

StaticScanItemModel::~StaticScanItemModel()
{
    delete g_pRootItem;
}

QVariant StaticScanItemModel::headerData(int nSection, Qt::Orientation orientation, int nRole) const
{
    QVariant result;

    if((orientation==Qt::Horizontal)&&(nRole==Qt::DisplayRole))
    {
        result=g_pRootItem->data(nSection);
    }

    return result;
}

QModelIndex StaticScanItemModel::index(int nRow, int nColumn, const QModelIndex &parent) const
{
    QModelIndex result;

    if(hasIndex(nRow, nColumn, parent))
    {
        StaticScanItem *pParentItem=nullptr;

        if(!parent.isValid())
        {
            pParentItem=g_pRootItem;
        }
        else
        {
            pParentItem=static_cast<StaticScanItem *>(parent.internalPointer());
        }

        StaticScanItem *childItem=pParentItem->child(nRow);

        if(childItem)
        {
            result=createIndex(nRow, nColumn, childItem);
        }
    }

    return result;
}

QModelIndex StaticScanItemModel::parent(const QModelIndex &index) const
{
    QModelIndex result;

    if(index.isValid())
    {
        StaticScanItem *pChildItem=static_cast<StaticScanItem *>(index.internalPointer());
        StaticScanItem *pParentItem=pChildItem->getParentItem();

        if(pParentItem!=g_pRootItem)
        {
            result=createIndex(pParentItem->row(),0,pParentItem);
        }
    }

    return result;
}

int StaticScanItemModel::rowCount(const QModelIndex &parent) const
{
    int nResult=0;

    if(parent.column()<=0)
    {
        StaticScanItem *pParentItem=nullptr;

        if(!parent.isValid())
        {
            pParentItem=g_pRootItem;
        }
        else
        {
            pParentItem=static_cast<StaticScanItem *>(parent.internalPointer());
        }

        nResult= pParentItem->childCount();
    }

    return nResult;
}

int StaticScanItemModel::columnCount(const QModelIndex &parent) const
{
    int nResult=0;

    if(parent.isValid())
    {
        nResult=static_cast<StaticScanItem *>(parent.internalPointer())->columnCount();
    }
    else
    {
        nResult=g_pRootItem->columnCount();
    }

    return nResult;
}

QVariant StaticScanItemModel::data(const QModelIndex &index, int nRole) const
{
    QVariant result;

    if(index.isValid())
    {
        StaticScanItem *pItem=static_cast<StaticScanItem *>(index.internalPointer());

        if(nRole==Qt::DisplayRole)
        {
            result=pItem->data(index.column());
        }
#ifdef QT_GUI_LIB
        else if(nRole==Qt::ForegroundRole)
        {
            SpecAbstract::RECORD_TYPE rt=pItem->scanStruct().type;

            // TODO more
            if(     (rt==SpecAbstract::RECORD_TYPE_INSTALLER)||
                    (rt==SpecAbstract::RECORD_TYPE_SFX))
            {
                result=QVariant(QColor(Qt::blue));
            }
            else if((rt==SpecAbstract::RECORD_TYPE_PROTECTOR)||
                    (rt==SpecAbstract::RECORD_TYPE_APKOBFUSCATOR)||
                    (rt==SpecAbstract::RECORD_TYPE_JAROBFUSCATOR)||
                    (rt==SpecAbstract::RECORD_TYPE_NETOBFUSCATOR)||
                    (rt==SpecAbstract::RECORD_TYPE_NETCOMPRESSOR)||
                    (rt==SpecAbstract::RECORD_TYPE_DONGLEPROTECTION)||
                    (rt==SpecAbstract::RECORD_TYPE_JOINER)||
                    (rt==SpecAbstract::RECORD_TYPE_PACKER))
            {
                result=QVariant(QColor(Qt::red));
            }
            else if((rt==SpecAbstract::RECORD_TYPE_PETOOL)||
                    (rt==SpecAbstract::RECORD_TYPE_APKTOOL))
            {
                result=QVariant(QColor(Qt::green));
            }
            else if((rt==SpecAbstract::RECORD_TYPE_OPERATIONSYSTEM)||
                    (rt==SpecAbstract::RECORD_TYPE_VIRTUALMACHINE))
            {
                result=QVariant(QColor(Qt::darkYellow));
            }
            else if(rt==SpecAbstract::RECORD_TYPE_SIGNTOOL)
            {
                result=QVariant(QColor(Qt::darkMagenta));
            }
            else if(rt==SpecAbstract::RECORD_TYPE_LANGUAGE)
            {
                result=QVariant(QColor(Qt::darkCyan));
            }
            else
            {
                result=QVariant(QApplication::palette().text().color());
            }
        }
#endif
    }

    return result;
}

Qt::ItemFlags StaticScanItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags result=Qt::NoItemFlags;

    if(index.isValid())
    {
        result=QAbstractItemModel::flags(index);
    }

    return result;
}

QString StaticScanItemModel::toXML()
{
    QString sResult;
    QXmlStreamWriter xml(&sResult);

    xml.setAutoFormatting(true);

    _toXML(&xml,g_pRootItem);

    return sResult;
}

QString StaticScanItemModel::toJSON()
{
    QString sResult;

    QJsonObject jsonResult;

    _toJSON(&jsonResult,g_pRootItem);

    QJsonDocument saveFormat(jsonResult);

    sResult=saveFormat.toJson(QJsonDocument::Indented).data();

    return sResult;
}

QString StaticScanItemModel::toCSV()
{
    QString sResult;

    _toCSV(&sResult,g_pRootItem);

    return sResult;
}

QString StaticScanItemModel::toTSV()
{
    QString sResult;

    _toTSV(&sResult,g_pRootItem);

    return sResult;
}

QString StaticScanItemModel::toFormattedString()
{
    QString sResult;

    _toString(&sResult,g_pRootItem,0);

    return sResult;
}

QString StaticScanItemModel::toString(SpecAbstract::SCAN_OPTIONS *pScanOptions)
{
    QString sResult;

    if(pScanOptions->bResultAsXML)
    {
        sResult=toXML();
    }
    else if(pScanOptions->bResultAsJSON)
    {
        sResult=toJSON();
    }
    else if(pScanOptions->bResultAsCSV)
    {
        sResult=toCSV();
    }
    else if(pScanOptions->bResultAsTSV)
    {
        sResult=toTSV();
    }
    else
    {
        sResult=toFormattedString();
    }

    return sResult;
}

StaticScanItem *StaticScanItemModel::rootItem()
{
    return this->g_pRootItem;
}

void StaticScanItemModel::_toXML(QXmlStreamWriter *pXml, StaticScanItem *pItem)
{
    if(pItem->childCount())
    {
        pXml->writeStartElement(pItem->data(0).toString());

        int nNumberOfChildren=pItem->childCount();

        for(qint32 i=0;i<nNumberOfChildren;i++)
        {
            _toXML(pXml,pItem->child(i));
        }

        pXml->writeEndElement();
    }
    else
    {
        SpecAbstract::SCAN_STRUCT ss=pItem->scanStruct();

        pXml->writeStartElement("detect");
        pXml->writeAttribute("type",SpecAbstract::recordTypeIdToString(ss.type));
        pXml->writeAttribute("name",SpecAbstract::recordNameIdToString(ss.name));
        pXml->writeAttribute("version",ss.sVersion);
        pXml->writeAttribute("info",ss.sInfo);
        pXml->writeCharacters(pItem->data(0).toString());
        pXml->writeEndElement();
    }
}

void StaticScanItemModel::_toJSON(QJsonObject *pJsonObject, StaticScanItem *pItem)
{
    if(pItem->childCount())
    {
        SpecAbstract::SCAN_STRUCT ss=pItem->scanStruct();

        QString sArrayName="detects";

        if(ss.id.filePart!=SpecAbstract::RECORD_FILEPART_UNKNOWN)
        {
            pJsonObject->insert("parentfilepart",SpecAbstract::recordFilePartIdToString(ss.parentId.filePart));
            pJsonObject->insert("filetype",XBinary::fileTypeIdToString(ss.id.fileType));

            sArrayName="values";
        }

        QJsonArray jsArray;

        int nNumberOfChildren=pItem->childCount();

        for(qint32 i=0;i<nNumberOfChildren;i++)
        {
            QJsonObject jsRecord;

            _toJSON(&jsRecord,pItem->child(i));

            jsArray.append(jsRecord);
        }

        pJsonObject->insert(sArrayName,jsArray);
    }
    else
    {
        SpecAbstract::SCAN_STRUCT ss=pItem->scanStruct();

        pJsonObject->insert("type",SpecAbstract::recordTypeIdToString(ss.type));
        pJsonObject->insert("name",SpecAbstract::recordNameIdToString(ss.name));
        pJsonObject->insert("version",ss.sVersion);
        pJsonObject->insert("info",ss.sInfo);
        pJsonObject->insert("string",pItem->data(0).toString());
    }
}

void StaticScanItemModel::_toCSV(QString *pString, StaticScanItem *pItem)
{
    if(pItem->childCount())
    {
        int nNumberOfChildren=pItem->childCount();

        for(qint32 i=0;i<nNumberOfChildren;i++)
        {
            _toCSV(pString,pItem->child(i));
        }
    }
    else
    {
        SpecAbstract::SCAN_STRUCT ss=pItem->scanStruct();

        QString sResult=QString("%1;%2;%3;%4;%5\n")
                        .arg(SpecAbstract::recordTypeIdToString(ss.type),
                             SpecAbstract::recordNameIdToString(ss.name),
                             ss.sVersion,
                             ss.sInfo,
                             pItem->data(0).toString());

        pString->append(sResult);
    }
}

void StaticScanItemModel::_toTSV(QString *pString, StaticScanItem *pItem)
{
    if(pItem->childCount())
    {
        int nNumberOfChildren=pItem->childCount();

        for(qint32 i=0;i<nNumberOfChildren;i++)
        {
            _toTSV(pString,pItem->child(i));
        }
    }
    else
    {
        SpecAbstract::SCAN_STRUCT ss=pItem->scanStruct();

        QString sResult=QString("%1\t%2\t%3\t%4\t%5\n").arg(
                                SpecAbstract::recordTypeIdToString(ss.type),
                                SpecAbstract::recordNameIdToString(ss.name),
                                ss.sVersion,
                                ss.sInfo,
                                pItem->data(0).toString()
                    );

        pString->append(sResult);
    }
}

void StaticScanItemModel::_toString(QString *pString, StaticScanItem *pItem, int nLevel)
{
    if(nLevel)
    {
        QString sResult;
        sResult=sResult.leftJustified(4*(nLevel-1),' '); // TODO function
        sResult.append(QString("%1\n").arg(pItem->data(0).toString()));
        pString->append(sResult);
    }

    if(pItem->childCount())
    {
        qint32 nNumberOfChildren=pItem->childCount();

        for(qint32 i=0;i<nNumberOfChildren;i++)
        {
            _toString(pString,pItem->child(i),nLevel+1);
        }
    }
}
