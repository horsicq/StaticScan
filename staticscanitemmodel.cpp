// copyright (c) 2017-2018 hors<horsicq@gmail.com>
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

StaticScanItemModel::StaticScanItemModel(QList<SpecAbstract::SCAN_STRUCT> *pListDetects,QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem=new StaticScanItem("result");

    QMap<QString,StaticScanItem *> mapParents;

    for(int i=0; i<pListDetects->count(); i++)
    {
        if(!mapParents.contains(pListDetects->at(i).id.uuid.toString()))
        {
            StaticScanItem *_itemParent;
            if(pListDetects->at(i).parentId.uuid=="")
            {
                _itemParent=rootItem;
            }
            else
            {
                _itemParent=mapParents.value(pListDetects->at(i).parentId.uuid.toString());
            }

            QString sParent=SpecAbstract::createTypeString(&pListDetects->at(i));

            StaticScanItem *itemParent=new StaticScanItem(sParent,_itemParent);
            _itemParent->appendChild(itemParent);

            mapParents.insert(pListDetects->at(i).id.uuid.toString(),itemParent);
        }

        StaticScanItem *itemParent=mapParents.value(pListDetects->at(i).id.uuid.toString());

        QString sItem=SpecAbstract::createResultString2(&pListDetects->at(i));
        StaticScanItem *item=new StaticScanItem(sItem,itemParent);
        item->setScanStruct(pListDetects->at(i));
        itemParent->appendChild(item);
    }
}

StaticScanItemModel::~StaticScanItemModel()
{
   delete rootItem;
}

QVariant StaticScanItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex StaticScanItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    StaticScanItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<StaticScanItem *>(parent.internalPointer());

    StaticScanItem *childItem=parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex StaticScanItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    StaticScanItem *childItem = static_cast<StaticScanItem *>(index.internalPointer());
    StaticScanItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int StaticScanItemModel::rowCount(const QModelIndex &parent) const
{
    StaticScanItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<StaticScanItem *>(parent.internalPointer());

    return parentItem->childCount();
}

int StaticScanItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<StaticScanItem *>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant StaticScanItemModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if(index.isValid())
    {
        StaticScanItem *item = static_cast<StaticScanItem *>(index.internalPointer());
        if(role==Qt::DisplayRole)
        {
            result=item->data(index.column());
        }
#ifdef QT_GUI_LIB
        else if(role==Qt::ForegroundRole)
        {
            SpecAbstract::RECORD_TYPES rt=item->scanStruct().type;

            if(         (rt==SpecAbstract::RECORD_TYPE_INSTALLER)||
                        (rt==SpecAbstract::RECORD_TYPE_SFX))
            {
                result=QVariant(QColor(Qt::blue));
            }
            else if(    (rt==SpecAbstract::RECORD_TYPE_PROTECTOR)||
                        (rt==SpecAbstract::RECORD_TYPE_NETOBFUSCATOR)||
                        (rt==SpecAbstract::RECORD_TYPE_PACKER))
            {
                result=QVariant(QColor(Qt::red));
            }
            else
            {
                result=QVariant(QColor(Qt::black));
            }
        }
#endif
    }

    return result;
}

Qt::ItemFlags StaticScanItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QString StaticScanItemModel::toXML()
{
    QString sResult;
    QXmlStreamWriter xml(&sResult);

    xml.setAutoFormatting(true);

    _toXML(&xml,rootItem);

    return sResult;
}

QString StaticScanItemModel::toFormattedString()
{
    QString sResult;

    _toString(&sResult,rootItem,0);

    return sResult;
}

QString StaticScanItemModel::toString(SpecAbstract::SCAN_OPTIONS *pScanOptions)
{
    QString sResult;
    if(pScanOptions->bResultAsXML)
    {
        sResult=toXML();
    }
    else
    {
        sResult=toFormattedString();
    }

    return sResult;
}

void StaticScanItemModel::_toXML(QXmlStreamWriter *pXml, StaticScanItem *item)
{
    if(item->childCount())
    {
        pXml->writeStartElement(item->data(0).toString());

        for(int i=0;i<item->childCount();i++)
        {
            _toXML(pXml,item->child(i));
        }

        pXml->writeEndElement();
    }
    else
    {
        SpecAbstract::SCAN_STRUCT ss=item->scanStruct();

        pXml->writeStartElement("detect");
        pXml->writeAttribute("type",SpecAbstract::recordTypeIdToString(ss.type));
        pXml->writeAttribute("name",SpecAbstract::recordNameIdToString(ss.name));
        pXml->writeAttribute("version",ss.sVersion);
        pXml->writeAttribute("info",ss.sInfo);
        pXml->writeCharacters(item->data(0).toString());
        pXml->writeEndElement();
    } 
}

void StaticScanItemModel::_toString(QString *pString, StaticScanItem *item, int nLevel)
{
    if(nLevel)
    {
        QString sResult;
        sResult=sResult.leftJustified(4*(nLevel-1),' ');
        sResult.append(QString("%1\n").arg(item->data(0).toString()));
        pString->append(sResult);
    }

    if(item->childCount())
    {
        for(int i=0;i<item->childCount();i++)
        {
            _toString(pString,item->child(i),nLevel+1);
        }
    }


}
