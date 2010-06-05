#ifndef TCPSERVER_H
#define TCPSERVER_H 1

/*
emerge dev-libs/libxml2
apt-get install libxml2-dev
*/


#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#define SERVER_PORT 9999

#define MAXPENDING 5    /* Maximum outstanding connection requests */

#define RCVBUFSIZE 4096   /* Size of receive buffer */

#define XML_ENCODING "ISO-8859-1"

#define CRYPTO_IV "test38sdskskhko"
#define CRYPTO_PW_LENGTH  0x1f

enum packetType 
{ 
	HeaderData = 1000,
	RequestHeaderData = 1001,
	
	DeviceData = 1002,
	RequestDeviceData = 1003,
	
	SendDataToServer = 1004,
	
	UnknownPacketType
};

struct tRequestHeader 
{
	struct in_addr requestIP;
	short answerPort;
};

int readIntegerFromElement(xmlTextReaderPtr reader);
struct in_addr readIPaddrFromElement(xmlTextReaderPtr reader);

int connectAndSend(struct in_addr dst, short dst_port, char *buffer, int len);

/* 	takes a pointer to a xmlBufferPtr, because after 
 *	sending we need to free the buffer.
 */
xmlTextWriterPtr createNewWriterForPacket(xmlBufferPtr *buffer);

int sendXmlRequestHeader(struct in_addr dst, short dst_port,
		int answerPort, struct in_addr myIP);
int sendXmlReplyHeader(struct in_addr dst, short dst_port,
		int deviceSerial, char *productName,
		char *deviceName, int deviceHardware, int deviceOS, int deviceFirmware,
		char *deviceAccessName, char *serverName, 
		int serverSendPort, int serverReceivePort);

enum packetType getPacketType(char *input, int len);

void handleTCPClient(int clntSocket, struct sockaddr_in clientAddr); 
void start_listening();

xmlChar *ConvertInput(const char *in, const char *encoding);

#endif
