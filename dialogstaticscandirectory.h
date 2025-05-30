/* Copyright (c) 2018-2025 hors<horsicq@gmail.com>
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
#ifndef DIALOGSTATICSCANDIRECTORY_H
#define DIALOGSTATICSCANDIRECTORY_H

#include <QDialog>
#include <QFileDialog>

#include "dialogstaticscanprocess.h"
#include "xoptions.h"
#include "xshortcutsdialog.h"

namespace Ui {
class DialogStaticScanDirectory;
}

class DialogStaticScanDirectory : public XShortcutsDialog {
    Q_OBJECT

public:
    explicit DialogStaticScanDirectory(QWidget *pParent, const QString &sDirName);
    ~DialogStaticScanDirectory();

private slots:
    void on_pushButtonOpenDirectory_clicked();
    void on_pushButtonScan_clicked();
    void scanDirectory(const QString &sDirectoryName);
    void scanResult(XScanEngine::SCAN_RESULT scanResult);
    void appendResult(const QString &sResult);
    void on_pushButtonOK_clicked();
    void on_pushButtonClear_clicked();
    void on_pushButtonSave_clicked();

signals:
    void resultSignal(QString sText);

private:
    Ui::DialogStaticScanDirectory *ui;
    SpecAbstract::SCAN_OPTIONS g_scanOptions;
};

#endif  // DIALOGSTATICSCANDIRECTORY_H
