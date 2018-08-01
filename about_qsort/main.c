#include<stdio.h>

#include"k_qsort.h"

//一些数据类型的比较函数
int compare_Int(const void *x, const void *y);
int compare_double(const void *x, const void *y);

// 数据打印函数 考虑是否可以将不同类型的打印函数做成一个打印函数 借助void*指针
void show_int(const int *, const int);
void show_double(const double *, const int);
int compare_double(const void *x, const void *y);
int compare_char(const void *x, const void *y);

//简单的假设 这样的效率并不是很高 循环中有判断
void showAllType(const void *array, size_t width, size_t len);

void showAllType(const void *array, size_t width, size_t len) {
  for(size_t i = 0; i < len; i++) {
    if(width == sizeof(char)) {
      printf("%c ", *((char *)array + i));
    }else if(width == sizeof(double)) {
      printf("%lf ", *((double *)array + i));
    }else {
      printf("%d ", *((int *)array + i));
    }
  }
  printf("\n");
}


int compare_char(const void *x, const void *y) {
  int result = *(char *)x - *(char *)y;

// printf("result = %d\n", result);

  if(0 == result) {
    return 0;
  }

  return result;
}

int compare_double(const void *x, const void *y) {
  double result = *(double *)x - *(double *)y;

  if(0.0 == result) {
    return 0;
  }

  return result > 0.0 ? 1 : -1;
}

// 自己提供一个比较函数 等于返回0 大于返回正数 小于返回负数
int compare_Int(const void *x, const void *y) {
  int result = *(int *)x - *(int *)y;

  if(0 == result) {
    return 0;
  }

  return result;
}

int main() {
  int array[] = {4,1,2,5,6,3,9,8,7};
  int len1 = sizeof(array) / sizeof(int);

  double array2[] = {4.1, 2.1, 5.2, 1.2, 0.1, 4.5};
  int len2 = sizeof(array2) / sizeof(double);

  char array3[] = {"jisfjiej.*-4849grig/grkgr\fiej"};
  int len3 = strlen(array3);

printf("len3 = %d\n", len3);
  k_qsort(array, len1, sizeof(int), compare_Int);
  k_qsort(array2, len2, sizeof(double), compare_double);
  k_qsort(array3, len3, sizeof(char), compare_char);

  puts(array3);
  show_int(array, len1);
  show_double(array2, len2);

  return 0;
}

void show_int(const int *array, const int len) {
  for(int i = 0; i < len; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

void show_double(const double *array, const int len) {
  int i;

  for(i = 0; i < len; i++) {
    printf("%lf ", array[i]);
  }
  printf("\n");
}
