#include "formresult.h"
#include "ui_formresult.h"

FormResult::FormResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormResult)
{
    ui->setupUi(this);
    pModel=0;
}

FormResult::~FormResult()
{
    delete ui;
}

void FormResult::setData(SpecAbstract::SCAN_RESULT scanResult, QString sSaveFileName)
{
    this->scanResult=scanResult;
    this->sSaveFileName=sSaveFileName;

    if(pModel)
    {
        delete pModel;
    }

    pModel=new StaticScanItemModel(&(this->scanResult.listRecords),this,1);
    ui->treeViewResult->setModel(pModel);
    ui->treeViewResult->expandAll();

    ui->labelTime->setText(QString("%1 %2").arg(this->scanResult.nScanTime).arg(tr("msec")));
}

void FormResult::on_pushButtonClear_clicked()
{
    ui->treeViewResult->setModel(nullptr);
    ui->labelTime->clear();
}

void FormResult::on_pushButtonSave_clicked()
{
    QAbstractItemModel *pModel=ui->treeViewResult->model();

    if(pModel)
    {
        DialogStaticScan::saveResult(this,(StaticScanItemModel *)pModel,sSaveFileName);
    }
}
