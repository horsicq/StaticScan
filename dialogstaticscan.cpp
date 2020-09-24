// copyright (c) 2017-2020 hors<horsicq@gmail.com>
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
#include "dialogstaticscan.h"
#include "ui_dialogstaticscan.h"

DialogStaticScan::DialogStaticScan(QWidget *pParent, QIODevice *pDevice) :
    QDialog(pParent),
    ui(new Ui::DialogStaticScan)
{
    ui->setupUi(this);

    this->pDevice=pDevice;
}

DialogStaticScan::~DialogStaticScan()
{
    delete ui;
}

void DialogStaticScan::on_pushButtonClose_clicked()
{
    this->close();
}

void DialogStaticScan::on_pushButtonScan_clicked()
{
    SpecAbstract::SCAN_RESULT scanResult={0};

    SpecAbstract::SCAN_OPTIONS options={0};

    options.bRecursiveScan=ui->checkBoxRecursiveScan->isChecked();
    options.bDeepScan=ui->checkBoxDeepScan->isChecked();
    options.bHeuristicScan=ui->checkBoxHeuristicScan->isChecked();

    DialogStaticScanProcess ds(this);
    ds.setData(pDevice,&options,&scanResult);
    ds.exec();

    QString sSaveDirectory=XBinary::getDeviceFilePath(pDevice)+QDir::separator()+"result"; // mb TODO
    ui->widgetResult->setData(scanResult,sSaveDirectory);
}
