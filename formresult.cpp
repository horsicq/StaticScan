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
#include "formresult.h"
#include "ui_formresult.h"

FormResult::FormResult(QWidget *pParent) :
    QWidget(pParent),
    ui(new Ui::FormResult)
{
    ui->setupUi(this);
    g_pModel=0;

    g_scanResult={0};
}

FormResult::~FormResult()
{
    delete ui;
}

void FormResult::setData(SpecAbstract::SCAN_RESULT scanResult, QString sSaveFileName)
{
    this->g_scanResult=scanResult;
    this->g_sSaveFileName=sSaveFileName;

    ui->labelElapsedTime->clear();

    QAbstractItemModel *pOldModel=ui->treeViewResult->model();

    g_pModel=new StaticScanItemModel(&(this->g_scanResult.listRecords),this,1);
    ui->treeViewResult->setModel(g_pModel);
    ui->treeViewResult->expandAll();

    delete pOldModel;

    ui->labelElapsedTime->setText(QString("%1 %2").arg(this->g_scanResult.nScanTime).arg(tr("msec")));
}

void FormResult::on_pushButtonClear_clicked()
{
    QAbstractItemModel *pOldModel=ui->treeViewResult->model();

    ui->treeViewResult->setModel(nullptr);

    delete pOldModel;

    ui->labelElapsedTime->clear();
}

void FormResult::on_pushButtonSave_clicked()
{
    QAbstractItemModel *pModel=ui->treeViewResult->model();

    if(pModel)
    {
        DialogStaticScanProcess::saveResult(this,(StaticScanItemModel *)pModel,g_sSaveFileName);
    }
}
