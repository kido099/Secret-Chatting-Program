#ifndef KEY_PRODUCE_H
#define KEY_PRODUCE_H

#include<iostream>
#include<cstdlib>
#include<math.h>

class CKEY_PRODUCE {
public:
 CKEY_PRODUCE();
 virtual ~CKEY_PRODUCE();

 int JudgePrime(unsigned int prime); //判断prime是否为素数

 int Count_N_AoLa_Num(unsigned int p, unsigned int q, unsigned int* ao_la); //算出p*q的欧拉值

 unsigned int CountCommonData(unsigned int a, unsigned int b); //求两个数的最大公因数

 int RandSelect_e(unsigned int ao_la, unsigned int * e ); //随机选择公钥e

 unsigned int GetOutNum(unsigned int b,unsigned int e , unsigned int d); //求b的e次方除d的余数

 unsigned int CountAnyNumAola(unsigned int number); //求任意大于2的整数的欧拉值

 int Produce_RSA_Key(unsigned int p,unsigned int q, unsigned int* Ke, unsigned int* Kd, unsigned int* model); //产生RSA 公_私 密钥

 int OverOneNum(unsigned int e,unsigned int model, unsigned int* d); //利用加的模等于模的加求e*d = 1 mod model 中的d
};

#endif
