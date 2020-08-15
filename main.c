#include <stdio.h>
#include <stdlib.h>

int lastDigit7base(int x) {
  if (x == 0)
    return 0;
  
  while ((x % 7) == 0) {
    x = x / 7;
  }
  return x % 7;
}

int lastDigitFactorial7base(int n) {
  if (n < 2)
    return 1;
  
  int m = 1;
  for (int i = 2; i <= n; i++) {
    m = lastDigit7base(m) * lastDigit7base(i);
  }
  return lastDigit7base(m);
}

int lastDigitFactorial7base_log_approach(int n) {
  if (n <= 7)
    return lastDigitFactorial7base(n);

  int len = 1;
  long exp = 7;
  while (exp * 7 <= n) {
    exp = exp * 7;
    len++;
  }

  int* arr = malloc(len * sizeof(int));
  int* seven_pow = malloc(len * sizeof(int));
  arr[0] = lastDigitFactorial7base(7);
  seven_pow[0] = 7;
  for (int i = 1; i < len; i++) {
    int m = 1;
    for (int j = 1; j <= 7; j++) {
      m = arr[i - 1] * lastDigit7base(m);
    }
    arr[i] = lastDigit7base(lastDigit7base(m) * lastDigitFactorial7base(7)); 
    // With some unbelievable luck, I figured this out ^^
    // Basically: 7! * 6! = 13! mod 7
    //            7! * 7! != 14! mod 7
    //            7! * 7! * 2 = 14! mod 7 (due to rounding up 14deci = 20 base 7)
    seven_pow[i] = seven_pow[i - 1] * 7;
  }
  
  int n_modify = n;
  int result = 1;
  int index = len - 1;
  int roundup = 1;
  while (index >= 0) {
    
    if (n_modify >= seven_pow[index]) {
      n_modify = n_modify - seven_pow[index];

      // The roundup here is explained in line 48's comments
      result = lastDigit7base(result * arr[index] * roundup);
      roundup++;
    } else {
      index--;
      roundup = 1;
    }
  }
  free(arr);
  free(seven_pow);

  return lastDigit7base(result * lastDigitFactorial7base(n_modify));
}

unsigned long long* num2sevenbase(unsigned long long num) {
  // assume number of digits won't exceed 63
  // zero index is for the number of digits
  if (num <= 0)
    return 0;

  unsigned long long* ptr;
  ptr = malloc(64 * sizeof(unsigned long long));
  int index = 1;
  while (num > 0) {
    ptr[index] = num % 7;
    num = num / 7;
    index++;
  }
  ptr[0] = index - 1;
  return ptr;
}

void print7base(unsigned long long* ptr) {
  for (int i = ptr[0]; i > 0; i--) {
    printf("%llu", ptr[i]);
  }
  printf("\n");
}

unsigned long long factorial(unsigned long long n) {
  if (n < 2)
    return 1;

  unsigned long long m = 1;
  for (unsigned long long i = 2; i <= n; i++) {
    m = m * i;
  }
  return m;
}

int main(void) {
  // 20,0000,0000
  
  for (int i = 1; i <= 14; i++) {
    printf("%d!: ", i);
    printf("O(n) algorithm %d, O(log(n)) algorithm %d, ", lastDigitFactorial7base(i), lastDigitFactorial7base_log_approach(i));
    printf("correct result: ");
    unsigned long long fact = factorial(i);
    unsigned long long *ptr = num2sevenbase(fact);
    print7base(ptr);
    free(ptr);
    
  }

  
  for (int i = 49; i <= 343; i++) {
    printf("%d!: ", i);
    printf("O(n) algorithm %d, O(log(n)) algorithm %d\n", lastDigitFactorial7base(i), lastDigitFactorial7base_log_approach(i));

  }

  int i = 2000000000;
  printf("%d!: ", i);
  printf("O(log(n)) algorithm %d\n", lastDigitFactorial7base_log_approach(i));

  // // Debug section
  // // Check if two algorithms denote the same result
  // int test = 1;
  // int lin, logn;
  // do {
  //   lin = lastDigitFactorial7base(test);
  //   logn = lastDigitFactorial7base_log_approach(test);
  //   printf("%d pass\n", test);
  //   test++;
  // } while (lin == logn || test < 100000);
  // printf("%d fail\n", (test - 1));

  return 0;
}

