
#include <stdio.h>
#include <stdarg.h> // va_arg
#include <stdint.h> // uint32_t

static inline int print_hex(char *buffer, uint32_t x, char _case)
{
    int i, len = 0;
    char digits[10];

    while(x > 15) {
        digits[len++] = x % 16;
        x /= 16;
    }

    digits[len++] = x;
    i=len;
    while(i--) {
        int c = digits[i];
        if(c < 10)
            c+='0';
        else
            c+=_case - 10;
        *buffer++=c;
    }

    *buffer = '\0';

    return len;
}

static inline int print_hex_small(char *buffer, int x)
{
    return print_hex(buffer, x, 'a');
}

static inline int print_hex_big(char *buffer,int X)
{
    return print_hex(buffer, X, 'A');
}

static inline int print_string(const char *s)
{
    int len = 0;
    while(s[len])
    {
        putchar(s[len++]);
    }
    return len;
}

static inline int print_decimal(char *buffer, int d)
{
    int i, len;
    len = 0;
    int digits[10];

    int sign = d < 0;
    
    if(sign)
        d = -1 * d;

    while(d > 9) {
        digits[len++] = d % 10;
        d /= 10;
    }

    digits[len++] = d;

    i = len;
    
    if(sign) {
        *buffer++ = '-';
        len++;
    }

    while(i)
        *buffer++ = digits[--i] + '0';

    *buffer='\0';
    return len;
}

int printf(const char * restrict format, ...)
{
    char buffer[11];

    va_list parameters;
    va_start(parameters, format);
    
    int written = 0;

    while(format[0]) {

            if(format[0] != '%') {
                putchar(*format++);
                written++;
            } else {

                switch(format[1]) {
                    case 'c':
                        int c = va_arg(parameters, int);
                        putchar(c);
                        written++;
                    break;
                    case 's':
                        char *s = va_arg(parameters, char *);
                        written += print_string(s);
                    break;
                    case 'd':
                        int d = va_arg(parameters, int);
                        print_decimal(buffer, d);
                        written += print_string(buffer);
                    break;
                    case 'x':
                        int x = va_arg(parameters, uint32_t);
                        print_hex_small(buffer, x);
                        written += print_string(buffer);
                        break;
                    case 'X':
                        int X = va_arg(parameters, uint32_t);
                        print_hex_big(buffer, X);
                        written += print_string(buffer);
                        break;
                    default:
                        putchar(format[1]);
                        written++;
                        break;
                }

                // 1 for % symbol, 1 for type or % literal
                format+=2;
            }

    }

    va_end(parameters);
    return written;
}

int sprintf(char *restrict s, const char *restrict format, ...)
{
    char *start = s;
    va_list parameters;
    va_start(parameters, format);
    while(format[0]) {

            if(format[0] != '%') {
                *s++ = *format++;
            } else {

                switch(format[1]) {
                    case 'c':
                        char c = va_arg(parameters, int);
                        *s++=c;
                    break;
                    case 's':
                        char *str = va_arg(parameters, char *);
                        while(*str)
                            *s++=*str++;
                    break;
                    case 'd':
                        int d = va_arg(parameters, int);
                        s+=print_decimal(s, d);
                    break;
                    case 'x':
                        int x = va_arg(parameters, uint32_t);
                        s+=print_hex_small(s, x);
                        break;
                    case 'X':
                        int X = va_arg(parameters, uint32_t);
                        s+=print_hex_big(s, X);
                        break;
                    default:
                        *s++=format[1];
                        break;
                }

                // 1 for % symbol, 1 for type or % literal
                format+=2;
            }

    }
    va_end(parameters);
    *s++='\0';
    return start - s;
}