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

#define HOST "phtresources.com"
#define PORT "443"
int main() 
{
	FILE *fp;
	fp  = fopen("data.log", "w");
    //  Initialize SSL the variables
    BIO* bio;
    SSL_CTX* ctx;
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
	//char body[100] = { 0 };	
	if(getIotResponse(ioT_Server_Reply,  errorMessage))
	{
		fprintf(fp, "%s", errorMessage);
	}
	if (!isValid(ioT_Server_Reply, temp1, 'T', '1'))
	{
		convertToFahrenheit(temp1);
		//puts(temp1);
	}
	if (!isValid(ioT_Server_Reply, temp2, 'T', '2'))
	{
		convertToFahrenheit(temp2);
		//puts(temp2);
	}	
	if (!isValid(ioT_Server_Reply, temp3, 'T', '3'))
	{
		convertToFahrenheit(temp3);
		//puts(temp3);
	}
	if (!isValid(ioT_Server_Reply, temp4, 'T', '4'))
	{
		convertToFahrenheit(temp4);
		//puts(temp4);
	}	
	if (!isValid(ioT_Server_Reply, hum1, 'V', '1'))
	{
		convertToHumidity(hum1);
		//puts(hum1);
	}
	if (!isValid(ioT_Server_Reply, hum2, 'V', '2'))
	{
		convertToHumidity(hum2);
		//puts(hum2);
	}	
	if (!isValid(ioT_Server_Reply,hum3, 'V', '3'))
	{
		convertToHumidity(hum3);
		//puts(hum3);
	}
	if (!isValid(ioT_Server_Reply, hum4, 'V', '4'))
	{
		convertToHumidity(hum4);
		//puts(hum4);
	}	

	printf("Getting ready to access to web server\n");	
	//   Registers the SSL/TLS ciphers and digests.
	//   Basically start the security layer.
	SSL_library_init();
	//  Creates a new SSL_CTX object as a framework to establish TLS/SSL
	//  or DTLS enabled connections
	ctx = SSL_CTX_new(SSLv23_client_method());
	//  -> Error check
	if (ctx == NULL)
	{
		fprintf(fp, "%s", "Ctx is null\n");
	}
	//   Creates a new BIO chain consisting of an SSL BIO
	bio = BIO_new_ssl_connect(ctx);


	//  Connect to web server.
	BIO_set_conn_hostname(bio, HOST ":" PORT);

	//   Attempts to connect the supplied BIO
	if(BIO_do_connect(bio) <= 0)
	{
		fprintf(fp, "%s", "Failed connection to web server\n");
		return 1;
	}
	else
	{
		fprintf(fp, "%s", "Connected to web server\n");
	}
	

	//char jsonData[200] = {"{\"temp1\": 21,\"temp2\": 22,\"temp3\": 23,\"temp4\": 24,\"hum1\": 25,\"hum2\": 26,\"hum3\": 27,\"hum4\": 28}"};
	
	sprintf(jsonData, "{\"temp1\": %s,\"temp2\": %s,\"temp3\": %s,\"temp4\":"
	" %s,\"hum1\": %s,\"hum2\": %s,\"hum3\": %s,\"hum4\": %s}",
	temp1, temp2, temp3, temp4, hum1, hum2, hum3, hum4);
	
	//   HTTP POST request.

	makeMessage(jsonData, write_buf);



	fprintf(fp, "\n new message\n");
	fprintf(fp, "%s", write_buf);
	fprintf(fp, "\n end of new message\n");

	int x = 0;
	x = BIO_write(bio, write_buf, strlen(write_buf));	
	//   Attempts to write len bytes from buf to BIO
	if(x <= 0)
	{
		//  Handle failed writes here
		if(!BIO_should_retry(bio))
		{
			// Not worth implementing, but worth knowing.
		}
		//  -> Let us know about the failed writes
		printf("Failed write to web server\n");
	}

	//  Variables used to read the response from the server
	int size;
	char buf[1024];
	//  Read the response message
	for(;;)
	{
		//  Get chunks of the response 1023 at the time.
		size = BIO_read(bio, buf, 1023);
		//  If no more data, then exit the loop
		if(size <= 0)
		{
			break;
		}
		//  Terminate the string with a 0, to let know C when the string 
		//  ends.
		buf[size] = 0;

		//  ->  Print out the response
		fprintf(fp, "%s", buf);
	}
	printf("Finished receiving Web Server Response\n");
	//  Clean after ourselves
	BIO_free_all(bio);
	SSL_CTX_free(ctx);
	//sleep(450);


	fclose(fp); 
    return 0;
}