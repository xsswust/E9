#ifndef PARATABLE_H
#define PARATABLE_H

#include <QWidget>

namespace Ui {
class ParaTable;
}

class ParaTable : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParaTable(QWidget *parent = 0);
    ~ParaTable();
    void readCurrentCalirationParameter();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::ParaTable *ui;
};
extern ParaTable *pParaTable;
#endif // PARATABLE_H
