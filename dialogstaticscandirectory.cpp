/* Copyright (c) 2018-2022 hors<horsicq@gmail.com>
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
#include "dialogstaticscandirectory.h"

#include "ui_dialogstaticscandirectory.h"

DialogStaticScanDirectory::DialogStaticScanDirectory(QWidget *pParent, QString sDirName) : XShortcutsDialog(pParent), ui(new Ui::DialogStaticScanDirectory) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);  // Qt::WindowTitleHint

    connect(this, SIGNAL(resultSignal(QString)), this, SLOT(appendResult(QString)));

    ui->checkBoxScanSubdirectories->setChecked(true);

    if (sDirName != "") {
        ui->lineEditDirectoryName->setText(sDirName);
    }
}

DialogStaticScanDirectory::~DialogStaticScanDirectory() {
    delete ui;
}

void DialogStaticScanDirectory::on_pushButtonOpenDirectory_clicked() {
    QString sInitDirectory = ui->lineEditDirectoryName->text();

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory") + QString("..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditDirectoryName->setText(QDir().toNativeSeparators(sDirectoryName));
    }
}

void DialogStaticScanDirectory::on_pushButtonScan_clicked() {
    QString sDirectoryName = ui->lineEditDirectoryName->text().trimmed();

    scanDirectory(sDirectoryName);

    getGlobalOptions()->setLastDirectory(sDirectoryName);
}

void DialogStaticScanDirectory::scanDirectory(QString sDirectoryName) {
    if (sDirectoryName != "") {
        ui->textBrowserResult->clear();

        SpecAbstract::SCAN_OPTIONS options = {0};
        options.bRecursiveScan = ui->checkBoxRecursiveScan->isChecked();
        options.bDeepScan = ui->checkBoxDeepScan->isChecked();
        options.bHeuristicScan = ui->checkBoxHeuristicScan->isChecked();
        options.bVerbose = ui->checkBoxVerbose->isChecked();
        options.bSubdirectories = ui->checkBoxScanSubdirectories->isChecked();
        options.bAllTypesScan = ui->checkBoxAllTypesScan->isChecked();
        // TODO Filter options
        // |flags|x all|

        DialogStaticScanProcess ds(this);
        connect(&ds, SIGNAL(scanResult(SpecAbstract::SCAN_RESULT)), this, SLOT(scanResult(SpecAbstract::SCAN_RESULT)), Qt::DirectConnection);
        ds.setData(sDirectoryName, &options);
        ds.showDialogDelay(1000);
    }
}

void DialogStaticScanDirectory::scanResult(SpecAbstract::SCAN_RESULT scanResult) {
    QString sResult = QString("%1 %2 %3").arg(QDir().toNativeSeparators(scanResult.sFileName), QString::number(scanResult.nScanTime), tr("msec"));
    sResult += "\r\n";  // TODO Linux

    QList<XBinary::SCANSTRUCT> _listRecords = SpecAbstract::convert(&(scanResult.listRecords));

    ScanItemModel model(&_listRecords);

    sResult += model.toString(XBinary::FORMATTYPE_TEXT).toUtf8().data();

    emit resultSignal(sResult);
}

void DialogStaticScanDirectory::appendResult(QString sResult) {
    ui->textBrowserResult->append(sResult);
}

void DialogStaticScanDirectory::on_pushButtonOK_clicked() {
    this->close();
}

void DialogStaticScanDirectory::on_pushButtonClear_clicked() {
    ui->textBrowserResult->clear();
}

void DialogStaticScanDirectory::on_pushButtonSave_clicked() {
    QString sFilter;
    sFilter += QString("%1 (*.txt)").arg(tr("Text documents"));
    QString sSaveFileName = ui->lineEditDirectoryName->text() + QDir::separator() + "result";
    QString sFileName = QFileDialog::getSaveFileName(this, tr("Save result"), sSaveFileName, sFilter);

    if (!sFileName.isEmpty()) {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadWrite)) {
            QString sText = ui->textBrowserResult->toPlainText();
            file.write(sText.toUtf8().data());

            file.close();
        }
    }
}
