#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h> // testing
#include <assert.h> // testing

void my_putchar(char c){
    write(1, &c, 1);
}

int my_putstr(char* s){
    if (!s){
        return 0;
    }
    int i = 0;
    while (s[i]){
        my_putchar(s[i++]);
    }
    return i;
}

char* convert_unsigned_base(unsigned int number, int base){
    unsigned int original_number = number;
    unsigned int len = 0;
    char* result = NULL;
    unsigned int temp_number;

    original_number = original_number > 0 ? original_number : -original_number;

    if (number != 0){
        while (number)
        {
            number = number / base;
            len++;
        }
        result = (char*)malloc(sizeof(char) * len + 1);
    }
    else {
        len = 1;
        result = (char*)malloc(sizeof(char) * (len + 1)); //2
    }
    if (!(result)){
        return NULL;
    }
    *(result + len) = '\0';
    len--;
    while (original_number > 0){
        temp_number = original_number % base;
        if (temp_number < 10)
            *(result + len) = temp_number + '0';
        else
            *(result + len) = ((temp_number)-10 + 97);
        original_number = original_number / base;
        len--;
    }
    if (len == 0 && result[1] == '\0'){
        *(result) = '0';
    }
    return result;
}

char* convert_signed_base(int number, int base){
    char* result;
    int len;
    int temp_number;
    int original_number = number;

    len = original_number > 0 ? 0 : 1;
    original_number = original_number > 0 ? original_number : -original_number;
    while (number){
        number = number / base;
        len++;
    }
    if (!(result = (char*)malloc(sizeof(char) * len + 1))){
        return NULL;
    }
    *(result + len) = '\0';
    len--;
    while (original_number > 0){
        temp_number = original_number % base;
        if (base == 16 && original_number < 0){
            temp_number = 15 - temp_number + 1;
        }
        if (temp_number < 10)
            *(result + len) = temp_number + '0';
        else
            *(result + len) = ((temp_number)-10 + 97);
        original_number = original_number / base;
        len--;
    }
    if (len == 0 && result[1] == '\0'){
        *(result + len) = '0';
    }
    else if (len == 0 && result[1] != '\0'){
        *(result + len) = '-';
    }
    return result;
}

char* convert_to_string(unsigned long int number, int base){
    unsigned long int original_number = number;
    unsigned int len = 0;
    char* result = NULL;
    unsigned long int temp_number;

    original_number = original_number > 0 ? original_number : -(original_number);
    while (number){
        number = number / base;
        len++;
    }
    if (!(result = (char*)malloc(sizeof(char) * len + 1))){
        return NULL;
    }
    *(result + len) = '\0';
    len--;
    while (original_number > 0){
        temp_number = original_number % base;
        if (temp_number < 10)
            *(result + len) = temp_number + '0';
        else
            *(result + len) = ((temp_number)-10 + 97);
        original_number = original_number / base;
        len--;
    }
    return result;
}

int my_printf(char* restrict format, ...){
    va_list args_p;
    char* sval;
    char* p;
    char* res_convert = NULL;
    intptr_t value_p;
    int len;
    int value;
    int written_chars = 0;

    va_start(args_p, format);
    for (p = format; *p; p++){
        if (*p != '%'){
            my_putchar(*p);
            written_chars++;
            continue;
        }
        switch (*++p){
            case 'd':{
                value = va_arg(args_p, int);
                res_convert = convert_signed_base(value, 10);
                len = my_putstr(res_convert);
                written_chars += len;
                break;
            }
            case 'o':{
                value = va_arg(args_p, int);
                res_convert = convert_unsigned_base(value, 8);
                len = my_putstr(res_convert);
                written_chars += len;
                break;
            }
            case 'u':{
                value = va_arg(args_p, int);
                res_convert = convert_unsigned_base(value, 10);
                len = my_putstr(res_convert);
                written_chars += len;
                break;
            }
            case 'x':{
                value = va_arg(args_p, int);
                res_convert = convert_unsigned_base(value, 16);
                len = my_putstr(res_convert);
                written_chars += len;
                break;
            }
            case 'c':{
                value = va_arg(args_p, int);
                my_putchar(value);
                written_chars++;
                break;
            }
            case 's':{
                sval = va_arg(args_p, char*);
                len = my_putstr(sval);
                written_chars += len;
                break;
            }
            case 'p':{
                value_p = va_arg(args_p, intptr_t);
                res_convert = convert_to_string(value_p, 16);
                write(1, "0x", 2);
                len = my_putstr(res_convert);
                written_chars += len + 2;
                break;
            }
        }
        if (res_convert != NULL){
            free(res_convert);
            res_convert = NULL;
        }
    }
    return written_chars;
    va_end(args_p);
}
