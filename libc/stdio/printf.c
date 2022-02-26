
#include <stdio.h>
#include <stdarg.h> // va_arg
#include <stdint.h> // uint32_t

static const char *small_hex_symbols = "0123456789abcdef";
static const char *big_hex_symbols = "0123456789ABCDEF";

static inline int print_hex(uint32_t x, const char *symbols)
{
    int i, len;
    len = 0;
    int digits[10];

    while(x > 15) {
        digits[len++] = x % 16;
        x /= 16;
    }

    digits[len++] = x;

    i = len;
    
    while(i)
        putchar(symbols[digits[--i]]);

    return len;
}

static inline int print_hex_small(int x)
{
    return print_hex(x, small_hex_symbols);
}

static inline int print_hex_big(int X)
{
    return print_hex(X, big_hex_symbols);
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

static inline int print_decimal(int d)
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
        putchar('-');
        len++;
    }

    while(i)
        putchar(digits[--i] + '0');

    return len;
}

int printf(const char * restrict format, ...)
{
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
                        written += print_decimal(d);
                    break;
                    case 'x':
                        int x = va_arg(parameters, uint32_t);
                        written += print_hex_small(x);
                        break;
                    case 'X':
                        int X = va_arg(parameters, uint32_t);
                        written += print_hex_big(X);
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