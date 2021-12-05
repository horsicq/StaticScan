/* Copyright (c) 2020-2021 hors<horsicq@gmail.com>
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
#ifndef HEURWIDGET_H
#define HEURWIDGET_H

#include "dialogstaticscanprocess.h"
#include "xshortcutswidget.h"

namespace Ui {
class HeurWidget;
}

class HeurWidget : public XShortcutsWidget
{
    Q_OBJECT

public:
    explicit HeurWidget(QWidget *pParent=nullptr);
    ~HeurWidget();
    void setData(QIODevice *pDevice,bool bAuto,XBinary::FT fileType);
    // mb TODO set sFileName

private slots:
    void on_pushButtonScan_clicked();
    void on_pushButtonSave_clicked();
    void scan();

protected:
    virtual void registerShortcuts(bool bState);

private:
    Ui::HeurWidget *ui;
    QIODevice *g_pDevice;
    XBinary::FT g_fileType;
};

#endif // HEURWIDGET_H
