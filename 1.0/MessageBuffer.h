#ifndef MESSAGEBUFFER_H
#define MESSAGEBUFFER_H
#include <pthread.h>
#define MESSAGE_COUNT 16
#define MESSAGE_LENGTH 2048

class MessageBuffer{
private:
  pthread_mutex_t mutex;//访问缓冲的互斥量
  pthread_cond_t condition;//访问缓冲区的条件变量
  //消息缓冲区，循环队列
  char buf[MESSAGE_COUNT][MESSAGE_LENGTH];
  int rear; //循环队列的队尾
  int front; //循环队列的队首
public:
   bool toStop;
   //构造函数
   MessageBuffer();
   //析构函数
   virtual ~MessageBuffer();
   //将消息放入消息缓冲区，当缓冲区满时阻塞，toStop=true时返回-1
   int PutMessage(const char *message);
   //从消息缓冲区中获得消息，当缓冲区空时阻塞，toStop=true时返回-1
   int GetMessage(char *mbuf, int buflen);
};

#endif // MESSAGEBUFFER_H
