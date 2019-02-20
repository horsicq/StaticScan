#include "formresult.h"
#include "ui_formresult.h"

FormResult::FormResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormResult)
{
    ui->setupUi(this);
}

FormResult::~FormResult()
{
    delete ui;
}

void FormResult::setData(SpecAbstract::SCAN_RESULT *pScanResult, QString sSaveFileName)
{
    scanResult=*pScanResult;
    this->sSaveFileName=sSaveFileName;

    StaticScanItemModel *model=new StaticScanItemModel(&scanResult.listRecords);
    ui->treeViewResult->setModel(model);
    ui->treeViewResult->expandAll();

    ui->labelTime->setText(QString("%1 %2").arg(scanResult.nScanTime).arg(tr("msec")));
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
        QString sFilter;
        sFilter+=QString("%1 (*.txt)").arg(tr("Text documents"));
        QString sFileName=QFileDialog::getSaveFileName(this,tr("Save result"),sSaveFileName,sFilter);

        if(!sFileName.isEmpty())
        {
            QFile file;
            file.setFileName(sFileName);

            if(file.open(QIODevice::ReadWrite))
            {
                QString sText=((StaticScanItemModel *)pModel)->toFormattedString();

                file.write(sText.toLatin1().data());

                file.close();
            }
        }
    }
}
