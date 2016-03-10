
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "Clients.h"

Clients::Clients() {
  pthread_mutex_init(&mutex, NULL);
  clientCount = 0;
  printf("Clients created./n");
}
Clients::~Clients() {
  pthread_mutex_destroy(&mutex);
  printf("Clients destroyed./n");
}

int Clients::Search(int sock){
  int index = -1;
  for(int i=0; i<clientCount; i++) {
     if(client[i].sock==sock){
        index = i;
        break;
     }
  }
  return index;
}
int Clients::IPtoString(unsigned long ip,char *buf,int buflen){
    unsigned char *p = (unsigned char*)&ip;
    if(buflen<16){
       return -1;
    }
    sprintf(buf,"%u.%u.%u.%u",*p,*(p+1),*(p+2),*(p+3));
    return strlen(buf);
}
int Clients::GetClientCount(){
   return clientCount;
}
bool Clients::PutClient(int sock,const struct sockaddr_in &clientAddr) {
  if(clientCount==MAX_CLIENT){
    return false;
  }
  pthread_mutex_lock(&mutex);
  client[clientCount].sock = sock;
  client[clientCount].clientAddr = clientAddr;
  int buflen = sizeof(client[clientCount].name);
  int pos = IPtoString(clientAddr.sin_addr.s_addr,client[clientCount].name,buflen);
  sprintf(&client[clientCount].name[pos],":%d",ntohs(clientAddr.sin_port));

  clientCount++;
  pthread_mutex_unlock(&mutex);
  return true;
}
void Clients::RemoveClient(int sock){
  pthread_mutex_lock(&mutex);
  int index = Search(sock);
  if(index!=-1){
    for(int i=index; i<clientCount-1; i++){
    client[i] = client[i+1];
    }
    clientCount--;
  }
  pthread_mutex_unlock(&mutex);
}
bool Clients::GetAddrBySocket(int sock,struct sockaddr_in *addr){
  pthread_mutex_lock(&mutex);
  int index = Search(sock);
  if(index!=-1){
    memcpy(addr,&client[index].clientAddr,sizeof(struct sockaddr_in));
  }
  pthread_mutex_unlock(&mutex);
  return index!=-1;
}
bool Clients::PutName(int sock,const char *name,int namelen) {
  pthread_mutex_lock(&mutex);
  int index = Search(sock);
  if(index!=-1){
    int copyLen = namelen>=NAME_LEN ? NAME_LEN-1:namelen;
    memcpy(client[index].name,name,copyLen);
    client[index].name[copyLen]='\0';
  }
  pthread_mutex_unlock(&mutex);
  return index!=-1;
}
bool Clients::GetName(int sock, char *name, int namelen) {
  pthread_mutex_lock(&mutex);
  int index = Search(sock);
  if(index!=-1){
    int msgLen = strlen(client[index].name);
    int copyLen = (msgLen<namelen)? msgLen:(namelen-1);
    memcpy(name,client[index].name,copyLen);
    name[copyLen]='\0';
  }
  pthread_mutex_unlock(&mutex);
  return index!=-1;
}
int Clients::GetAllSocket(int* sockArray, int arrayLen ) {
  pthread_mutex_lock(&mutex);
  int copyCount = arrayLen>clientCount ? clientCount : arrayLen;
  for(int i=0; i<copyCount; i++){
    sockArray[i] = client[i].sock;
  }
  pthread_mutex_unlock(&mutex);
  return copyCount;
}
