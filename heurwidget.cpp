/* Copyright (c) 2020-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "heurwidget.h"

#include "ui_heurwidget.h"

HeurWidget::HeurWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::HeurWidget)
{
    ui->setupUi(this);

    this->g_pDevice = nullptr;
    this->g_fileType = XBinary::FT_UNKNOWN;

    ui->checkBoxDeepScan->setChecked(true);
    ui->checkBoxRecursiveScan->setChecked(true);
    ui->checkBoxHeuristicScan->setChecked(false);
    ui->checkBoxVerbose->setChecked(false);
    ui->checkBoxAllTypesScan->setChecked(false);
}

HeurWidget::~HeurWidget()
{
    delete ui;
}

void HeurWidget::setData(QIODevice *pDevice, bool bAuto, XBinary::FT fileType)
{
    this->g_pDevice = pDevice;
    this->g_fileType = fileType;

    XFormats::setFileTypeComboBox(fileType, pDevice, ui->comboBoxType);

    if (bAuto) {
        scan();
    }
}

void HeurWidget::on_pushButtonScan_clicked()
{
    scan();
}

void HeurWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void HeurWidget::on_pushButtonSave_clicked()
{
    QAbstractItemModel *pModel = ui->treeViewScan->model();

    if (pModel) {
        QString sSaveFileName = XBinary::getResultFileName(g_pDevice, QString("%1.txt").arg(tr("Result")));

        DialogStaticScanProcess::saveResult(this, (ScanItemModel *)pModel, sSaveFileName);
    }
}

void HeurWidget::on_comboBoxType_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    scan();
}

void HeurWidget::scan()
{
    SpecAbstract::SCAN_RESULT scanResult = {0};
    SpecAbstract::SCAN_OPTIONS options = {0};

    options.bRecursiveScan = ui->checkBoxRecursiveScan->isChecked();
    options.bDeepScan = ui->checkBoxDeepScan->isChecked();
    options.bHeuristicScan = ui->checkBoxHeuristicScan->isChecked();
    options.bVerbose = ui->checkBoxVerbose->isChecked();
    options.bAllTypesScan = ui->checkBoxAllTypesScan->isChecked();
    options.bShowDetects = true;
    options.fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());

    DialogStaticScanProcess dialogStaticScanProcess(this);
    dialogStaticScanProcess.setData(g_pDevice, &options, &scanResult);
    dialogStaticScanProcess.showDialogDelay(1000);

    QAbstractItemModel *pOldTreeModel = ui->treeViewScan->model();

    QList<XBinary::SCANSTRUCT> _listRecords = SpecAbstract::convert(&(scanResult.listRecords));

    ScanItemModel *pModel = new ScanItemModel(&_listRecords, 1);
    ui->treeViewScan->setModel(pModel);
    ui->treeViewScan->expandAll();

    deleteOldAbstractModel(&pOldTreeModel);

    qint32 nNumberOfHeurs = scanResult.listHeurs.count();

    QAbstractItemModel *pOldTableModel = ui->tableViewHeur->model();

    QStandardItemModel *pHeurModel = new QStandardItemModel(nNumberOfHeurs, 3, this);

    pHeurModel->setHeaderData(0, Qt::Horizontal, tr("Type"));
    pHeurModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    pHeurModel->setHeaderData(2, Qt::Horizontal, tr("Value"));

    for (qint32 i = 0; i < nNumberOfHeurs; i++) {
        QStandardItem *pItemHeurType = new QStandardItem;
        pItemHeurType->setText(SpecAbstract::heurTypeIdToString(scanResult.listHeurs.at(i).detectType));
        pHeurModel->setItem(i, 0, pItemHeurType);

        QStandardItem *pItemName = new QStandardItem;
        pItemName->setText(
            QString("%1(%2)[%3]")
                .arg(SpecAbstract::recordNameIdToString(scanResult.listHeurs.at(i).name), scanResult.listHeurs.at(i).sVersion, scanResult.listHeurs.at(i).sInfo));
        pHeurModel->setItem(i, 1, pItemName);

        QStandardItem *pItemValue = new QStandardItem;
        pItemValue->setText(scanResult.listHeurs.at(i).sValue);
        pHeurModel->setItem(i, 2, pItemValue);
    }

    ui->tableViewHeur->setModel(pHeurModel);

    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    deleteOldAbstractModel(&pOldTableModel);

    // mb TODO scan time
}
