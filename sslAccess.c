#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>


#define HOST "phtresources.com"
#define PORT "443"

char connectWithServer(char* serverWriteBuffer, char* response)
{
    int size;
    BIO* bio;
    SSL_CTX* ctx;

    FILE *file;
	file  = fopen("data.log", "a");
    // fprintf(file, "\n new message\n");
	// fprintf(file, "%s", serverWriteBuffer);
	// fprintf(file, "\n end of new message\n");

    SSL_library_init();
    ctx = SSL_CTX_new(SSLv23_client_method());
    //  -> Error check
	if (ctx == NULL)
	{
		fprintf(file, "%s", "Ctx is null\n");
	}
	bio = BIO_new_ssl_connect(ctx);
	//  Connect to web server.
	BIO_set_conn_hostname(bio, HOST ":" PORT);

	//   Attempts to connect the supplied BIO
	if(BIO_do_connect(bio) <= 0)
	{
		fprintf(file, "%s", "Failed connection to web server\n");
		return 1;
	}
	else
	{
		fprintf(file, "%s", "Connected to web server.   The response is:\n");
	}
	//   Attempts to write len bytes from buf to BIO
	if(BIO_write(bio, serverWriteBuffer, strlen(serverWriteBuffer)) <= 0)
	{
		//  Handle failed writes here
		if(!BIO_should_retry(bio))
		{
			// Not worth implementing, but worth knowing.
		}
		//  -> Let us know about the failed writes
		fprintf(file, "%s", "Failed write to web server\n");
	}

	//  Read the response message
	for(;;)
	{
		//  Get chunks of the response 1023 at the time.
		size = BIO_read(bio, response, 1023);
		//  If no more data, then exit the loop
		if(size <= 0)
		{
			break;
		}
		//  Terminate the string with a 0, to let know C when the string 
		//  ends.
		response[size] = 0;

		//  ->  Print out the response
		fprintf(file, "%s\n", response);
	}
	fprintf(file, "%s", "Finished receiving Web Server Response\n\n");
	//  Clean after ourselves
	BIO_free_all(bio);
	SSL_CTX_free(ctx);
	//sleep(450);
	fclose(file); 
    return 0;
}

