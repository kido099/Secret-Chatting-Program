#include "encryption.h"

CENCRYPTION::CENCRYPTION() { }
CENCRYPTION::~CENCRYPTION() { }

//==================================对原文进行加密并覆盖原缓冲区
void CENCRYPTION::TxtEncrypt(unsigned* cipSourceTxt, int buffSize, unsigned int Ke, unsigned int model)
{
 int i;
 for( i=0; i < buffSize; i++ )
 {
  cipSourceTxt[i] = GetOutNum( cipSourceTxt[i], Ke, model );
 }
}

//==================================求b的e次方除d的余数
unsigned int CENCRYPTION::GetOutNum(unsigned int b,unsigned int e , unsigned int d)
{
unsigned int i;
 unsigned int outNum= 1;
 for( i=0; i < e; i++)//=========用了乘的模 等于 模的乘
 {
  outNum *= b;
  if( outNum >= d )
  {
   outNum %= d;
  }
  if(!outNum)
   return outNum;
 }
 return outNum%d;
}

void CENCRYPTION::Encrypt(UINT PublicKey, UINT mod, FILE* fipRe, FILE* fipWr ,char* extrName)
{
 unsigned int ReSize;
 unsigned int uBuf[BUFFER_SIZE]= {0,};
 char cBuf[BUFFER_SIZE];
 unsigned int i;
 for(i=0; i&lt3; i++)//=====我认为扩展名是3个字符
 {
  if(extrName)//=========如果有扩展名, 将扩展名放入uBuf 和数据一样加密
  {
   uBuf[i]= 0;
   *((char*)(&uBuf[i])) = extrName[i];
  }
  else
   uBuf[i]= 0;
 }
 if(extrName)//===============如果有扩展名, 将扩展名加密
  TxtEncrypt(uBuf, 3,PublicKey,mod);
fwrite( (char*)uBuf,1, 3*sizeof(unsigned int), fipWr);//密文前12个,字节中是源文件的 扩展名信息
 do
 {
  ReSize= fread(cBuf, 1, BUFFER_SIZE,fipRe);
  if(ReSize)
  {
   unsigned int record=1;
   unsigned int WrNum;
for(i=0; i < ReSize; i++)
   {
    uBuf[i]= 0;
    *((char*)(&uBuf[i])) = (cBuf[i]) ;
   }    TxtEncrypt(uBuf, ReSize,PublicKey,mod);    WrNum= fwrite( (char*)uBuf,1, ReSize*sizeof(unsigned int), fipWr);
   printf("第%d次写入%d字节!\n",record++, WrNum);
  }
 }while(ReSize == BUFFER_SIZE);
}
void CENCRYPTION::Explain(UINT PrivateKey, UINT mod, FILE* fipRe, FILE* fipWr )
{
 unsigned int ReSize;
 unsigned int uBuf[BUFFER_SIZE]= {0,};
 char cBuf[BUFFER_SIZE];
 do
 {
  ReSize= fread(uBuf, sizeof(unsigned int), BUFFER_SIZE,fipRe);
  if(ReSize)
  {
   unsigned int i;
   unsigned int record=1;
   unsigned int WrNum;
   TxtEncrypt(uBuf, ReSize,PrivateKey,mod);
   for(i=0; i&ltReSize; i++)
    cBuf[i]= (char)(uBuf[i]);
   WrNum= fwrite( cBuf,1, ReSize, fipWr);
   printf("第%d次写入%d字节!\n",record++, WrNum);
  }

 }while(ReSize == BUFFER_SIZE);
}

