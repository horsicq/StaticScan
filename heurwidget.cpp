// copyright (c) 2020-2021 hors<horsicq@gmail.com>
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
#include "heurwidget.h"
#include "ui_heurwidget.h"

HeurWidget::HeurWidget(QWidget *pParent) :
    QWidget(pParent),
    ui(new Ui::HeurWidget)
{
    ui->setupUi(this);

    this->g_pParent=pParent;

    ui->checkBoxDeepScan->setChecked(true);
    ui->checkBoxRecursiveScan->setChecked(true);
    ui->checkBoxHeuristicScan->setChecked(false);
}

HeurWidget::~HeurWidget()
{
    delete ui;
}

void HeurWidget::setData(QIODevice *pDevice, bool bAuto, XBinary::FT fileType, QWidget *pParent)
{
    this->g_pDevice=pDevice;
    this->g_fileType=fileType;
    this->g_pParent=pParent;

    if(bAuto)
    {
        scan();
    }
}

void HeurWidget::on_pushButtonScan_clicked()
{
    scan();
}

void HeurWidget::scan()
{
    SpecAbstract::SCAN_RESULT scanResult={0};
    SpecAbstract::SCAN_OPTIONS options={0};

    options.bRecursiveScan=ui->checkBoxRecursiveScan->isChecked();
    options.bDeepScan=ui->checkBoxDeepScan->isChecked();
    options.bHeuristicScan=ui->checkBoxHeuristicScan->isChecked();
    options.bShowDetects=true;
    options.fileType=g_fileType;

    DialogStaticScanProcess ds(g_pParent);
    ds.setData(g_pDevice,&options,&scanResult);
    ds.exec();

    QAbstractItemModel *pOldTreeModel=ui->treeViewScan->model();

    StaticScanItemModel *pModel=new StaticScanItemModel(&(scanResult.listRecords),this,1);
    ui->treeViewScan->setModel(pModel);
    ui->treeViewScan->expandAll();

    delete pOldTreeModel;

    int nNumberOfHeurs=scanResult.listHeurs.count();

    QAbstractItemModel *pOldTableModel=ui->tableViewHeur->model();

    QStandardItemModel *pHeurModel=new QStandardItemModel(nNumberOfHeurs,3,this);

    pHeurModel->setHeaderData(0,Qt::Horizontal,tr("Type"));
    pHeurModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
    pHeurModel->setHeaderData(2,Qt::Horizontal,tr("Value"));

    for(int i=0;i<nNumberOfHeurs;i++)
    {
        QStandardItem *pHeurType=new QStandardItem;
        pHeurType->setText(SpecAbstract::heurTypeIdToString(scanResult.listHeurs.at(i).detectType));
        pHeurModel->setItem(i,0,pHeurType);

        QStandardItem *pName=new QStandardItem;
        pName->setText(QString("%1(%2)[%3]").arg(SpecAbstract::recordNameIdToString(scanResult.listHeurs.at(i).name)).arg(scanResult.listHeurs.at(i).sVersion).arg(scanResult.listHeurs.at(i).sInfo));
        pHeurModel->setItem(i,1,pName);

        QStandardItem *pValue=new QStandardItem;
        pValue->setText(scanResult.listHeurs.at(i).sValue);
        pHeurModel->setItem(i,2,pValue);
    }

    ui->tableViewHeur->setModel(pHeurModel);

    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Interactive);
    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);

    delete pOldTableModel;

    // mb TODO scan time
}
