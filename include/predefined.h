#ifndef PREDEFINED_H
#define PREDEFINED_H

// 事先定义一个头文件，包含项目可能会用到的变量名，函数，宏....

#include<string.h>
// #include<cstring.h>

//自定义一个命名空间，在其他而文件使用时，直接声明using 即可
namespace bpt {

//自定义一个 b+树 order 阶数
#define BP_ORDER 50

//数据的结构体，插入，更改等公用
struct value_t{
	char name[256];
	int age;
	char email[256];
};

//自定义了一个16字符大小的空字符串？？
struct key_t{	
	char k[16];

	key_t(const char *str = ""){
		//清除内存，原项目用bzero,
		memset(k, 0, sizeof(k));	//clean 0-sizeof(k)
		strcpy(k, str);		//str内容复制到 k
	}
};

inline int keycmp(const key_t &a, const key_t &b){
	int x = strlen(a.k) - strlen(b.k);
	//strcmp 比较两个字符串长度大小
	return x == 0 ? strcmp(a.k, b.k) : x;
}
	
}

#endif