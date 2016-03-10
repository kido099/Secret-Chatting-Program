#include "key_produce.h"
#include<iostream>
#include<stdio.h>

 CKEY_PRODUCE::CKEY_PRODUCE() {}
 CKEY_PRODUCE::~CKEY_PRODUCE() {}

 //产生RSA 公_私 密钥
int CKEY_PRODUCE::Produce_RSA_Key(unsigned int p,unsigned int q, unsigned int* Ke, unsigned int* Kd, unsigned int* model) {
 unsigned int ao_la;
 if( Count_N_AoLa_Num(p, q, &ao_la) ) {
   if( RandSelect_e(ao_la, Ke) )  {
     if( OverOneNum(*Ke, ao_la, Kd) )  {
        *model= p*q;
        return 1;
     }
   }
 }
 return 0;
}

//判断prime是否为素数
int CKEY_PRODUCE::JudgePrime(unsigned int prime) {
 unsigned int i;
 unsigned int limit= (unsigned int)sqrt( (double)prime );
 for(i=2; i <= limit; i++) {
  if(prime%i==0) {
   return 0;
  }
 }
 return 1;
}

//算出p*q的欧拉值
int CKEY_PRODUCE::Count_N_AoLa_Num(unsigned int p, unsigned int q, unsigned int * ao_la) {
 if(!JudgePrime(p)) {
  return 0;
 }
 if(!JudgePrime(q)) {
  return 0;
 }
 *ao_la = (p-1)*(q-1);
 return 1;
}

//求两个数的最大公因数
unsigned int CKEY_PRODUCE::CountCommonData(unsigned int a, unsigned int b) {
 unsigned int c;
 c=a%b;
 if( c ) {
  return CountCommonData(b,c);
 }
 else {
  return b;
 }
}

//随机选择公钥e
int CKEY_PRODUCE::RandSelect_e( unsigned int ao_la, unsigned int* e ) {
 unsigned int tmp;
 unsigned int div;
 if( ao_la <= 2 ) {
  return 0;
 }
 srand( time(0) );
 div= ao_la - 2;
 do {
  tmp = ( (unsigned int)rand()+90 ) % div + 2;
 }
 while( CountCommonData(tmp, ao_la) != 1 );
 *e = tmp;
 return 1;
}

//求b的e次方除d的余数
unsigned int CKEY_PRODUCE::GetOutNum(unsigned int b,unsigned int e , unsigned int d) {
 unsigned int i;
 unsigned int outNum = 1;
 for( i=0; i<e; i++) {
  outNum *= b;
  if( outNum >= d ) {
   outNum %= d;
  }
  if(!outNum) {
   return outNum;
  }
 }
 return outNum%d;
}

//利用加的模等于模的加求e*d = 1 mod model 中的d
int CKEY_PRODUCE::OverOneNum(unsigned int e,unsigned int model, unsigned int* d) {
 unsigned int i;
 unsigned int over= e;
 for(i=1; i<model; ) {
  over= over % model;
  if( over==1 ) {
   *d = i;
   return 1;
  }
  else {
   if(over+e<= model) {
    do {
     i++;
     over += e;
    }
    while( over+e <= model );
   }
   else {
    i++;
    over +=e;
   }
  }
 }
 return 0;
}

//求任意大于1的整数的欧拉值
unsigned int CKEY_PRODUCE::CountAnyNumAola(unsigned int number) {
 unsigned int ao_la= 1;
 unsigned int i;
  if( number<=1 ) {
   printf("本函数不处理2以下的范围!\n");
  }
  for(i=2; i<number ; i++) {
    if( CountCommonData(number, i)==1 )
         ao_la ++;
  }
 return ao_la;
}
