#ifndef CLIENTS_H
#define CLIENTS_H
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#define NAME_LEN 50
#define MAX_CLIENT 30

typedef struct client_info{
   int sock;
   struct sockaddr_in clientAddr;
   char name[NAME_LEN];
} CLIENT_INFO;

class Clients{
private:
  pthread_mutex_t mutex;
  CLIENT_INFO client[MAX_CLIENT];
  int clientCount;
  int IPtoString(unsigned long ip, char *buf, int buflen);
  int Search(int sock);
public:
  Clients();//构造函数
  virtual ~Clients();//析构函数

  int GetClientCount();
  bool PutClient(int sock,const struct sockaddr_in &clientAddr);
  void RemoveClient(int sock);
  bool GetAddrBySocket(int sock,struct sockaddr_in *addr);
  bool PutName(int sock,const char *name, int namelen);
  bool GetName(int sock, char *name, int namelen);
  int GetAllSocket(int* sockArray, int arrayLen );
};

#endif // CLIENTS_H
