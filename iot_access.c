#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#include <string.h>	//strlen

char getIotResponse(char* reply, char* errorMessage)
{
    int socket_IotServer;
	struct sockaddr_in server_Iot;
	char address_Iot[] = "192.168.2.44";
	char message_Iot[] = "GET / HTTP/1.1\r\n\r\n";
	int portIot = 35000;

    socket_IotServer = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_IotServer == -1)
	{
        errorMessage = "Could not create socket for IoT device";
        return 1;
	}
    server_Iot.sin_addr.s_addr = inet_addr(address_Iot);
	server_Iot.sin_family = AF_INET;
	server_Iot.sin_port = htons(portIot);
    if (connect(socket_IotServer, (struct sockaddr*)&server_Iot, sizeof(server_Iot)) < 0)
	{
        errorMessage = "Could not connect to IoT device";
        return 1;
	}
    if(send(socket_IotServer, message_Iot, strlen(message_Iot), 0) < 0)
	{
        errorMessage = "Mesage to IoT device failed";
        return 1;
	}
    if (recv(socket_IotServer, reply, 2000, 0) < 0)
	{
        errorMessage = "Did not receive a message from IoT device";
        return 1;
	}
    errorMessage = "no error";
    return 0;
}