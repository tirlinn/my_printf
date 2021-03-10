#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

char* my_str(char* p1)
{
    if(p1 == NULL)
        return "(null)";
    else
        return p1;
}

int my_strlen(char* p1)
{
    int i = 0;
    while(p1[i++] != '\0');
    return --i;
}

void my_strcpy(char* p1, char* p2)
{
    char *out = p1;
    while( (*p1++ = *p2++) );
    p1 = out;
}

void my_strcat(char* p1, char* p2)
{
    char* out = p1 + my_strlen(p1);
    while ( (*out++ = *p2++) );
}

void my_char(char* p1, char p2)
{
    p1[0] = p2;
    p1[1] = '\0';
}

void my_put_int(char* p1, long int p2, int base, bool sign)
{
    unsigned long int p3;
    char out_0[16];
    char* out = &out_0[15];
    *out = '\0';

    if (p2 < 0 && sign )
        p3 = -p2;
    else if ( p2 < 0 )
        p3 = (unsigned int) p2;
    else
        p3 = p2;

    do
    {
        if (p3 % base > 9)
            *--out = '0' + 39 + p3 % base;
        else 
            *--out = '0' + p3 % base;
        p3 /= base;
    }
    while ( p3 );

    if (p2 < 0 && sign )
        *--out = '-';

    my_strcpy(p1 , out);
}

void my_put_addr(char* p1, void* p2)
{
    unsigned long int p3 = (unsigned long int) p2;
    char out_0[33];
    my_strcpy(p1, "0x");
    my_put_int( out_0, p3, 16 , false);
    my_strcat(p1, out_0);
}

int my_printf(char * restrict format, ...)
{
    int out_num;
    char* out = malloc(250*sizeof(char));
    char* out_free = malloc(251*sizeof(char));
    out[0] = '\0';

    va_list a_list;

    va_start( a_list, format );
        for (int i = 0; format[i] ; i++)
        {
            if (format[i] == '%')
            {
                i++;
                switch (format[i]) 
                {
                    case 'd':
                        my_put_int( out_free, va_arg(a_list, int), 10 , true); 
                        my_strcat( out , out_free );
                        break;
                    case 'o': 
                        my_put_int( out_free, va_arg(a_list, int), 8 , false);
                        my_strcat( out , out_free );
                        break;
                    case 'u':
                        my_put_int( out_free, va_arg(a_list, int), 10 , false); 
                        my_strcat( out , out_free );
                        break;
                    case 'x':
                        my_put_int( out_free, va_arg(a_list, int), 16 , false); 
                        my_strcat( out , out_free );
                        break;
                    case 'p': 
                        my_put_addr( out_free, va_arg(a_list, void*) );
                        my_strcat( out , out_free );
                        break;
                    case 's': 
                        my_strcat( out , my_str( va_arg(a_list, char*) ) );
                        break;
                    case 'c':
                        my_char( out_free, va_arg(a_list, int ) );
                        my_strcat( out, out_free );
                        break;         
                }
            }
            else 
            {
                my_char( out_free, format[i] );
                my_strcat( out, out_free );
            }
        }
    va_end (a_list);

    out_num = my_strlen(out);

    write(1, out, out_num);

    free(out_free);
    free(out);

    return out_num;
}

int main()
{
    int s = 3213213;
    void* s_p = &s;
    int a = my_printf(  "Hello %c %o %u %x %d %p %s %s\n", 'H', s, s, s, s, s_p, "Done", (char*)NULL);
    int b = printf(     "Hello %c %o %u %x %d %p %s %s\n", 'H', s, s, s, s, s_p, "Done", (char*)NULL);
    printf("%d %d\n", a, b);

    return 0;
}