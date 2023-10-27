#ifndef TABLEINFO_H
#define TABLEINFO_H

#include<QString>
class TableInfo
{
public:
    TableInfo();
    void setId(int id);
    void setTname(QString tName);
    void setOrderId(int OrderId);
    void setState(int state);
    void setCardid(QString cardid);
    QString getTname();
    int getState();
    int getOrderId();
    QString getcardid();
    int getId();
    QString toString();
private:
    int id;
    QString tName;
    int OrderId;
    int state;
    QString cardid;
};

#endif // TABLEINFO_H
