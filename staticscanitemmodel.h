// Copyright (c) 2017-2021 hors<horsicq@gmail.com>
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
#ifndef STATICSCANITEMMODEL_H
#define STATICSCANITEMMODEL_H

#include <QAbstractItemModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QXmlStreamWriter>
#ifdef QT_GUI_LIB
#include <QApplication>
#include <QColor>
#include <QPalette>
#endif
#include "staticscanitem.h"

class StaticScanItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit StaticScanItemModel(QList<SpecAbstract::SCAN_STRUCT> *pListScanStructs,QObject *pParent=nullptr,int nNumberOfColumns=1);
    ~StaticScanItemModel() override;

    QVariant headerData(int nSection,Qt::Orientation orientation,int nRole=Qt::DisplayRole) const override;
    QModelIndex index(int nRow,int nColumn,const QModelIndex &parent=QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index,int nRole=Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QString toXML();
    QString toJSON();
    QString toCSV();
    QString toTSV();
    QString toFormattedString();
    QString toString(SpecAbstract::SCAN_OPTIONS *pScanOptions);
    StaticScanItem *rootItem();

private:
    void _toXML(QXmlStreamWriter *pXml,StaticScanItem *pItem);
    void _toJSON(QJsonObject *pJsonObject,StaticScanItem *pItem);
    void _toCSV(QString *pString,StaticScanItem *pItem);
    void _toTSV(QString *pString,StaticScanItem *pItem);
    void _toString(QString *pString,StaticScanItem *pItem,qint32 nLevel);

private:
    StaticScanItem *g_pRootItem;
};

#endif // STATICSCANITEMMODEL_H
