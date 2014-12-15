#ifndef EDITMODEL_H
#define EDITMODEL_H

#include <QWidget>

namespace Ui {
class EditModel;
}

class EditModel : public QWidget
{
    Q_OBJECT
    
public:
    explicit EditModel(QWidget *parent = 0);
    ~EditModel();
    
private:
    Ui::EditModel *ui;
};

#endif // EDITMODEL_H
