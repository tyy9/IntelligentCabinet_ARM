#ifndef KEYBORAD_H
#define KEYBORAD_H

#include <QWidget>

namespace Ui {
class KeyBorad;
}

class KeyBorad : public QWidget
{
    Q_OBJECT

public:
    explicit KeyBorad(QWidget *parent = nullptr);
    ~KeyBorad();

private:
    Ui::KeyBorad *ui;
};

#endif // KEYBORAD_H
