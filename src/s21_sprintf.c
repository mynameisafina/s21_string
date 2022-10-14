#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

#include "s21_string.h"

char *s21_ulnginstr(unsigned long n) {
  char str[24];
  char *out;
  int i, j;
  i = 0;
  j = 0;
  if (n == 0) {
    str[i++] = '0';
  }
  while (n) {
    str[i++] = n % 10 + '0';
    n /= 10;
  }
  if ((out = (char *)malloc(sizeof(char) * (i + 1)))) {
    while (i--) {
      out[j++] = str[i];
    }
    out[j] = '\0';
  }
  return (out);
}

char *s21_itoa(long long n) {
  char str[24];
  char *out;
  int i, j, minus;
  i = 0;
  j = 0;
  minus = 0;
  if (n == 0) {
    str[i++] = '0';
  }
  if (n == LONG_MIN) {
    n /= 10;
    str[i++] = 8 + '0';
  }
  if (n == -2147483648) {
    n /= 10;
    str[i++] = 8 + '0';
  }
  if (n < 0) {
    n *= -1;
    minus++;
  }
  while (n) {
    str[i++] = n % 10 + '0';
    n /= 10;
  }
  if (minus) {
    str[i++] = '-';
  }
  if ((out = (char *)malloc(sizeof(char) * (i + 1)))) {
    while (i--) {
      out[j++] = str[i];
    }
    out[j] = '\0';
  }
  return (out);
}

void s21_reverse_array(char *a, int n) {
  int l = n % 2 == 0 ? n / 2 : n / 2 + 1;
  for (int i = 0; i < l; i++) {
    char tmp = a[i];
    int j = i + 1;
    a[i] = a[n - j];
    a[n - j] = tmp;
  }
}

void s21_clear(char *str_clr) {
  while (*str_clr) {
    *str_clr++ = 0;
  }
}

char *s21_float(long double num, int n) {
  int ro = 1;
  if (n >= 19) {
    ro = 0;
  }
  static char str[1024];
  static char str_res[1024];
  s21_clear(str);
  s21_clear(str_res);
  int minus = 0;
  int round1 = 0;
  if (num == 0) {
    int tmp_acc = n;
    str[0] = '0';
    str[1] = '.';
    int i = 2;
    while (tmp_acc--) {
      str[i++] = '0';
    }
  }
  if (signbit(num)) {
    num = (-1) * num;
    minus = 1;
  }
  long double tmp_flt;
  tmp_flt = num;
  long long tmp_int = (long long)tmp_flt;
  int tmp_acc = n;
  int j = 0;
  long double reminder = num - tmp_int;
  reminder = reminder * 10;
  if (n >= 19) {
    reminder = reminder / 10;
    n = n - 1;
  }
  while (tmp_acc > 0) {
    reminder *= 10;
    tmp_acc--;
  }
  long long k = reminder;
  if (n == 0) {
    double a = 1;
    reminder = reminder / 10;
    a = a - reminder;
    if (!(tmp_int % 2) && round(a) == 1) {
      k = 4;
    }
  }
  while ((n + 1) > 0) {
    str[j] = (k % 10) + '0';
    k /= 10;
    n--;
    j++;
  }
  str[j] = '.';
  if (tmp_int == 0) {
    str[j + 1] = '0';
  }
  while (tmp_int != 0) {
    str[(j + 1)] = (tmp_int % 10) + '0';
    j++;
    tmp_int /= 10;
  }
  int y = (int)s21_strlen(str);
  s21_reverse_array(str, y);
  if (ro) {
    if (str[(y - 1)] >= '5' && str[(y - 1)] <= '9') {
      int h = y - 2;
      while (h != -1) {
        if (str[h] == '9') {
          str[h] = '0';
          h--;
          if (h == -1) {
            round1 = 1;
          }
        } else {
          if (str[h] == '.') {
            h = h - 1;
          }
          if (str[h] != '9') {
            str[h] = str[h] + 1;
            h = -1;
          }
        }
      }
    }
  }
  if (minus) {
    s21_strcat(str_res, "-");
  }
  if (round1) {
    s21_strcat(str_res, "1");
  }
  s21_strcat(str_res, str);

  int f = (int)s21_strlen(str_res);
  if (ro) {
    str_res[f - 1] = '\0';
  }
  if (str_res[f - 2] == '.') {
    str_res[f - 2] = '\0';
  }
  return str_res;
}

long shrt(long num) {
  while (num >= 65536) {
    num %= 65536;
  }
  while (num < 0) {
    num %= 65536;
  }
  return (num);
}

char *s21_format_str(char *str, char *current, s_flags flag,
                     unsigned long long *len_out) {
  int i = 0, Length = (int)s21_strlen(current);
  char tmp_str[Length + 1];
  while (*current) {
    tmp_str[i++] = *current++;
  }
  tmp_str[i] = '\0';
  if (flag.accuracy) {
    if (flag.accuracy < Length && flag.accuracy != -1) {
      Length = flag.accuracy;
      tmp_str[flag.accuracy] = '\0';
    }
  } else if (flag.accuracy == -1) {
    flag.width -= Length;
    tmp_str[Length] = '\0';
  } else {
    tmp_str[0] = '\0';
    Length = 0;
  }
  i = 0;
  if (flag.minus) {
    while (tmp_str[i]) {
      *str++ = tmp_str[i++];
      *len_out += 1;
    }
    if (flag.width > Length) {
      flag.width -= Length;
      while (flag.width) {
        *str++ = ' ';
        *len_out += 1;
        flag.width--;
      }
    } else {
      while (tmp_str[i]) {
        *str++ = tmp_str[i++];
        *len_out += 1;
      }
    }
  }
  if (flag.width > Length) {
    flag.width -= Length;
    while (flag.width) {
      *str++ = ' ';
      *len_out += 1;
      flag.width--;
    }
    while (tmp_str[i]) {
      *str++ = tmp_str[i++];
      *len_out += 1;
    }
  } else {
    while (tmp_str[i]) {
      *str++ = tmp_str[i++];
      *len_out += 1;
    }
  }
  return (str);
}

char *s21_format_chr(char *str, char current, s_flags flag,
                     unsigned long long *len_out) {
  if (flag.width) {
    flag.width -= 1;
  }
  if (!flag.minus) {
    while (flag.width--) {
      *str++ = ' ';
      *len_out += 1;
    }
    *str++ = current;
    *len_out += 1;
  } else {
    *str++ = current;
    *len_out += 1;
    while (flag.width--) {
      *str++ = ' ';
      *len_out += 1;
    }
  }
  return (str);
}
char *s21_format_int(char *str, int num, s_flags flag,
                     unsigned long long *len_out) {
  int length, minus = 0;
  char *current;
  current = s21_itoa(num);
  char *free_pointer = current;
  length = s21_strlen(current);
  if (num == 0 && flag.accuracy == 0) {
    flag.width++;
  }
  if (num < 0) {
    minus = 1;
    current++;
    length--;
    flag.width--;
    flag.plus = 0;
    flag.space = 0;
  }
  if (flag.space && !minus && !flag.plus) {
    *str++ = ' ';
    *len_out += 1;
  }
  if (flag.space && flag.plus) {
    flag.space = 0;
  }
  if (!flag.accuracy && !num) {
    *current = '\0';
  }
  if (flag.accuracy < length) {
    flag.accuracy = length;
  }
  if (flag.accuracy >= length) {
    if (!flag.minus) {
      while (flag.width-- > (flag.accuracy + flag.plus + flag.space)) {
        *str++ = ' ';
        *len_out += 1;
      }
      if (minus) {
        *str++ = '-';
        *len_out += 1;
      } else if (flag.plus && num >= 0) {
        *str++ = '+';
        *len_out += 1;
      }
      while (flag.accuracy > length++) {
        *str++ = '0';
        *len_out += 1;
      }
      while (*current) {
        *str++ = *current++;
        *len_out += 1;
      }
    } else {
      if (minus) {
        *str++ = '-';
        *len_out += 1;
      }
      if (flag.plus && num >= 0) {
        *str++ = '+';
        *len_out += 1;
      }
      while (flag.accuracy > length++) {
        *str++ = '0';
        *len_out += 1;
      }
      while (*current) {
        *str++ = *current++;
        *len_out += 1;
      }
      while (flag.width-- > (flag.accuracy + flag.plus + flag.space)) {
        *str++ = ' ';
        *len_out += 1;
      }
    }
  }
  if (free_pointer) free(free_pointer);
  return (str);
}

char *s21_format_lng(char *str, long num, s_flags flag,
                     unsigned long long *len_out) {
  int length, minus = 0;
  char *current;
  if (flag.shrt) {
    num = shrt(num);
  }
  current = s21_itoa(num);
  char *free_pointer = current;
  length = s21_strlen(current);
  if (num == 0 && flag.accuracy == 0) {
    flag.width++;
  }
  if (num < 0) {
    minus = 1;
    current++;
    length--;
    flag.width--;
    flag.plus = 0;
    flag.space = 0;
  }
  if (flag.space && !minus && !flag.plus) {
    *str++ = ' ';
    *len_out += 1;
  }
  if (flag.space && flag.plus) {
    flag.space = 0;
  }
  if (!flag.accuracy && !num) {
    *current = '\0';
  }
  if (flag.accuracy < length) {
    flag.accuracy = length;
  }
  if (flag.accuracy >= length) {
    if (!flag.minus) {
      while (flag.width-- > (flag.accuracy + flag.plus + flag.space)) {
        *str++ = ' ';
        *len_out += 1;
      }
      if (minus) {
        *str++ = '-';
        *len_out += 1;
      } else if (flag.plus && num >= 0) {
        *str++ = '+';
        *len_out += 1;
      }
      while (flag.accuracy > length++) {
        *str++ = '0';
        *len_out += 1;
      }
      while (*current) {
        *str++ = *current++;
        *len_out += 1;
      }
    } else {
      if (minus) {
        *str++ = '-';
        *len_out += 1;
      }
      if (flag.plus && num >= 0) {
        *str++ = '+';
        *len_out += 1;
      }
      while (flag.accuracy > length++) {
        *str++ = '0';
        *len_out += 1;
      }
      while (*current) {
        *str++ = *current++;
        *len_out += 1;
      }
      while (flag.width-- > (flag.accuracy + flag.space + flag.plus)) {
        *str++ = ' ';
        *len_out += 1;
      }
    }
  }
  if (free_pointer) free(free_pointer);
  return (str);
}

char *s21_format_ulng(char *str, unsigned long num, s_flags flag,
                      unsigned long long *len_out) {
  int length;
  char *current;
  if (flag.shrt) {
    num = shrt((unsigned)num);
  }
  if (flag.lng) {
    current = s21_ulnginstr(num);
  } else {
    current = s21_ulnginstr((unsigned)num);
  }
  char *free_pointer = current;
  length = s21_strlen(current);
  if (num == 0 && flag.accuracy == 0) {
    flag.width++;
  }
  if (!flag.accuracy && !num) {
    *current = '\0';
  }
  if (flag.accuracy < length) {
    flag.accuracy = length;
  }
  if (flag.accuracy >= length) {
    if (!flag.minus) {
      while (flag.width-- > (flag.accuracy + flag.plus + flag.space)) {
        *str++ = ' ';
        *len_out += 1;
      }
      while (flag.accuracy > length++) {
        *str++ = '0';
        *len_out += 1;
      }
      while (*current) {
        *str++ = *current++;
        *len_out += 1;
      }
    } else {
      while (flag.accuracy > length++) {
        *str++ = '0';
        *len_out += 1;
      }
      while (*current) {
        *str++ = *current++;
        *len_out += 1;
      }
      while (flag.width-- > (flag.accuracy + flag.space + flag.plus)) {
        *str++ = ' ';
        *len_out += 1;
      }
    }
  }
  if (free_pointer) free(free_pointer);
  return (str);
}

char *s21_format_flt(char *tmp_str, char *str, double num, s_flags flag,
                     int len, unsigned long long *len_out) {
  int length_width;
  if (signbit(num)) {
    flag.plus = 0;
    flag.space = 0;
  }
  if (flag.space && flag.plus) flag.space = 0;
  length_width = (flag.width - len - flag.space - flag.plus);
  if (!flag.minus && length_width >= 0) {
    while (length_width--) {
      *str++ = ' ';
      *len_out += 1;
    }
  }
  if (flag.space && !(signbit(num))) {
    *str++ = ' ';
    *len_out += 1;
  } else if (flag.plus && !(signbit(num))) {
    *str++ = '+';
    *len_out += 1;
  }
  while (*tmp_str) {
    *str++ = *tmp_str++;
    *len_out += 1;
  }
  if (flag.minus && length_width >= 0) {
    while (length_width--) {
      *str++ = ' ';
      *len_out += 1;
    }
  }
  return (str);
}
void set_flag_begin(s_flags *flag) {
  flag->minus = 0;
  flag->plus = 0;
  flag->space = 0;
  flag->width = 0;
  flag->accuracy = -1;
  flag->lng = 0;
  flag->shrt = 0;
}

int s21_isdigit(char chr) {
  int flag = 0;

  if (chr >= '0' && chr <= '9') {
    flag = 1;
  }
  return (flag);
}
s_flags str_parse(char *parse, int length) {
  parse++;
  s_flags flag = {0, 0, 0, 0, -1, 0, 0};
  int i = 0, j = 0;
  char width[12], accuracy[12];
  while (length--) {
    j++;
    if (*parse == '-') {
      flag.minus = 1;
      parse++;
    } else if (*parse == ' ') {
      flag.space = 1;
      parse++;
    } else if (*parse == '+') {
      flag.plus = 1;
      parse++;
    } else if (s21_isdigit(*parse)) {
      while (s21_isdigit(*parse)) {
        width[i++] = *parse++;
      }
      width[i] = '\0';
      flag.width = atoi(width);
    } else if (*parse == '.') {
      parse++;
      if (s21_isdigit(*parse)) {
        i = 0;
        while (s21_isdigit(*parse)) {
          accuracy[i++] = *parse++;
        }
        accuracy[i] = '\0';
        flag.accuracy = atoi(accuracy);
      } else {
        flag.accuracy = 0;
      }
    } else if (*parse == 'l') {
      flag.lng = 1;
      parse++;
    } else if (*parse == 'h') {
      flag.shrt = 1;
      parse++;
    }
  }
  return (flag);
}

int s21_sprintf(char *str, const char *format, ...) {
  int int_num, length;
  unsigned long long l_out = 0;
  unsigned long long *len_out = &l_out;
  long tmp_num;
  short shrt_num;
  char *parse, *current, chr;
  s_flags flag;
  double tmp_double;
  va_list args;
  va_start(args, format);

  while (*format) {
    parse = s21_NULL;
    length = 0;
    if (*format == '%') {
      parse = (char *)format;
      length++;
      format++;
      if (*format) {
        while (*format != 's' && *format != 'c' && *format != 'd' &&
               *format != 'f' && *format != 'i' && *format != 'u' &&
               *format != '%') {
          format++;
          length++;
        }
      }
      if (*format == '%') {
        flag = str_parse(parse, length);
        if (flag.minus && flag.width) {
          flag.width--;
          *str++ = *format++;
          *len_out += 1;
          while (flag.width--) {
            *str++ = ' ';
            *len_out += 1;
          }
        } else if (!flag.minus && flag.width) {
          flag.width--;
          while (flag.width--) {
            *str++ = ' ';
            *len_out += 1;
          }
          *str++ = *format++;
          *len_out += 1;
        } else {
          *str++ = *format++;
          *len_out += 1;
        }
        set_flag_begin(&flag);
      } else if (*format == 's') {
        format++;
        flag = str_parse(parse, length);
        if (flag.lng) {
          char *tmp_str;
          if ((tmp_str = calloc(2048, 1))) {
            wchar_t *lng_str = va_arg(args, wchar_t *);
            (void)wcstombs(tmp_str, lng_str, 2048);
            str = s21_format_str(str, tmp_str, flag, len_out);
            free(tmp_str);
          }
        } else {
          current = va_arg(args, char *);
          if (current == s21_NULL) current = "(null)";
          str = s21_format_str(str, current, flag, len_out);
        }
        set_flag_begin(&flag);
      } else if (*format == 'c') {
        format++;
        flag = str_parse(parse, length);
        if (flag.lng) {
          if (flag.accuracy == 0) flag.accuracy = 3;
          char tmp_str[4];
          wchar_t lng_chr = va_arg(args, wchar_t);
          (void)wcstombs(tmp_str, &lng_chr, 4);
          str = s21_format_str(str, tmp_str, flag, len_out);
        } else {
          chr = va_arg(args, int);
          str = s21_format_chr(str, chr, flag, len_out);
        }
        set_flag_begin(&flag);
      } else if (*format == 'f') {
        format++;
        flag = str_parse(parse, length);
        if (flag.accuracy == -1) flag.accuracy = 6;
        tmp_double = va_arg(args, double);
        char *tmp_str = s21_float(tmp_double, flag.accuracy);
        int len = s21_strlen(tmp_str);
        str = s21_format_flt(tmp_str, str, tmp_double, flag, len, len_out);
        set_flag_begin(&flag);
      } else if (*format == 'i' || *format == 'd') {
        format++;
        flag = str_parse(parse, length);
        if (flag.lng) {
          tmp_num = va_arg(args, long);
          str = s21_format_lng(str, tmp_num, flag, len_out);
        } else if (flag.shrt) {
          shrt_num = (short)va_arg(args, int);
          str = s21_format_int(str, shrt_num, flag, len_out);
        } else {
          int_num = va_arg(args, int);
          str = s21_format_int(str, int_num, flag, len_out);
        }
        set_flag_begin(&flag);
      } else if (*format == 'u') {
        format++;
        flag = str_parse(parse, length);
        if (flag.space || flag.plus) {
          flag.space = 0;
          flag.plus = 0;
        }
        tmp_num = va_arg(args, long);
        str = s21_format_ulng(str, tmp_num, flag, len_out);
        set_flag_begin(&flag);
      }
    }
    if (*format && *format != '%') {
      *str++ = *format++;
      *len_out += 1;
    }
  }
  *str = '\0';
  va_end(args);
  return (l_out);
}
