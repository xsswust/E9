#ifndef HISDATASEARCH_H
#define HISDATASEARCH_H

#include <QWidget>

namespace Ui {
class HisDataSearch;
}

class HisDataSearch : public QWidget
{
    Q_OBJECT
    
public:
    explicit HisDataSearch(QWidget *parent = 0);
    ~HisDataSearch();
    
private slots:
    void on_pbExit_clicked();

private:
    Ui::HisDataSearch *ui;
};

#endif // HISDATASEARCH_H
