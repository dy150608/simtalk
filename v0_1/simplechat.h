#ifndef SIMPLECHAT_H
#define SIMPLECHAT_H

#include <QTimer>
#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QtCore/QCoreApplication>
#include "terminal/net/connect.hpp"

#include <unistd.h>
#include <fstream>

namespace Ui {
class simplechat;
}

void mq_send(Cconnect *con);

class simplechat : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit simplechat(QWidget *parent = 0);
    ~simplechat();
    void set_con(Cconnect *con);
    void update_msg();
    void update_ctact();
    friend void mq_send(Cconnect *con);
    void mq_send();
    void mq_recv();
    void read_history_message();
    void write_history_message();
    void read_help(std::string);

signals:
    void sig_reg(char*);
    void sig_login(char*);
    void sig_list(char*);
    void sig_recv(char*);

public slots:
    void loginret(char *msg);
    void registeret(char *msg);
    void listret(char *msg);
    void recvmsg(char *msg);
    void timer_timeout();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_listWidget_2_clicked(const QModelIndex &index);

    void on_listWidget_clicked(const QModelIndex &index);

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

private:
    QListWidgetItem *curr = nullptr;      //当前对话用户
    std::string contact;
    Ui::simplechat *ui;
    Cconnect *con = nullptr;
    std::string _msg;
};

#endif // SIMPLECHAT_H
