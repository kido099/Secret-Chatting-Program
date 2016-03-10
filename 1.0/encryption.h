#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "key_produce.h"


class CENCRYPTION{
public:
 CENCRYPTION();
 virtual ~CENCRYPTION();

 void Encrypt(UINT PublicKey, UINT mod, FILE* fipRe, FILE* fipWr,char* extrName );
 void Explain(UINT PrivateKey, UINT mod, FILE* fipRe, FILE* fipWr );
 void TxtEncrypt(unsigned* cipSourceTxt, int buffSize, unsigned int Ke, unsigned int model);

private:
//==================================求b的e次方除d的余数
unsigned int GetOutNum(unsigned int b,unsigned int e , unsigned int d);
//==================================对原文进行加密并覆盖原缓冲区
};

#endif // ENCRYPTION_H
