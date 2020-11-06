
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#define HOST "phtresources.com"

char serviceMethod[]="api/readings";
char requestBuilder[150]={0};

char isValid(const char *s, char *p, char first, char second)
{
    while (*s != 0)
    {
        if(*s == first)
        {
            s++;

            if(*s == second)
            {
                s += 4;

                for(int i =0; i < 4; i++)
                {
                    *p = *s;
                    p++;
                    s++;
                }
                p++;
                *p = '\0';
                return 0;
            }
        }
        s++;
    }
    return 1;
}



char makeMessage(char* jsonData, char* message)
{

    int len = strlen(jsonData);

    sprintf(message,  "POST /api/readings/ HTTP/1.1\r\n"
                    "Host: " HOST "\r\n"
                    "Connection: close\r\n"
                    "Accept: application/json\r\n"
                    "User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n" 
                    "content-type: application/json\r\n"
                    "Accept-Encoding: gzip\r\n"
                    "Content-Length: %d\r\n\r\n"
                    "%s\r\n", len, jsonData);
    return 0;
}

void convertToFahrenheit(char *in )
{
	double temperature = 0;
	char temporaryString[318] = {0};
	temperature = atof(in);
	temperature = (1.8 * temperature) + 32;
	sprintf(temporaryString, "%f", temperature);
	for(int i = 0; i < 5; i++)
	{
		*in = temporaryString[i];
		in++;
	}
	in++;
	*in = '\0';
}

void convertToHumidity(char *in)
{
	double humidity = 0;
	char temporaryString[318] = {0};
	humidity = atof(in);
	if(humidity != 0)
	{
		humidity /= 9.24;
		humidity = 90 - ((2.7 -humidity) / .03);
	}
	sprintf(temporaryString, "%f", humidity);
	for(int i = 0; i < 5; i++)
	{
		*in = temporaryString[i];
		in++;
	}
	in++;
	*in = '\0';
}