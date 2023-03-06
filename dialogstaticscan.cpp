/* Copyright (c) 2017-2023 hors<horsicq@gmail.com>
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
#include "dialogstaticscan.h"

#include "ui_dialogstaticscan.h"

DialogStaticScan::DialogStaticScan(QWidget *pParent) : QDialog(pParent), ui(new Ui::DialogStaticScan)
{
    ui->setupUi(this);

    g_pDevice = nullptr;

    setWindowFlags(Qt::Window);
}

DialogStaticScan::~DialogStaticScan()
{
    delete ui;
}

void DialogStaticScan::setData(QIODevice *pDevice, bool bAuto)
{
    this->g_pDevice = pDevice;

    if (bAuto) {
        scan();
    }
}

void DialogStaticScan::on_pushButtonClose_clicked()
{
    this->close();
}

void DialogStaticScan::on_pushButtonScan_clicked()
{
    scan();
}

void DialogStaticScan::scan()
{
    SpecAbstract::SCAN_RESULT scanResult = {0};

    SpecAbstract::SCAN_OPTIONS options = {0};

    options.bRecursiveScan = ui->checkBoxRecursiveScan->isChecked();
    options.bDeepScan = ui->checkBoxDeepScan->isChecked();
    options.bHeuristicScan = ui->checkBoxHeuristicScan->isChecked();
    options.bVerbose = ui->checkBoxVerbose->isChecked();
    options.bAllTypesScan = ui->checkBoxAllTypesScan->isChecked();

    DialogStaticScanProcess ds(XOptions::getMainWidget(this));
    ds.setData(g_pDevice, &options, &scanResult);
    ds.showDialogDelay();  // TODO const

    QString sSaveFileName = XBinary::getResultFileName(g_pDevice, QString("%1.txt").arg(tr("Result")));
    ui->widgetResult->setData(scanResult, sSaveFileName);
}
