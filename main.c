#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <string.h>	//strlen
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

#include "getString.h"
#include "iot_access.h"
#include "sslAccess.h"

#define HOST "phtresources.com"
#define PORT "443"
int main() 
{
	FILE *fp;
	fp  = fopen("data.log", "a");
    //  Initialize SSL the variables

    //  Initialize Http the variables
	char write_buf[500] = {0};
	char jsonData[200] = {0};
	char errorMessage[100] = {0};
	char ioT_Server_Reply[1000];
    char temp1[5] = { 0 };
    char temp2[5] = { 0 };
    char temp3[5] = { 0 };
    char temp4[5] = { 0 };
    char hum1[5] = { 0 };
    char hum2[5] = { 0 };
    char hum3[5] = { 0 };
    char hum4[5] = { 0 };
	char webResponse[1024];
	if(getIotResponse(ioT_Server_Reply,  errorMessage))
	{
		fprintf(fp, "%s", errorMessage);
	}
	if (!isValid(ioT_Server_Reply, temp1, 'T', '1'))
	{
		convertToFahrenheit(temp1);
	}
	if (!isValid(ioT_Server_Reply, temp2, 'T', '2'))
	{
		convertToFahrenheit(temp2);
	}	
	if (!isValid(ioT_Server_Reply, temp3, 'T', '3'))
	{
		convertToFahrenheit(temp3);
	}
	if (!isValid(ioT_Server_Reply, temp4, 'T', '4'))
	{
		convertToFahrenheit(temp4);
	}	
	if (!isValid(ioT_Server_Reply, hum1, 'V', '1'))
	{
		convertToHumidity(hum1);
	}
	if (!isValid(ioT_Server_Reply, hum2, 'V', '2'))
	{
		convertToHumidity(hum2);
	}	
	if (!isValid(ioT_Server_Reply,hum3, 'V', '3'))
	{
		convertToHumidity(hum3);
	}
	if (!isValid(ioT_Server_Reply, hum4, 'V', '4'))
	{
		convertToHumidity(hum4);
	}	

 
	//char jsonData[200] = {"{\"temp1\": 21,\"temp2\": 22,\"temp3\": 23,\"temp4\": 24,\"hum1\": 25,\"hum2\": 26,\"hum3\": 27,\"hum4\": 28}"};
	
	sprintf(jsonData, "{\"temp1\": %s,\"temp2\": %s,\"temp3\": %s,\"temp4\":"
	" %s,\"hum1\": %s,\"hum2\": %s,\"hum3\": %s,\"hum4\": %s}",
	temp1, temp2, temp3, temp4, hum1, hum2, hum3, hum4);
	
	//   HTTP POST request.

	makeMessage(jsonData, write_buf);

	connectWithServer(write_buf, webResponse);

 
    return 0;
}