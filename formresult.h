#ifndef FORMRESULT_H
#define FORMRESULT_H

#include <QWidget>

#include "dialogstaticscan.h"

namespace Ui {
class FormResult;
}

class FormResult : public QWidget
{
    Q_OBJECT

public:
    explicit FormResult(QWidget *parent = nullptr);
    ~FormResult();
    void setData(SpecAbstract::SCAN_RESULT scanResult, QString sSaveFileName);

private slots:
    void on_pushButtonClear_clicked();
    void on_pushButtonSave_clicked();

private:
    Ui::FormResult *ui;
    SpecAbstract::SCAN_RESULT scanResult;
    QString sSaveFileName;
};

#endif // FORMRESULT_H
