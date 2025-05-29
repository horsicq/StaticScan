/* Copyright (c) 2019-2025 hors<horsicq@gmail.com>
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
#ifndef FORMSTATICSCAN_H
#define FORMSTATICSCAN_H

#include <QWidget>

#include "dialogstaticscan.h"
#include "formresult.h"

namespace Ui {
class FormStaticScan;
}

class FormStaticScan : public QWidget {
    Q_OBJECT

public:
    struct OPTIONS {
        bool bHideRecursive;
        bool bScanAfterOpen;
        bool bIsImage;
    };

    explicit FormStaticScan(QWidget *parent = nullptr);
    ~FormStaticScan();
    void setData(QIODevice *pDevice, OPTIONS *pOptions);

private slots:
    void on_pushButtonScan_clicked();
    void scan();

private:
    Ui::FormStaticScan *ui;
    QIODevice *pDevice;
    OPTIONS *pOptions;
};

#endif  // FORMSTATICSCAN_H
