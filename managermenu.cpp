#include "managermenu.h"
#include "ui_managermenu.h"
#include <QThread>
ManagerMenu::ManagerMenu(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::ManagerMenu)
{
    ui->setupUi(this);
    GetTableData();
    tableAddDialog = NULL;
    // 设置列数
    ui->TableList->setColumnCount(5);
    ui->TableList->setHorizontalHeaderLabels(QStringList() << "id"
                                                           << "t_name"
                                                           << "OrderId"
                                                           << "state"
                                                           << "card_id");
    ui->TableList->setRowCount(100);
    connect(&manager_getTableInfo, SIGNAL(finished(QNetworkReply *)), this, SLOT(GetTableDataReply(QNetworkReply *)));
    connect(&manager_UpdateTableInfo, SIGNAL(finished(QNetworkReply *)), this, SLOT(UpdateTableInfoReply(QNetworkReply *)));
    connect(&manager_deleteTableInfo, SIGNAL(finished(QNetworkReply *)), this, SLOT(DeleteTableInfoReply(QNetworkReply *)));
}

ManagerMenu::~ManagerMenu()
{
    qDebug()<<"ManagerMenu close";
    foreach (auto tableinfo, tableinfo_v)
    {
        delete tableinfo;
    }
    delete ui;
}
void ManagerMenu::GetTableData()
{

    // 获取url地址
    QString url_str = QString("http://192.168.63.168:8001/myintelligentcabinet/usr-order/findAll");
    // 创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // get请求获取图片
    manager_getTableInfo.get(request);

}
void ManagerMenu::GetTableDataReply(QNetworkReply *reply)
{
    // 先清空容器
    foreach (auto tableinfo, tableinfo_v)
    {
        delete tableinfo;
    }
    tableinfo_v.clear();
    //------------
    auto Info = reply->readAll();
    qDebug() << Info;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString tableinfo = QString::fromUtf8(Info);
    // CodeInfo.replace("\\n","\n");
    qDebug().noquote() << tableinfo;
    // QMessageBox::information(this,"提示","订单号已记录");
    // 解析json数据
    // 遍历获取json数据
    // 解析JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(Info);
    if (!jsonDoc.isNull())
    {
        QJsonObject jsonObj = jsonDoc.object();

        // 获取"data"对象
        QJsonObject dataObj = jsonObj["data"].toObject();

        // 获取"table_list"数组
        QJsonArray tableList = dataObj["table_list"].toArray();

        // 遍历每个对象
        for (const QJsonValue &value : tableList)
        {
            QJsonObject tableObj = value.toObject();
            int id = tableObj["id"].toInt();
            int state = tableObj["state"].toInt();
            QString tname = tableObj["tname"].toString();
            QJsonValue orderIdValue = tableObj["orderId"];
            int orderId = orderIdValue.toInt();
            QString cardid = tableObj["cardId"].toString();
            TableInfo *tableinfo = new TableInfo();
            tableinfo->setTname(tname);
            tableinfo->setId(id);
            tableinfo->setOrderId(orderId);
            tableinfo->setState(state);
            tableinfo->setCardid(cardid);
            // tableinfo->toString();
            tableinfo_v.push_back(tableinfo);
        }
    }
    int i = 0;
    foreach (auto tableinfo, tableinfo_v)
    {
        ui->TableList->setItem(i, 0, new QTableWidgetItem(QString::number(tableinfo->getId())));
        ui->TableList->setItem(i, 1, new QTableWidgetItem(tableinfo->getTname()));
        ui->TableList->setItem(i, 2, new QTableWidgetItem(QString::number(tableinfo->getOrderId())));
        ui->TableList->setItem(i, 3, new QTableWidgetItem(QString::number(tableinfo->getState())));
        ui->TableList->setItem(i, 4, new QTableWidgetItem(tableinfo->getcardid()));
        i++;
    }
    reply->abort();
    reply->deleteLater();
}
void ManagerMenu::refreshTableData()
{
    // 先清空表格数据
    ui->TableList->clearContents();
    GetTableData();
//    // 销毁对话框内存
//    if (tableAddDialog != NULL)
//    {
//        delete tableAddDialog;
//        tableAddDialog = NULL;
//    }
}
void ManagerMenu::on_btnAdd_pressed()
{
    tableAddDialog = new TableAddDialog(this);
    tableAddDialog->setGeometry(0, 0, 640, 330);
    connect(tableAddDialog, SIGNAL(ReturnToMenu()), this, SLOT(refreshTableData()));
    tableAddDialog->show();
}

void ManagerMenu::on_TableList_cellClicked(int row, int column)
{
    // 记录可能要更新的行
    int flag = 0;
    // 防止更新无效数据
    if (!ui->TableList->item(row, 0))
    {
        qDebug() << "error";
        flag = 1;
    }
    foreach (int r, update_row)
    {
        if (r == row)
        {
            flag = 1;
        }
    }
    // 防止重复更新相同的行数据
    if (flag != 1)
    {
        qDebug() << row;
        update_row.push_back(row);
    }
}

void ManagerMenu::on_btnUpdate_pressed()
{
    // 获取容器中各行的数据，保存到类中并发送请求
    foreach (int row, update_row)
    {
        qDebug()<<"on_btnUpdate_pressed--row:"<<row;
        QVector<QTableWidgetItem *> tableitem_v;
        qDebug()<<1;
        for (int i = 0; i < ui->TableList->columnCount(); i++)
        {
            auto item = ui->TableList->item(row, i);
            tableitem_v.push_back(item);
        }
        qDebug()<<3;
        TableInfo *tableinfo = new TableInfo();
        // 多行更新
        tableinfo->setId((tableitem_v.at(0)->text()).toInt());
        tableinfo->setTname(tableitem_v.at(1)->text());
        tableinfo->setOrderId((tableitem_v.at(2)->text()).toInt());
        tableinfo->setState((tableitem_v.at(3)->text()).toInt());
        tableinfo->setCardid(tableitem_v.at(4)->text());
        tableinfo->toString();
        qDebug()<<2;
        UpdateTableData(tableinfo);
    }
    update_row.clear(); // 清除所要更新的行数据
    QMessageBox::information(this, "提示", "更新完毕");
    QThread::sleep(1);
    refreshTableData();
}
void ManagerMenu::UpdateTableData(TableInfo *tableinfo)
{
    // 获取url地址
    QString url_str = QString("http://192.168.63.168:8001/myintelligentcabinet/table-info/updateTable");
    // 创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    // 把appcode和服务器主机域名添加到请求对象中
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject jsonObj;
    jsonObj["id"] = tableinfo->getId();
    jsonObj["tname"] = tableinfo->getTname();
    jsonObj["state"] = tableinfo->getState();
    jsonObj["orderId"] = tableinfo->getOrderId();
    jsonObj["cardId"] = tableinfo->getcardid();
    QJsonDocument jdoc(jsonObj);
    manager_UpdateTableInfo.put(request, jdoc.toJson(QJsonDocument::Compact));

}
void ManagerMenu::UpdateTableInfoReply(QNetworkReply *reply)
{
    auto Info = reply->readAll();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    auto UpdateInfo = codec->toUnicode(Info);
    UpdateInfo.replace("\\n", "\n");
    QJsonDocument jdoc = QJsonDocument::fromJson(UpdateInfo.toUtf8());
    int code = 20000;
    if (!jdoc.isNull())
    {
        QJsonObject jsonObj = jdoc.object();
        code = jsonObj["code"].toInt();
    }
    if (code == 20001)
    {
        qDebug() << "update rowdata success!";
    }
    else
    {
        QMessageBox::information(this, "提示", "更新失败");
    }
    reply->abort();
    reply->deleteLater();
}

// 支持多行删除
void ManagerMenu::on_btnDelete_pressed()
{
    // 获取容器中各行的数据，保存到类中并发送请求
    foreach (int row, delete_row)
    {
        QVector<QTableWidgetItem *> tableitem_v;
        for (int i = 0; i < ui->TableList->columnCount(); i++)
        {
            auto item = ui->TableList->item(row, i);
            tableitem_v.push_back(item);
        }
        TableInfo *tableinfo = new TableInfo();
        // 多行更新
        tableinfo->setId((tableitem_v.at(0)->text()).toInt());
        tableinfo->setTname(tableitem_v.at(1)->text());
        tableinfo->setOrderId((tableitem_v.at(2)->text()).toInt());
        tableinfo->setState((tableitem_v.at(3)->text()).toInt());
        tableinfo->setCardid(tableitem_v.at(4)->text());
        tableinfo->toString();
        DeleteTableData(tableinfo);
    }
    delete_row.clear(); // 清除所要更新的行数据
    update_row.clear();//防止更新已删除的行数据
    QMessageBox::information(this, "提示", "删除完毕");
    QThread::sleep(1);
    refreshTableData();
}
void ManagerMenu::DeleteTableData(TableInfo *tableinfo)
{
    // 获取url地址
    QString url_str = QString("http://192.168.63.168:8001/myintelligentcabinet/table-info/deleteTable/%1")
                          .arg(tableinfo->getId());
    // 创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    // 把appcode和服务器主机域名添加到请求对象中
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    manager_deleteTableInfo.deleteResource(request);

}
void ManagerMenu::DeleteTableInfoReply(QNetworkReply *reply)
{
    auto Info = reply->readAll();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    auto UpdateInfo = codec->toUnicode(Info);
    UpdateInfo.replace("\\n", "\n");
    QJsonDocument jdoc = QJsonDocument::fromJson(UpdateInfo.toUtf8());
    int code = 20000;
    if (!jdoc.isNull())
    {
        QJsonObject jsonObj = jdoc.object();
        code = jsonObj["code"].toInt();
        qDebug()<<"code:"<<code;
    }
    if (code == 20001)
    {
        qDebug() << "delete rowdata success!";
    }
    else
    {
        QMessageBox::information(this, "提示", "删除失败");
    }
    reply->abort();
    reply->deleteLater();
}

void ManagerMenu::on_TableList_itemSelectionChanged()
{
    delete_row.clear();
    // 获取被多选中的行数据
    QList<QTableWidgetItem *> items = ui->TableList->selectedItems();
    // 获取行值
    foreach (auto item, items)
    {
        int flag=0;
        auto row = item->row();
        //防止放入重复行数
        foreach(int item,delete_row){
            if(item==row){
                flag=1;
            }
        }
        // 防止删除无用数据
        if (ui->TableList->item(row, item->column()))
        {
            if(flag==0){
                qDebug()<<"row:"<<row;
                delete_row.push_back(row);
            }
        }
    }
}


void ManagerMenu::on_btnReturn_pressed()
{
    //获取上一个界面
    QWidget *w=this->parentWidget();
    w->show();
    emit ReturnSignal();
    delete this;
}

