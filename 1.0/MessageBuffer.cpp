#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "MessageBuffer.h"

MessageBuffer::MessageBuffer() {
  toStop = false;
  pthread_mutex_init(&mutex,NULL);//初始化互斥量
  pthread_cond_init(&condition,NULL);//初始化条件变量
  rear = 0; //队尾指针指向0
  front = 0; //队首指针指向0
  printf("A MessageBuffer intance created./n");
}
MessageBuffer::~MessageBuffer(){
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&condition);
  printf("A MessageBuffer instance destroyed./n");
}
//将消息放入消息缓冲区
int MessageBuffer::PutMessage(const char *message){
  struct timespec t;
  //等待互斥量
  pthread_mutex_lock(&mutex);
  while(!toStop && (rear+1)%MESSAGE_COUNT==front){
     t.tv_sec = time(NULL)+1;
     t.tv_nsec = 0;
     pthread_cond_timedwait(&condition,&mutex,&t);
  }
  if(toStop){
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&mutex);
    return -1;
  }
  int messageLen = strlen(message);
  int copyLen = messageLen>=MESSAGE_LENGTH?MESSAGE_LENGTH-1:messageLen;
  memcpy(buf[rear],message,copyLen);
  buf[rear][copyLen]='\0';
  rear = (rear+1)%MESSAGE_COUNT;
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&mutex);
  return 0;
}
//从消息缓冲区中获得消息
int MessageBuffer::GetMessage(char *mbuf, int buflen){
  struct timespec t;
  pthread_mutex_lock(&mutex);
  while(!toStop && rear==front){
    t.tv_sec = time(NULL)+1;
    t.tv_nsec = 0;
    pthread_cond_timedwait(&condition,&mutex,&t);
  }
  if(toStop){
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&mutex);
    return -1;
  }
  int messageLen = strlen(buf[front]);
  int copyLen = messageLen>=buflen ? buflen-1 : messageLen;
  memcpy(mbuf,buf[front],copyLen);
  mbuf[copyLen]='\0';
  front = (front+1)%MESSAGE_COUNT;
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&mutex);
  return 0;
}
