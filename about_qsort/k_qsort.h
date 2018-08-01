/*
时间:2018/7/31
作者:张宇坤
内容:仿照stdlib库实现快排函数 接受参数void *类型 ,调用时需要传递一个指向函数的指针 自定义比较函数
否则快排将不知道数据类型,无法进行比较

技术总结
1
在操作void*类型的指针时 注意其指类 才能进一步用好void* 例如
void *mid; mid + 4; 是对mid指针的地址加4个字节
而同样是void*指针, ((int *)mid + 2) 则不是加2的int类型大小的字节,也就是移动8个字节 这里mid指针确定了指类
强转double和char也是同理 这是较忽略掉的一点

2
在排序算法中,尤其完成的是快排 一定涉及交换元素,由于是void*类型交换的时候, 不能够确定如何申请中间变量
不能申请一个指针去交换 void *temp,那么交换不是内存处的值,交换的是指针的指向
解决: 将指针想到内存中 ,以字节存放, 那么就挨个交换字节.  char就可以当做中间变量,  比如int为四字节 需要交换四次
所以内部是需要循环的.

任何类型的指针都是4字节(32位下), 指针的本质就是代表一个地址 0028FF38 这是16进制的表示, 每一位16进制由四位二进制表示
所以是32位二进制的一个地址 0-1 本质还是0-1代码
0     0     2   8     F    F    3   8
0000 0000 0010 1000 1111 1111 0011 1000
00000000-FFFFFFFFF
*/

#ifndef _K_QSORT_H_
#define _K_QSORT_H_

#include<malloc.h>
#include<string.h>

void k_swap(void *a,  void *b, unsigned int type);

// 仿照stdlib库实现快排函数 void*类型的数据 实际上是对内存的操作
//
void k_qsort(void *, size_t, size_t, int (*)(const void *, const void *));


void k_qsort_once(void *array, size_t left, size_t right, size_t type,
                  int (*)(const void *,const void *));

void k_qsort_once(void *array, size_t left, size_t right, size_t type,
                  int (*cmp)(const void *a, const void *b)) {
  if(left < right) {
// 需要一个深拷贝
    void *mid = calloc(1, type);
    memcpy(mid, array + ((left + right) / 2) * type, type);
    size_t i = left - 1;
    size_t j = right + 1;
    // printf("[%c]\n", *(char *)mid);
    while(1) {
      // 从左往右找到一个大于等于中间轴的 停止
      // while(array[++i] < s); 包括了等于的情况 这是不合理的
      // 以前在实现快排的时候用的是值比较 而现在的快排则是比较内存处的值大小 对比较函数的返回值细分 分为 大于 等于 小于 三类
      while(cmp(array + (++i)*type, mid) < 0);
      // 从右往左找到一个 小于等于中间轴的 停止
			// while(array[--j] > s);
      while(cmp(array + (--j)*type, mid) > 0);
      if(i >= j) {
        break;
      }
      k_swap(array + i * type, array + j * type, type);
    }
    free(mid);
    k_qsort_once(array, left, i - 1, type, cmp);
    k_qsort_once(array, j + 1, right, type, cmp);
  }
}

void k_qsort(void *base, size_t len, size_t width, int (*cmp)(const void *a, const void *b)) {
  k_qsort_once(base, 0, len - 1, width, cmp);
}

//内存处的值的交换 当做字节来处理  int是4字节 double是8字节 所以需要传递一个type参数
void k_swap(void *a,  void *b, unsigned int type) {
  int i = 0;

  while(type > 0) {
    char temp = *((char*)a + i);
    *((char*)a + i) = *((char*)b + i);
    *((char*)b + i) = temp;
    ++i;
    type--;
  }
}

#endif
