#include "simplechat.h"
#include "ui_simplechat.h"
#include <QString>
#include <QDebug>

simplechat::simplechat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::simplechat)
{
    /*初始化连接器*/
    Cconnect *_con = new Cconnect;
    this->set_con(_con);
    qDebug() << "[success] client init";
    ui->setupUi(this);
    sleep(1);

    connect(this, SIGNAL(sig_reg(char*)), this, SLOT(registeret(char*)));
    connect(this, SIGNAL(sig_login(char*)), this, SLOT(loginret(char*)));
    connect(this, SIGNAL(sig_list(char*)), this, SLOT(listret(char*)));
    connect(this, SIGNAL(sig_recv(char*)), this, SLOT(recvmsg(char*)));

    std::thread mq_appointment(&simplechat::mq_recv, this);
    mq_appointment.detach();
    qDebug() << "分发线程已分离";
    std::thread mq_send_t(&simplechat::mq_send, this);
    mq_send_t.detach();
    qDebug() << "只读线程已分离";

    QTimer *timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(timer_timeout()));
    timer->start(500);
}

simplechat::~simplechat()
{
    delete this->con;
    delete ui;
}
void simplechat::set_con(Cconnect *_con)
{
    this->con = _con;
}

void mq_send(Cconnect *con)
{
    con->send();
}

void simplechat::mq_send()
{
    this->con->send();
}

void simplechat::timer_timeout()
{
    ui->label->setText(QString(this->_msg.c_str()));
}

//分发
void simplechat::mq_recv()
{
    while(1)
    {
        if(sem_wait(&this->con->sem1) < 0)
            perror("");
        char *buf = new char[1500];
        memset((void*)buf, 0, 1500);
        memcpy((void*)buf, (void*)this->con->BUF, this->con->BUFLEN);

        qDebug() << "recv a message from buf";
        /*for(int i = 0; i < this->con->BUFLEN; i ++)
            printf(" %d", buf[i]);
        printf("\n");*/

        protocol pro;
        memset((void*)&pro, 0, sizeof(protocol));
        memcpy((void*)&pro, (void*)buf, sizeof(protocol));
        if(pro.data_type == MESSAGE && pro.msg_type & SEND_MSG)
        {
            qDebug() << "message";
            emit sig_recv(buf);
        }
        else if(pro.data_type == ACCOUNT_INFO && pro.msg_type & REGISTER_ACCOUNT)
        {
            qDebug() << "register";
            emit sig_reg(buf);
        }
        else if(pro.data_type == ACCOUNT_INFO && pro.msg_type & LOGIN_ACCOUNT)
        {
            qDebug() << "login";
            emit sig_login(buf);
        }
        else if(pro.data_type == ACCOUNT_INFO && pro.msg_type & LIST_ONLINE)
        {
            qDebug() << "list";
            emit sig_list(buf);
        }
        else
        {
            qDebug() << "unknow message type";
        }
        sleep(1);
        memset((void*)this->con->BUF, 0, sizeof(this->con->BUF));
        this->con->BUFLEN = 0;
        if(sem_post(&this->con->sem2) < 0)
            perror("");
        qDebug() << "once over";
    }
}

/*登录*/
void simplechat::on_pushButton_clicked()
{
    /*连接服务器*/
    if(this->con->link(nullptr) == false)
        return;
    /*读取到帐号和密码进行验证*/
    QString account = ui->lineEdit->text();
    if(this->con->login(account.toStdString().c_str(), account.toStdString().c_str()) == false)
        return;
}
/*登录返回*/
void simplechat::loginret(char *buf)
{
    account_info acc;
    memcpy((void*)&(acc), (void*)(buf+sizeof(protocol)), sizeof(account_info));
    std::string res(buf+sizeof(protocol)+sizeof(account_info));
    qDebug() << acc.id;
    qDebug() << acc.name;
    qDebug() << res.c_str();

    if( std::string(this->con->acnt.id) == std::string(acc.id) || std::string(this->con->acnt.name) == std::string(acc.name) )
    {
        if(res.find("success") != std::string::npos)
        {
            memcpy((void*)&this->con->acnt, (void*)&acc, sizeof(account_info));
            /*将本机信息设置到标签*/
            char tmp[64] = {};
            sprintf(tmp, "%s\n%s", this->con->acnt.id, this->con->acnt.name);
            ui->label_2->setText(QString(tmp));
            /*登录成功后将页码设置为消息页面*/
            ui->stackedWidget->setCurrentIndex(1);
            this->update_msg();
            qDebug() << "[success] login success";
            //登录成功后需要获取当前在线用户以更新对话列表
            this->con->list_online();

            /*登录成功获取历史消息*/
            read_history_message();
        }
    }
    delete[] buf;
}
void simplechat::read_help(std::string title)
{
    std::cout << "title: " << title << std::endl;
    std::string acc_info = title.substr(0, title.find(":"));

    int i = 0;
    for(; i < ui->listWidget->count(); i ++)
    {
        /*如果有相同的*/
        if(ui->listWidget->item(i)->text() == QString(title.c_str()))
        {
            this->curr = ui->listWidget->item(i);
            this->con->cu = this->con->msg_list.find(acc_info);
            return;
        }
    }
    QListWidgetItem *_item = new QListWidgetItem(QString(title.c_str()));
    ui->listWidget->addItem(_item);
    this->curr = _item;
    this->contact = _item->text().toStdString();
    this->con->msg_list.insert(std::pair<std::string, std::vector<msg*>>(acc_info, std::vector<msg*>()));
    this->update_ctact();
}
/*获取历史消息*/
void simplechat::read_history_message()
{
    std::string msgpath = MSG_PATH;
    msgpath += this->con->acnt.id;
    std::cout << msgpath << std::endl;
    std::ifstream ifs(msgpath, std::fstream::in);
    if(!ifs.is_open())
    {
        std::cout << "open local message file failed" << std::endl;
        return;
    }

    while(!ifs.eof())
    {
        char __msg[1300] = {};
        ifs.getline(__msg, sizeof(__msg));

        std::string title(__msg);
        if(title.find('[') != std::string::npos)
        {
            if(ifs.eof())
                break;

            title = title.substr(1, title.size()-2);
            this->read_help(title);
            continue;
        }
        else
        {
            if(ifs.eof())
                break;

            msg *_msg = new msg;
            memset((void*)_msg, 0, sizeof(msg));

            /*读取一条消息*/
            strncpy(_msg->date, __msg, strlen(__msg));
            ifs.getline(__msg, sizeof(__msg));
            strncpy(_msg->receiver, __msg, strlen(__msg));
            ifs.getline(__msg, sizeof(__msg));
            strncpy(_msg->sender, __msg, strlen(__msg));
            ifs.getline(__msg, sizeof(__msg));
            strncpy(_msg->cont, __msg, strlen(__msg));

            if(!this->curr || this->con->cu == this->con->msg_list.end())
                continue;
            (this->con->cu->second).push_back(_msg);
        }
    }
    this->update_msg();
    std::cout << "read over" << std::endl;
}

/*去往注册*/
void simplechat::on_pushButton_2_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}

/*注册*/
void simplechat::on_pushButton_4_clicked()
{
    /*连接服务器*/
    if(this->con->link(nullptr) == false)
        return;
    QString account = ui->lineEdit_2->text();
    if(this->con->registered(account.toStdString().c_str(), account.toStdString().c_str()) == false)
        return;
}
/*注册返回*/
void simplechat::registeret(char *buf)
{
    std::string _tmp(this->con->acnt.name);
    memcpy((void*)&(this->con->acnt), (void*)(buf+sizeof(protocol)), sizeof(account_info));
    std::string res(buf+sizeof(protocol)+sizeof(account_info));
    qDebug() << this->con->acnt.id;
    qDebug() << this->con->acnt.name;
    qDebug() << res.c_str();

    if(std::string(this->con->acnt.name) == _tmp)
    {
        if(res.find("success") != std::string::npos)
        {
            /*将注册成功后的帐号自动填充*/
            ui->lineEdit->setText(QString(this->con->acnt.id));
            /*注册成功后将页码设置为登录页面*/
            ui->stackedWidget_2->setCurrentIndex(0);
            qDebug() << "[success] registered success";
        }
    }
    delete[] buf;
}

/*登出*/
void simplechat::on_pushButton_3_clicked()
{
    /*将消息写入本地文件*/
    this->write_history_message();
    this->con->quit();
    this->curr = nullptr;
    this->_msg = "";
    for(int i = 0; i < ui->listWidget->count(); i ++)
    {
        delete ui->listWidget->takeItem(0);
    }
    ui->stackedWidget->setCurrentIndex(0);
}
/*写入历史消息*/
void simplechat::write_history_message()
{
    std::string msgpath = MSG_PATH;
    msgpath += this->con->acnt.id;
    std::cout << msgpath << std::endl;
    std::ofstream ofs(msgpath, std::fstream::out);
    if(!ofs.is_open())
    {
        std::cout << "open local message file failed" << std::endl;
        return;
    }
    for(int i = 0; i < ui->listWidget->count(); i ++)
    {
        std::string title = ui->listWidget->item(i)->text().toStdString();
        std::string _id = title.substr(0, title.find(':'));
        std::cout << _id << std::endl;
        auto itm = this->con->msg_list.find(_id);
        if(itm == this->con->msg_list.end())
            continue;

        /*填充头部*/
        ofs << '[' << title << ']' << std::endl;
        for(auto itv = (itm->second).begin(); itv != (itm->second).end(); itv ++)
        {
            ofs << (*itv)->date << std::endl;
            ofs << (*itv)->receiver << std::endl;
            ofs << (*itv)->sender << std::endl;
            ofs << (*itv)->cont << std::endl;
        }
    }
    std::cout << "write over" << std::endl;
}

/*发送消息*/
void simplechat::on_pushButton_10_clicked()
{
    QString content = ui->lineEdit_3->text();
    ui->lineEdit_3->setText("");
    if(content.size() == 0)
        return;
    if(!this->curr)
        return;

    msg *_msg = new msg;
    memset((void*)_msg, 0, sizeof(msg));

    time_t t;
    time(&t);
    std::string time = time_to_date(t);
    memcpy((void*)_msg->date, (void*)time.c_str(), time.size());
    std::string recv = this->con->cu->first;
    memcpy((void*)_msg->receiver, (void*)recv.c_str(), recv.size());
    std::string sender(this->con->acnt.id);
    memcpy((void*)_msg->sender, (void*)sender.c_str(), sender.size());
    memcpy((void*)_msg->cont, (void*)content.toStdString().c_str(), content.size());
    this->con->send_msg(_msg, sizeof(msg));
    this->update_msg();
}

/*接收消息*/
void simplechat::recvmsg(char *buf)
{
    protocol pro;
    int len = sizeof(protocol);
    memset((void*)&pro, 0, len);
    memcpy((void*)&pro, (void*)buf, len);
    if(pro.data_type == MESSAGE && pro.msg_type & SEND_MSG)
    {
        msg *_msg = new msg;
        memset((void*)_msg, 0, sizeof(msg));
        memcpy((void*)_msg, (void*)(buf+len), sizeof(msg));
        qDebug() << _msg->date;
        qDebug() << _msg->receiver;
        qDebug() << _msg->sender;
        qDebug() << _msg->cont;

        /*判断发送者是否在对话列表*/
        if(this->con->msg_list.find(std::string(_msg->sender)) != this->con->msg_list.end())
        {
            qDebug() << "on contact list";
            /*如果发送者是当前对话人*/
            if(std::string(_msg->sender) == this->con->cu->first)
                this->con->cu->second.push_back(_msg);
            else
            {
                auto it = this->con->msg_list.find(std::string(_msg->sender));
                it->second.push_back(_msg);
            }
        }
        /*否则就将发送者添加至对话列表*/
        else
        {
            qDebug() << "not on contact list";
            this->con->msg_list.insert(std::pair<std::string, std::vector<msg*>>(std::string(_msg->sender), std::vector<msg*>()));
            auto _it = this->con->msg_list.find(std::string(_msg->sender));
            _it->second.push_back(_msg);
            this->con->cu = _it;

            std::string _con;
            /*根据在线用户列表获取用户名等信息*/
            auto it = this->con->online_list.find(std::string(_msg->sender));
            if(it == this->con->online_list.end())
                _con = _msg->sender;
            else
            {
                _con += it->first;
                _con += ':';
                _con += it->second;
            }

            QListWidgetItem *_item = new QListWidgetItem(QString(_con.c_str()));
            ui->listWidget->addItem(_item);
            this->curr = _item;
            this->contact = _item->text().toStdString();
        }
        for(auto itm = this->con->msg_list.begin(); itm != this->con->msg_list.end(); itm ++)
        {
            std::cout << "-----------" << std::endl;
            for(auto itv = itm->second.begin(); itv != itm->second.end(); itv ++)
            {
                std::cout << (*itv)->cont << std::endl;
            }
        }
        this->update_msg();
    }
    delete[] buf;
    qDebug() << "got a message";
}

/*更新对话框内容*/
void simplechat::update_msg()
{
    if(!this->curr)
        return;
    ui->label->clear();
    this->_msg = "";
    if(this->con->cu == this->con->msg_list.end())
    {
        //如果迭代器有问题, 将内容置空
        ui->label->setText(QString(this->_msg.c_str()));
        return;
    }
    std::cout << "curr num: " << this->con->cu->first << std::endl;
    std::vector<msg*> _vec;
    int i = 0;
    for(auto it = (this->con->cu->second).rbegin(); it != (this->con->cu->second).rend(); it ++, i ++)
    {
        if(i < 4)
        {
            _vec.push_back(*it);
        }
    }

    std::cout << "msg size: " << _vec.size() << std::endl;
    for(auto it = _vec.rbegin(); it != _vec.rend(); it ++)
    {
        this->_msg += (*it)->date;
        this->_msg += ':';
        this->_msg += (*it)->sender;
        this->_msg += '\n';
        this->_msg += (*it)->cont;
        this->_msg += '\n';
    }
    ui->label->setText(QString(this->_msg.c_str()));
}

/*更新对话人*/
void simplechat::update_ctact()
{
    if(!this->curr)
    {
        qDebug() << "[failed] fetch current contact user";
        return;
    }
    qDebug() << this->curr->text();
    /*获取当前联系人的最新对话消息*/
    std::string acc_info = this->curr->text().toStdString();
    acc_info = acc_info.substr(0, acc_info.find(":"));

    if(this->con->msg_list.find(acc_info) == this->con->msg_list.end())
    {
        qDebug() << "[failed] fetch current message";
        return;
    }
    this->contact = acc_info;
    this->con->cu = this->con->msg_list.find(this->contact);
    this->con->cu_msg = this->con->cu->second.rbegin();
    std::cout << "curr user: " << this->con->cu->first << std::endl;
}

/*消息*/
void simplechat::on_pushButton_5_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
    this->update_ctact();
    this->update_msg();
}

/*联系人*/
void simplechat::on_pushButton_6_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
    if(!this->con->list_online())
        return;
}
/*联系人返回*/
void simplechat::listret(char *buf)
{
    protocol pro;
    int len = sizeof(protocol);
    memset((void*)&pro, 0, sizeof(protocol));
    memcpy((void*)&pro, buf, sizeof(protocol));
    //读取用户信息
    acc user;
    int data_len = sizeof(acc);
    int size = pro.msg_len;
    qDebug() << size;
    for(int i = 0; i < size; i ++)
    {
        memset((void*)&user, 0, data_len);
        memcpy((void*)&user, buf+len, data_len);
        if(this->con->online_list.find(std::string(user.id)) == this->con->online_list.end())
            this->con->online_list.insert(std::pair<std::string, std::string>(std::string(user.id), std::string(user.name)));
        len += data_len;
    }
    /*先将所有列删除, toconfirm*/
    for(int i = 0; i < ui->stackedWidget_3->count(); i ++)
        delete ui->listWidget_2->takeItem(0);

    /*将在线用户添加到联系人列表*/
    for(auto it = this->con->online_list.begin(); it != this->con->online_list.end(); it ++)
    {
        char tmp[32] = {};
        std::cout << it->first << ' ' << it->second << std::endl;
        sprintf(tmp, "%s:%s", it->first.c_str(), it->second.c_str());
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString(tmp));
        ui->listWidget_2->addItem(item);
    }
    qDebug() << "[success] fetch online user list success";
    delete[] buf;
}

/*我的*/
void simplechat::on_pushButton_7_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(2);
}

/*联系人跳转对话框*/
void simplechat::on_listWidget_2_clicked(const QModelIndex &index)
{
    QListWidgetItem *item = ui->listWidget_2->currentItem();
    std::string acc_info = item->text().toStdString();
    acc_info = acc_info.substr(0, acc_info.find(":"));

    int i = 0;
    for(; i < ui->listWidget->count(); i ++)
    {
        /*如果有相同的*/
        if(ui->listWidget->item(i)->text() == item->text())
        {
            this->curr = ui->listWidget->item(i);
            this->con->cu = this->con->msg_list.find(acc_info);
            ui->stackedWidget_3->setCurrentIndex(0);
            return;
        }
    }
    QListWidgetItem *_item = new QListWidgetItem(QString(item->text()));
    ui->listWidget->addItem(_item);
    this->curr = _item;
    this->contact = _item->text().toStdString();
    this->con->msg_list.insert(std::pair<std::string, std::vector<msg*>>(acc_info, std::vector<msg*>()));
    this->update_ctact();
    this->update_msg();
    ui->stackedWidget_3->setCurrentIndex(0);
}

/*对话列表*/
void simplechat::on_listWidget_clicked(const QModelIndex &index)
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if(!item)
    {
        qDebug() << "no contact user";
        return;
    }
    this->curr = item;
    this->update_ctact();
    this->update_msg();
}

/*消息向前翻页-*/
void simplechat::on_pushButton_11_clicked()
{
    return;
    if(!this->curr || this->con->cu == this->con->msg_list.end())
        return;
    for(int i = 0; this->con->cu_msg != this->con->cu->second.rend(); this->con->cu_msg ++, i ++)
    {
        if(i == 4)
            break;
    }
    std::cout << (*this->con->cu_msg)->cont << std::endl;
}

/*消息向后翻页+*/
void simplechat::on_pushButton_12_clicked()
{
    return;
    if(!this->curr || this->con->cu == this->con->msg_list.end())
        return;
    for(int i = 0; this->con->cu_msg != this->con->cu->second.rbegin(); this->con->cu_msg --, i ++)
    {
        if(i == 4)
            break;
    }
    std::cout << (*this->con->cu_msg)->cont << std::endl;
}
