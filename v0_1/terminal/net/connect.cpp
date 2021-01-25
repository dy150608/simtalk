#include "connect.hpp"

Cconnect::Cconnect()
{
    qDebug() << "client constructor running";
    this->init();
    this->link(nullptr);
    sem_init(&this->sem1, 0, 0);
    sem_init(&this->sem2, 0, 1);
}

Cconnect::~Cconnect()
{
    this->quit();
    if(sem_destroy(&this->sem1) < 0)
        perror("");
    if(sem_destroy(&this->sem2) < 0)
        perror("");
}

void Cconnect::init()
{
    /*初始化一些数据*/
    memset((void*)&this->acnt, 0, sizeof(account_info));

    std::ifstream fin(SYSTEM, std::ios::in);
    if(fin.is_open() == false)
    {
        qDebug() << "[failed] fetch server ip";
        return;
    }
    while(fin.eof() == false)
    {
        std::string tmp;
        getline(fin, tmp);
        if(tmp.find("ipv4") != std::string::npos)
        {
            std::string _tmp = tmp.substr(tmp.find('=')+1);
            qDebug() << _tmp.c_str();
            strncpy(this->ip_str, _tmp.c_str(), _tmp.size());
            inet_pton(AF_INET, _tmp.c_str(), (void*)&aim_ip);
            qDebug() << "[success] fetch server address";
            return;
        }
    }
    qDebug() << "[failed] fetch server address";
}

bool Cconnect::heart_beats(char *target_ip)
{
    cout << target_ip << endl;
    //发送心跳包检测连线情况 todo
    DEBUG("heart beat datagram send succeed.");
    return true;
}

bool Cconnect::link(char *target_ip)
{
    if(this->tcp_fd != 0)
    {
        qDebug() << "already link to server";
        return true;
    }

    this->tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->tcp_fd < 0)
    {
        qDebug() << "[failed] link init";
        return false;
    }

    sockaddr_in sock;
	sock.sin_family = AF_INET;
	sock.sin_port	= htons(SERVER_PORT);
    if(target_ip != nullptr)
        sock.sin_addr.s_addr = inet_addr(target_ip);
    else
        sock.sin_addr.s_addr = this->aim_ip;

    if(connect(this->tcp_fd, (sockaddr*)&sock, sizeof(sockaddr_in)) < 0)
    {
        close(this->tcp_fd);
        qDebug() << "[failed] link server";
        return false;
    }
    qDebug() << "[success] link server";
    this->is_link = true;
	return true;
}

void Cconnect::quit()
{
    if(this->is_link == false || this->tcp_fd == 0)
        return;

    protocol pro;
    /*协议消息为帐号注销*/
    pro.data_type = ACCOUNT_INFO;
    pro.msg_type  = LOGOUT_ACCOUNT;
    pro.msg_len   = sizeof(account_info);

    char buf[1500] = {};
    size_t len = sizeof(protocol);
    /*填充协议*/
    memcpy((void*)buf, (const void*)&pro, len);
    /*填充帐号类型消息*/
    memcpy((void*)(buf+len), (const void*)&this->acnt, sizeof(account_info));
    len += sizeof(account_info);
    pro.msg_len = len - sizeof(protocol);

    /*将帐号信息发送到服务器，不管服务器有没有收到都需要退出*/
    write(this->tcp_fd, buf, len);

    /*释放消息*/
    for(auto itm = msg_list.begin(); itm != msg_list.end(); itm ++)
    {
        for(auto itv = itm->second.begin(); itv != itm->second.end(); itv ++)
            delete *itv;
        itm->second.clear();
    }
    /*清理遗留对话列表*/
    this->msg_list.clear();
    this->cu = this->msg_list.end();
}

bool Cconnect::login(const char *acc, const char *pwd)
{
    if(this->is_link == false || this->tcp_fd == 0)
        return false;

    protocol pro;
    /*协议消息为帐号登录*/
    pro.data_type = ACCOUNT_INFO;
    pro.msg_type  = LOGIN_ACCOUNT;
    pro.msg_len   = 0;

    memset((void*)&this->acnt, 0, sizeof(account_info));
    strncpy(this->acnt.id, acc, strlen(acc));
    strncpy(this->acnt.name, acc, strlen(acc));
    //strncpy(acc_info.pwd, pwd, strlen(pwd));
    this->acnt.type = user;

    char buf[1500] = {};
    size_t len = sizeof(protocol);
    /*填充协议*/
    memcpy((void*)buf, (const void*)&pro, len);
    /*填充帐号类型消息*/
    memcpy((void*)(buf+len), (const void*)&this->acnt, sizeof(account_info));
    len += sizeof(account_info);
    pro.msg_len = len - sizeof(protocol);

    int ret = write(this->tcp_fd, buf, len);
    if(ret < 0)
    {
        qDebug() << "[failed]the message send to server";
        return false;
    }
    else
    {
        qDebug() << "[loading]loginnig...";
    }

    return true;
}

bool Cconnect::registered(const char *acc, const char *pwd)
{
    if(this->is_link == false || this->tcp_fd == 0)
        return false;

    protocol pro;
    /*协议消息为帐号注册*/
    pro.data_type = ACCOUNT_INFO;
    pro.msg_type  = REGISTER_ACCOUNT;
    pro.msg_len   = 0;

    memset((void*)&this->acnt, 0, sizeof(account_info));
    strncpy(this->acnt.name, acc, strlen(acc));
    this->acnt.type = user;

    char buf[1500] = {};
    size_t len = sizeof(protocol);
    /*填充协议*/
    memcpy((void*)buf, (const void*)&pro, len);
    /*填充帐号类型消息*/
    memcpy((void*)(buf+len), (const void*)&this->acnt, sizeof(account_info));
    len += sizeof(account_info);
    pro.msg_len = len - sizeof(protocol);

    int ret = write(this->tcp_fd, buf, len);
    if(ret < 0)
    {
        qDebug() << "[failed]the message send to server";
        return false;
    }
    return true;
}

bool Cconnect::list_online()
{
    if(this->is_link == false || this->tcp_fd == 0)
        return false;

    protocol pro;
    /*协议消息为获取在线用户列表*/
    pro.data_type = ACCOUNT_INFO;
    pro.msg_type  = LIST_ONLINE;
    pro.msg_len   = 0;

    char buf[1500] = {};
    size_t len = sizeof(protocol);
    /*填充协议*/
    memcpy((void*)buf, (const void*)&pro, len);
    /*填充帐号类型消息*/
    memcpy((void*)(buf+len), (const void*)&this->acnt, sizeof(account_info));
    len += sizeof(account_info);
    pro.msg_len = len - sizeof(protocol);

    int ret = write(this->tcp_fd, buf, len);
    if(ret < 0)
    {
        qDebug() << "[failed]the message send to server";
        return false;
    }
    return true;
}

bool Cconnect::send_msg(msg *_msg, int msglen)
{
    /*发送的消息需要添加到消息列表*/
    (this->cu->second).push_back(_msg);
    if(this->is_link == false || this->tcp_fd == 0)
        return false;

    protocol pro;
    /*协议消息为发送消息*/
    pro.data_type = MESSAGE;
    pro.msg_type  = SEND_MSG;
    pro.msg_len   = 0;

    char buf[1500] = {};
    size_t len = sizeof(protocol);
    /*填充协议*/
    memcpy((void*)buf, (const void*)&pro, len);

    /*填充消息数据*/
    memcpy((void*)(buf+len), (const void*)_msg, msglen);
    len += msglen;
    pro.msg_len = len - sizeof(protocol);
    /*qDebug() << len;
    for(int i = 0; i < len; i ++)
        printf(" %d", buf[i]);
    printf("\n");*/

    int ret = write(this->tcp_fd, buf, len);
    if(ret < 0)
    {
        qDebug() << "[failed]the message send to server";
        return false;
    }
    return true;
}

void Cconnect::send()
{

    while(1)
    {
        /*从服务器端读取数据*/
        if(sem_wait(&this->sem2) < 0)
            perror("");
        memset((void*)this->BUF, 0, sizeof(this->BUF));
        int ret = read(this->tcp_fd, this->BUF, sizeof(this->BUF));
        if(ret < 0)
            perror("");
        this->BUFLEN = ret;
        qDebug() << "read from server";
        /*for(int i = 0; i < this->BUFLEN; i ++)
            printf(" %d", this->BUF[i]);
        printf("\n");*/

        if(sem_post(&this->sem1) < 0)
            perror("");
    }
}
