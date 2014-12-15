#ifndef EDITMODEL_H
#define EDITMODEL_H

#include <QWidget>
#include <vle/gvle2/vlevpz.h>

namespace Ui {
class editModel;
}

class EditModel : public QWidget
{
    Q_OBJECT
    
public:
    explicit EditModel(QWidget *parent = 0);
    ~EditModel();
    void setModel(vleVpzModel *model);
    
private:
    Ui::editModel *ui;
    vleVpzModel   *mModel;
};

#endif // EDITMODEL_H
