#ifndef EXPORTHISDATA_H
#define EXPORTHISDATA_H

#include <QWidget>

namespace Ui {
class ExportHisData;
}

class ExportHisData : public QWidget
{
    Q_OBJECT
    
public:
    explicit ExportHisData(QWidget *parent = 0);
    ~ExportHisData();
    
private slots:
    void on_pbExit_clicked();

private:
    Ui::ExportHisData *ui;
};

#endif // EXPORTHISDATA_H
