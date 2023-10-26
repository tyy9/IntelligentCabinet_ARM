#include "tableinfo.h"
#include<QDebug>
TableInfo::TableInfo()
{

}
void TableInfo::setId(int id){
    this->id=id;
}
void TableInfo::setTname(QString tName){
    this->tName=tName;
}
void TableInfo::setOrderId(int OrderId){
    this->OrderId=OrderId;
}
void TableInfo::setState(int state){
    this->state=state;
}
void TableInfo::setCardid(QString cardid){
    this->cardid=cardid;
}
QString TableInfo::getTname(){
    return tName;
}
int TableInfo::getState(){
    return state;
}
QString TableInfo::toString(){
    qDebug()<<"id:"<<id<<"tName:"<<tName<<"OrderId:"<<OrderId<<"state:"<<state;
}
