#include "dialog.h"
#include "ui_dialog.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <QTimer>
#include <QtCore>
#define MAXBUF 1024
#include <sys/select.h>
#include <pthread.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "MessageBuffer.h"
#include "Clients.h"
using namespace std;
#define SERVER_PORT 8000
#define BUFFER_SIZE 4096
#ifndef MAX_CLIENT
#define MAX_CLIENT 30
#endif
#ifndef NAME_LEN
#define NAME_LEN 50
#endif

bool toStop=false;
int s;
MessageBuffer messageBuffer;
Clients clients;
pthread_t tListenId;
pthread_t tRecvId;
pthread_t tSendId;




int sockfd, len;
    struct sockaddr_in dest;
    char buffer[MAXBUF + 1];
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd = -1;


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::help(){
//    int len;
//        //struct sockaddr_in dest;
//        char buffer[MAXBUF + 1];
//        fd_set rfds;
//        struct timeval tv;
//        int retval, maxfd = -1;
        /* 把集合清空 */
        FD_ZERO(&rfds);
        /* 把标准输入句柄0加入到集合中 */
        FD_SET(0, &rfds);
        maxfd = 0;
        /* 把当前连接句柄sockfd加入到集合中 */
        FD_SET(sockfd, &rfds);
        if (sockfd > maxfd)
            maxfd = sockfd;
        /* 设置最大等待时间 */
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        /* 开始等待 */
        retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
//printf("%d  %d %d  %d ",retval,sockfd, maxfd, rfds);
        if (retval == -1) {
            printf("将退出，select出错！ %s", strerror(errno));
            exit(0);
         //} else if (retval == 0) {
            /* printf
               ("没有任何消息到来，用户也没有按键，继续等待……\n"); */
           // continue;
        } else {
            if (FD_ISSET(sockfd, &rfds)) {
                /* 连接的socket上有消息到来则接收对方发过来的消息并显示 */
                bzero(buffer, MAXBUF + 1);
                /* 接收对方发过来的消息，最多接收 MAXBUF 个字节 */
                len = recv(sockfd, buffer, MAXBUF, 0);
                if (len > 0){

                   QString out = QString(QLatin1String(buffer));
                   // printf
                     //   ("接收消息成功:'%s'，共%d个字节的数据\n",
                       //  buffer, len);
                  ui->textBrowser->append(out);
                }

                    else {
                    if (len < 0)
                        printf
                            ("消息接收失败！错误代码是%d，错误信息是'%s'\n",
                             errno, strerror(errno));
                    else
                        printf("对方退出了，聊天终止！\n");
                    exit(0);
                }
            }
            ///////////////////////////////////////////////
        }
    }


void Dialog::on_pushButton_clicked()
{

//    //int sockfd; //len;
//        struct sockaddr_in dest;
//        //char buffer[MAXBUF + 1];
//        //fd_set rfds;
//        //struct timeval tv;
//        //int retval, maxfd = -1;

        QString port=ui->lineEdit->text();
        QByteArray b = port.toLatin1();
            char *p = b.data();
        QString IP=ui->lineEdit_2->text();
        QByteArray a = IP.toLatin1();
            char *I = a.data();

//        if (argc != 3) {
//            printf
//                ("参数格式错误！正确用法如下：\n\t\t%s IP地址 端口\n\t比如:\t%s 127.0.0.1 80\n此程序用来从某个 IP 地址的服务器某个端口接收最多 MAXBUF 个字节的消息",
//                 argv[0], argv[0]);
//            exit(0);
//        }
        /* 创建一个 socket 用于 tcp 通信 */
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Socket");
            exit(errno);
        }

        /* 初始化服务器端（对方）的地址和端口信息 */
        bzero(&dest, sizeof(dest));
        dest.sin_family = AF_INET;
        dest.sin_port = htons(atoi(p));
        if (inet_aton(I, (struct in_addr *) &dest.sin_addr.s_addr) == 0) {
            perror(I);
            exit(errno);
        }

        /* 连接服务器 */
        if (::connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
            perror("Connect ");
            exit(errno);
        }
printf("%d",sockfd);
        printf
            ("\n准备就绪，可以开始聊天了……直接输入消息回车即可发信息给对方\n");
        QTimer* timer=new QTimer(this);
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(help()));
        timer->start(2500);

        /* 关闭连接 */
        //::close(sockfd);
        //return 0;
}

void Dialog::on_pushButton_2_clicked()
{
//    if (FD_ISSET(0, &rfds)) {
        /* 用户按键了，则读取用户输入的内容发送出去 */
        bzero(buffer, MAXBUF + 1);
        QString input =ui->textEdit->toPlainText();
        QByteArray c = input.toLatin1();
            char *buffer = c.data();
        //fgets(buffer, MAXBUF, stdin);
        if (!strncasecmp(buffer, "quit", 4)) {
            printf("自己请求终止聊天！\n");
                        exit(0);
        }
        /* 发消息给服务器 */
        len = send(sockfd, buffer, strlen(buffer), 0);
        if (len < 0) {
            printf
                ("消息'%s'发送失败！错误代码是%d，错误信息是'%s'\n",
                 buffer, errno, strerror(errno));
            exit(0);
        } else
            printf
                ("消息：%s\t发送成功，共发送了%d个字节！\n",
                 buffer, len);
    //}
}


void ProcessMessage(int sock,char buf[],int bufsize,int bytes){
    struct sockaddr_in clientAddr;
      if(!clients.GetAddrBySocket(sock,&clientAddr)){
         printf("ProcessMessage: can not find socket address./n");
         return;
      }
      char ipString[16];
      unsigned char *ip = (unsigned char*)&clientAddr.sin_addr.s_addr;
      sprintf(ipString,"%u.%u.%u.%u",*ip,*(ip+1),*(ip+2),*(ip+3));
      unsigned short port = ntohs(clientAddr.sin_port);
      buf[bytes]='\0';
      printf("Message from %s:%d: %s/n",ipString,port,buf);
      const char* CMD_BYE="bye";
      if(strcmp(buf,CMD_BYE)==0){
        send(sock,CMD_BYE,strlen(CMD_BYE),0);
        clients.RemoveClient(sock);
        ::close(sock);
        printf("%s:%u disconnected./n", ipString, port);
        return;
      }else{
            char bufWithName[BUFFER_SIZE+NAME_LEN];
        char cmdname[5];
            char name[NAME_LEN];
        memcpy(cmdname, buf, 4);
        cmdname[4] = '\0';
            const char* CMD_NAME="name";
        if(strcmp(cmdname,CMD_NAME)==0){
           char newname[NAME_LEN];
               int nameLen = strlen(buf+5);
               int copyLen;
               if(nameLen>=NAME_LEN){
                  copyLen = NAME_LEN-1;
               }else{
                  copyLen = nameLen;
               }
               memcpy(newname,buf+5,copyLen);
               newname[copyLen]='\0';
           clients.GetName(sock,name,sizeof(name));
           sprintf(bufWithName,"%s change name to %s",name,newname);
           clients.PutName(sock,newname,strlen(newname));
           messageBuffer.PutMessage(bufWithName);
            }else{
               clients.GetName(sock,name,sizeof(name));
               sprintf(bufWithName,"%s: %s",name,buf);
           messageBuffer.PutMessage(bufWithName);
            }
      }
}


    void* ListenThread(void*ps){
        int s=*(int*)ps;
          fd_set listenSet;
          int sock;
          struct sockaddr_in clientAddr;
          struct timeval timeout;
          while(!toStop){
             FD_ZERO(&listenSet);
             FD_SET(s,&listenSet);
             timeout.tv_sec = 5;
             timeout.tv_usec = 0;
             int ret = select(s+1,&listenSet,NULL,NULL,&timeout);
             if(toStop){
                  printf("ListenThread: exit./n");
                  return NULL;
             }
             if(ret==-1){
                printf("ListenThread: select() failed!/n");
             }else if(ret==0){
                printf("ListenThread: select() time out./n");
             }else{
                if(FD_ISSET(s,&listenSet)){
                   socklen_t addrlen = sizeof(struct sockaddr_in);
                   memset(&clientAddr,0,sizeof(struct sockaddr_in));
                   if((sock=accept(s,(struct sockaddr*)&clientAddr,&addrlen))==-1){
                       fprintf(stderr,"accept failed./n");
                   }
                   if(!clients.PutClient(sock,clientAddr)){
                       printf("max client limited. MAX_CLIENT=%d/n",MAX_CLIENT);
                       ::close(sock);
                   }
                   printf("accept a connection from %s:%u/n",
                           inet_ntoa(*(struct in_addr*)&(clientAddr.sin_addr.s_addr)),
                           ntohs(clientAddr.sin_port));
                   printf("new socket is: %u/n",sock);
                 }
             }
           }
           return NULL;
}

    void* RecvThread(void*){

        fd_set readSet;
          int sock[MAX_CLIENT];
          char buf[BUFFER_SIZE];
          struct timeval timeout;
          while(!toStop){
            int count = clients.GetAllSocket(sock,MAX_CLIENT);
            if(count==0){
              sleep(2);
              if(toStop){
                printf("RecvThread: exit./n");
                return NULL;
              }
              continue;
            }
            FD_ZERO(&readSet);
            int maxfd=0;
            for(int i=0;i<count;i++){
               printf("--%d",sock[i]);
               FD_SET(sock[i],&readSet);
               if(sock[i]>maxfd){
                 maxfd = sock[i];
               }
            }
            printf("/n");
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;
            int ret = select(maxfd+1,&readSet,NULL,NULL,&timeout);
            if(toStop){
                printf("RecvThread: exit./n");
                return NULL;
            }
            if(ret==-1){
              printf("RecvThread: select() failed!/n");
            }else if(ret==0){
              printf("RecvThread: select() time out./n");
            }else{
              for(int i=0; i<count; i++){
                 if(FD_ISSET(sock[i],&readSet)){
                    int bytes=recv(sock[i],buf,sizeof(buf)-1,0);
                    if(bytes==-1){
                      printf("RecvThread: recv failed./n");
                      clients.RemoveClient(sock[i]);
                      ::close(sock[i]);
                    }else if(bytes==0){
                      printf("RecvThread: socket closed by the other side./n");
                      clients.RemoveClient(sock[i]);
                      ::close(sock[i]);
                    }else{
                      ProcessMessage(sock[i],buf,sizeof(buf),bytes);
                    }
                 }
              }
            }

          }

         return NULL;
        }
void* SendThread(void*){
    fd_set writeSet;
       int sock[MAX_CLIENT];
       char buf[BUFFER_SIZE];
       struct timeval timeout;
       while(!toStop){
           int ret = messageBuffer.GetMessage(buf,sizeof(buf));
           printf("get a message from buffer./n");
           if(ret==-1){
              printf("SendThread: exit./n");
              return NULL;
           }
           int count = clients.GetAllSocket(sock,MAX_CLIENT);
           FD_ZERO(&writeSet);
           int maxfd = 0;
           for(int i=0;i<count;i++){
              FD_SET(sock[i],&writeSet);
              if(sock[i]>maxfd){
                  maxfd = sock[i];
              }
           }
           timeout.tv_sec = 2;
           timeout.tv_usec = 0;
           ret = select(maxfd+1,NULL,&writeSet,NULL,&timeout);
           if(toStop){
              printf("SendThread: exit./n");
              return NULL;
           }
           if(ret==-1){
             printf("SendThread: select() failed!/n");
           }else if(ret==0){
             printf("SendThread: select() time out./n");
           }else{
             for(int i=0;i<count;i++){
                if(FD_ISSET(sock[i],&writeSet)){
                    int messageLen = strlen(buf);
                    int bytes = send(sock[i],buf,messageLen,0);
                    if(bytes==-1){
                       printf("SendThread: send() failed./n");
                    }else if(bytes!=messageLen){
                       printf("SendThread: send message trunked.");
                    }else{
                       //do nothing
                    }
                }
             }
           }
       }
       return NULL;

}



void Dialog::on_pushButton_3_clicked()
{
    void* ListenThread(void*);
    void* RecvThread(void*);
    void* SendThread(void*);
   void ProcessMessage(int sock,char buf[],int bufsize,int bytes);

    QString myport=ui->lineEdit_3->text();
    QByteArray b = myport.toLatin1();
        char *p = b.data();
         unsigned int port = atoi(p);
//      int s;
      struct sockaddr_in serverAddr;

      s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
      if(s==-1){
        fprintf(stderr,"create socket failed./n");
        exit(0);
      }
      bzero(&serverAddr,sizeof(struct sockaddr_in));
      serverAddr.sin_family = AF_INET;
      serverAddr.sin_port = htons(port);
      serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      if(bind(s,(struct sockaddr*)&serverAddr,sizeof(serverAddr))==-1){
        fprintf(stderr,"bind socket to port %d failed./n",port);
       exit(0);
      }
      if(listen(s,SOMAXCONN)==-1){
        fprintf(stderr,"listen failed./n");
        exit(0);
      }
      printf("Server  is listening on ");
      char hostname[255];
      if(gethostname(hostname,sizeof(hostname))){
        printf("gethostname() failed./n");
        exit(0);
      }
      struct hostent* pHost = gethostbyname(hostname);
      if(pHost){
          for(int i=0; pHost->h_addr_list[i]; i++){
             //qDebug("%s ",inet_ntoa(*(in_addr*)pHost->h_addr_list[i]));
          ui->lineEdit_2->setText(inet_ntoa(*(in_addr*)pHost->h_addr_list[i]));
          }
      }
      printf("/nport: %d/n",port);
//      pthread_t tListenId;
      if(pthread_create(&tListenId,NULL,ListenThread,&s)){
        printf("failed to create listen thread./n");
        exit(0);
      }
//      pthread_t tRecvId;
      if(pthread_create(&tRecvId,NULL,RecvThread,NULL)){
        printf("failed to create recv thread./n");
        exit(0);
      }
//      pthread_t tSendId;
      if(pthread_create(&tSendId,NULL,SendThread,NULL)){
        printf("failed to create send thread./n");
       exit(0);
      }
//qDebug("whilebefore");
//     if(getchar()!='q')
//         exit(0);

//      toStop = true;
//      messageBuffer.toStop = true;
//qDebug("before");
//      pthread_join(tListenId,NULL);
//      pthread_join(tRecvId,NULL);
//      pthread_join(tSendId,NULL);
//      //char*m="after";
//      qDebug("after");
//      ::close(s);
//      int sock[MAX_CLIENT];
//      int count = clients.GetAllSocket(sock,MAX_CLIENT);
//      for(int i=0;i<count;i++){
//        ::close(sock[i]);
//      }

//      printf("server stopped./n");

//      exit(0);

}

void Dialog::on_pushButton_4_clicked()
{
    toStop = true;
    messageBuffer.toStop = true;
qDebug("before");
    pthread_join(tListenId,NULL);
    pthread_join(tRecvId,NULL);
    pthread_join(tSendId,NULL);
    //char*m="after";
    qDebug("after");
    ::close(s);
    int sock[MAX_CLIENT];
    int count = clients.GetAllSocket(sock,MAX_CLIENT);
    for(int i=0;i<count;i++){
      ::close(sock[i]);
    }

    printf("server stopped./n");

    exit(0);
}
