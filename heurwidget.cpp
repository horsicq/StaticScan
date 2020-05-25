// copyright (c) 2020 hors<horsicq@gmail.com>
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

HeurWidget::HeurWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeurWidget)
{
    ui->setupUi(this);

    ui->checkBoxDeepScan->setChecked(true);
    ui->checkBoxRecursiveScan->setChecked(true);
    ui->checkBoxHeuristicScan->setChecked(false);
}

HeurWidget::~HeurWidget()
{
    delete ui;
}

void HeurWidget::setData(QIODevice *pDevice, bool bAuto)
{
    this->pDevice=pDevice;

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
    options.bShowHeuristic=true;

    DialogStaticScan ds(this);
    ds.setData(pDevice,&options,&scanResult);
    ds.exec();

    QAbstractItemModel *pOldTreeModel=ui->treeViewScan->model();

    StaticScanItemModel *pModel=new StaticScanItemModel(&(scanResult.listRecords),this,1);
    ui->treeViewScan->setModel(pModel);
    ui->treeViewScan->expandAll();

    delete pOldTreeModel;

    int nNumberOfHeurs=scanResult.listHeurs.count();

    QAbstractItemModel *pOldTableModel=ui->tableViewHeur->model();

    QStandardItemModel *pHeurModel=new QStandardItemModel(nNumberOfHeurs,1,this);

    ui->tableViewHeur->setModel(pHeurModel);

    delete pOldTableModel;

    // mb TODO scan time
}
