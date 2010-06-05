
#include "tcpserver.h"
#include "debug.h"

int readIntegerFromElement(xmlTextReaderPtr reader)
{
	xmlChar *text;
	int value;
	int nodeType;
	
	if (xmlTextReaderRead(reader) == 1)
	{
		text = xmlTextReaderValue(reader);
		nodeType = xmlTextReaderNodeType(reader);

		if ((nodeType == 3/*XmlNodeType.Text*/) && (text != NULL))
		{
			
			sscanf((char*)text, "%d", &value);

			xmlFree(text);
			
			/* return the decoded the int value */					
			return value;
		}
		else
		{
			return -0xaffe;
		}
	}				
	else
	{
		return -0xaffe;
	}
}

struct in_addr readIPaddrFromElement(xmlTextReaderPtr reader)
{
	xmlChar *text;
	struct in_addr ip;
	int nodeType;
	
	if (xmlTextReaderRead(reader) == 1)
	{
		text = xmlTextReaderValue(reader);
		nodeType = xmlTextReaderNodeType(reader);

		if ((nodeType == 3/*XmlNodeType.Text*/) && (text != NULL))
		{
			inet_pton(AF_INET, (char*)text, &ip);

			printf("convertIP %s\n", text);

			xmlFree(text);
		}
		else
		{
			ip.s_addr = INADDR_ANY;
		}
	}				
	else
	{
		ip.s_addr = INADDR_ANY;
	}

	return ip;
}

int connectAndSend(struct in_addr dst, short dst_port, char *buffer, int len)
{
	struct sockaddr_in sockaddr;
	int sd, bytes;

	sd = socket(AF_INET,SOCK_STREAM, 0);
	
	if (sd <0) 
	{
		printf("Error creating a socket\n");
		return -1;	
	}
	
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(dst_port);
	sockaddr.sin_addr = dst;

   	printf("sending to client %s\n", inet_ntoa(dst));
   	

	if (connect(sd, (struct sockaddr *) &sockaddr, sizeof(struct sockaddr_in)) < 0) 
	{
		printf("Fehler bei der Verbindungsaufnahme!\n");
		return -2;
	}
   	
	bytes = send(sd, buffer, len, 0);
	
	close(sd);
	
	return bytes;
}


char *genPasswordStr(char *password)
{
/*
	//int i;
private: String __gc* EncryptData(String __gc* Password, String __gc* Data)
{
    Byte __gc* bytes __gc [] = 0;
    for (Int32 __gc* i = Password->Length; (i <= 0x1f); i++)
    {
        Password = String::Concat(Password, Convert::ToString(Convert::ToChar(0x31)));
    }
    Byte __gc* inputBuffer __gc [] = Encoding::Default->GetBytes(Data);
    Byte __gc* rgbIV __gc [] = Encoding::Default->GetBytes(S"test38sdskskhko");
    Byte __gc* rgbKey __gc [] = Encoding::Default->GetBytes(Password);
    SymmetricAlgorithm __gc* algorithm = SymmetricAlgorithm::Create();
    algorithm->KeySize = 0x100;
    algorithm->IV = rgbIV;
    algorithm->Key = rgbKey;
    try
    {
        bytes = algorithm->CreateEncryptor(rgbKey, rgbIV)->TransformFinalBlock(inputBuffer, 0, inputBuffer->Length);
        return Encoding::Default->GetString(bytes);
    }
    catch (Exception __gc* exception1)
    {
        return S"";
    }
}
 

 
*/	
	return 0;
}

xmlTextWriterPtr createNewWriterForPacket(xmlBufferPtr *buffer)
{
	xmlTextWriterPtr writer;
	int rc;

	/* Create a new XML buffer, to which the XML document will be written */
	*buffer = xmlBufferCreate();
	if (*buffer == NULL) 
	{
		printf("createNewWriterForPacket: Error creating the xml buffer\n");
		return NULL;
	}
	
	/* Create a new XmlWriter for memory, with no compression.
	* Remark: there is no compression for this kind of xmlTextWriter */
	writer = xmlNewTextWriterMemory(*buffer, 0);
	if (writer == NULL) 
	{
		printf("createNewWriterForPacket: Error creating the xml writer\n");
		return NULL;
	}
	
	/* Start the document with the xml default for the version,
	* encoding ISO 8859-1 and the default for the standalone
	* declaration. */
	rc = xmlTextWriterStartDocument(writer, NULL, XML_ENCODING, NULL);
	if (rc < 0) 
	{
		printf("createNewWriterForPacket: Error at xmlTextWriterStartDocument\n");
		return NULL;
	}
	
	return writer;
}

int sendXmlRequestHeader(struct in_addr dst, short dst_port,
		int answerPort, struct in_addr myIP)
{
	xmlTextWriterPtr writer;
	xmlBufferPtr buffer;
	int rc = 0;
	
	if (socket <= 0)
	{
		return -1;
	}
	
	writer = createNewWriterForPacket(&buffer);

	rc += xmlTextWriterStartElement(writer, BAD_CAST "RequestHeader");

	rc += xmlTextWriterStartElement(writer, BAD_CAST "Data");
	

	/* add ProtocolType element with value */
	rc += xmlTextWriterWriteFormatElement(writer, BAD_CAST "ProtocolType", 
			"%d", RequestHeaderData);
	
	/* add RequestIp element with value */
	rc += xmlTextWriterWriteElement(writer, BAD_CAST "RequestIp", BAD_CAST inet_ntoa(myIP));

	/* add AnswerPort element with value */
	rc += xmlTextWriterWriteFormatElement(writer, BAD_CAST "AnswerPort", 
		"%d", answerPort);

	
	/* close Data */
	rc += xmlTextWriterEndElement(writer);
	
	/* close RequestHeader */
	rc += xmlTextWriterEndElement(writer);

	/* close all unclosed und flush buffers */
	xmlTextWriterEndDocument(writer);

	printf("sending XML %d\n'%s'\n", buffer->use, buffer->content);
	
	if (connectAndSend(dst, dst_port, (char *)buffer->content, buffer->use) != buffer->use)
	{
		printf("sending failed\n");
	}

	xmlFreeTextWriter(writer);

	xmlBufferFree(buffer);
	
	return rc;
}

struct tRequestHeader* parseRequestHeader(char *input, int len)
{
    struct tRequestHeader *result = malloc(sizeof(struct tRequestHeader));    
	xmlTextReaderPtr reader;
	xmlChar *name;
	int ret;
    
    memset(result, 0x00, sizeof(struct tRequestHeader));
 
/*	
<RequestHeader>
  <Data>
    <ProtocolType>1001</ProtocolType>
    <RequestIp>192.168.0.31</RequestIp>
    <AnswerPort>9998</AnswerPort>
  </Data>
</RequestHeader>
*/
   
    /* http://www.xmlsoft.org/html/libxml-parser.html#xmlParserOption */
    reader = xmlReaderForMemory(input, len, NULL/*URL*/,NULL/*encoding*/, 0 /*options*/); 
	
	if (reader != NULL) 
	{
		/* get first element */
		ret = xmlTextReaderRead(reader);
		
		/* iterate through all elements */
		while (ret == 1) 
		{
			name = xmlTextReaderName(reader);
			
			if ((result->requestIP.s_addr == 0) && (xmlStrcmp(name, BAD_CAST "RequestIp") == 0))
			{
				result->requestIP = readIPaddrFromElement(reader);
			}	
			else if ((result->answerPort == 0) && (xmlStrcmp(name, BAD_CAST "AnswerPort") == 0))
			{
				result->answerPort = readIntegerFromElement(reader);
			}	
			
			xmlFree(name);
			
			/* goto next element */
			ret = xmlTextReaderRead(reader);
		}		

		xmlFreeTextReader(reader);
		return result;	
	}
	else
	{
		free(result);
		return NULL;
	}		
}


int sendXmlReplyHeader(struct in_addr dst, short dst_port,
		int deviceSerial, char *productName,
		char *deviceName, int deviceHardware, int deviceOS, int deviceFirmware,
		char *deviceAccessName, char *serverName, 
		int serverSendPort, int serverReceivePort)
{
	xmlTextWriterPtr writer;
	xmlBufferPtr buffer;
	int rc = 0;
	
	if (socket <= 0)
	{
		return -1;
	}
	
	writer = createNewWriterForPacket(&buffer);

	rc += xmlTextWriterStartElement(writer, BAD_CAST "DeviceHeader");

	rc += xmlTextWriterStartElement(writer, BAD_CAST "Data");
	

	/* add ProtocolType element with value */
	rc += xmlTextWriterWriteFormatElement(writer, BAD_CAST "ProtocolType", 
			"%d", HeaderData);
	
	/* add DeviceSerial element with value */
	rc += xmlTextWriterWriteFormatElement(writer, BAD_CAST "DeviceSerial", 
			"%d", deviceSerial);
	
	/* add ProductName element with value */
	rc += xmlTextWriterWriteElement(writer, BAD_CAST "ProductName", 
			BAD_CAST productName);
	
	/* add DeviceName element with value */
	rc += xmlTextWriterWriteElement(writer, BAD_CAST "DeviceName", BAD_CAST deviceName);
	
	/* add DeviceHardware element with value */
	rc += xmlTextWriterWriteFormatElement(writer, BAD_CAST "DeviceHardware", 
			"%d", deviceHardware);
	
	/* add DeviceOS element with value */
	rc += xmlTextWriterWriteFormatElement(writer, BAD_CAST "DeviceOS", 
			"%d", deviceOS);
	
	/* add DeviceFirmware element with value */
	rc += xmlTextWriterWriteFormatElement(writer, BAD_CAST "DeviceFirmware", 
			"%d", deviceFirmware);
	
	/* add DeviceAccessName element with value */
	rc += xmlTextWriterWriteElement(writer, BAD_CAST "DeviceAccessName", BAD_CAST deviceAccessName);
	
	/* add ServerName element with value */
	rc += xmlTextWriterWriteElement(writer, BAD_CAST "ServerName", BAD_CAST serverName);
	
	/* add ServerSendPort element with value */
	rc += xmlTextWriterWriteFormatElement(writer, BAD_CAST "ServerSendPort", 
			"%d", serverSendPort);
	
	/* add ServerReceivePort element with value */
	rc += xmlTextWriterWriteFormatElement(writer, BAD_CAST "ServerReceivePort", 
			"%d", serverReceivePort);
	/* BUGFIX: typo in AquaSuite */
	rc += xmlTextWriterWriteFormatElement(writer, BAD_CAST "ServerRecivePort", 
			"%d", serverReceivePort);

	
	/* close Data */
	rc += xmlTextWriterEndElement(writer);
	
	/* close DeviceHeader */
	rc += xmlTextWriterEndElement(writer);

	/* close all unclosed und flush buffers */
	xmlTextWriterEndDocument(writer);

	printf("sending XML %d\n'%s'\n", buffer->use, buffer->content);

	if (connectAndSend(dst, dst_port, (char *)buffer->content, buffer->use) != buffer->use)
	{
		printf("sending failed\n");
	}

	xmlFreeTextWriter(writer);

	xmlBufferFree(buffer);
	
	return rc;
}

enum packetType getPacketType(char *input, int len)
{
	xmlTextReaderPtr reader;
	xmlChar *name;
	int ret, type;
    
    /* http://www.xmlsoft.org/html/libxml-parser.html#xmlParserOption */
    reader = xmlReaderForMemory(input, len, NULL/*URL*/,NULL/*encoding*/, 0 /*options*/); 
	
	if (reader != NULL) 
	{
		/* get first element */
		ret = xmlTextReaderRead(reader);
		
		/* iterate through all elements */
		while (ret == 1) 
		{
			name = xmlTextReaderName(reader);
			
			if (xmlStrcmp(name, BAD_CAST "ProtocolType") == 0)
			{
				/* found <ProtocolType> */
				type = readIntegerFromElement(reader);
				
				xmlFree(name);
				xmlFreeTextReader(reader);

				return (enum packetType)type;
			}	
			
			xmlFree(name);
			
			/* goto next element */
			ret = xmlTextReaderRead(reader);
		}		
	}
	
	xmlFreeTextReader(reader);
	return UnknownPacketType;
}

static void processNode(xmlTextReaderPtr reader) 
{
    xmlChar *name, *value;

    name = xmlTextReaderName(reader);
    if (name == NULL)
        name = xmlStrdup(BAD_CAST "--");
    value = xmlTextReaderValue(reader);

    printf("%d %d %s %d",
            xmlTextReaderDepth(reader),
            xmlTextReaderNodeType(reader),
            name,
            xmlTextReaderIsEmptyElement(reader));
    xmlFree(name);
    if (value == NULL)
        printf("\n");
    else {
        printf(" %s\n", value);
        xmlFree(value);
    }
}

void parseXML(char *input, int len)
{
    xmlTextReaderPtr reader;
    int ret;
    
    /* http://www.xmlsoft.org/html/libxml-parser.html#xmlParserOption */
    reader = xmlReaderForMemory(input, len, NULL/*URL*/,NULL/*encoding*/, 0 /*options*/); 

	if (reader != NULL) 
	{						
		ret = xmlTextReaderRead(reader);

		while (ret == 1) 
		{
			processNode(reader);
			ret = xmlTextReaderRead(reader);
		}
	
		xmlFreeTextReader(reader);
		if (ret != 0) 
		{
			printf("failed to parse\n");
		}
	} 
	else 
	{
		printf("Unable to read input\n");
	}
}


void dieWithError(char *errorMessage)  /* Error handling function */
{
	perror(errorMessage);
	exit(1);
}

/* TCP client handling function */
void handleTCPClient(int clntSocket, struct sockaddr_in clientAddr)
{
	char receiveBuffer[RCVBUFSIZE];        /* Buffer for echo string */
	int ptr;
	int recvMsgSize;                    /* Size of received message */
	enum packetType type;
	struct tRequestHeader *requestHeader = NULL;
	
	/* Receive message from client */
	if ((recvMsgSize = recv(clntSocket, receiveBuffer, RCVBUFSIZE, 0)) < 0)
	{
		dieWithError("recv() failed");
	}
	
	/* Close client socket like Aquasuite does */
	close(clntSocket);    

	if (recvMsgSize == 0)
	{
		/* this was just a scan */
		return;
	}

	/* Send received string and receive again until end of transmission */
	ptr = 0;
	
	/* remove zeros at beginning (why they are here?!) */
	while ((receiveBuffer[ptr] == 0x00) && (ptr < RCVBUFSIZE))
	{
		ptr++;
	}
	ptr++;
	
	type = getPacketType(receiveBuffer+ptr, recvMsgSize-ptr);
	
	
	switch(type)
	{
		case HeaderData:
			printf("Received: HeaderData\n");
			parseXML(receiveBuffer+ptr, recvMsgSize-ptr);
			break;
		
		case RequestHeaderData:
			requestHeader = parseRequestHeader(receiveBuffer+ptr, recvMsgSize-ptr);
			
			if (requestHeader != NULL)
			{
				printf("Received: RequestHeaderData\n");
								
	 			//sendXmlReplyHeader(requestHeader->requestIP, requestHeader->answerPort, 
	 			//	99999 /*deviceSerial*/,  "aquastream USB"/*productName*/,
				//	"aquastream xt demo" /*deviceName*/, 0x69 /*deviceHardware*/, 0x66 /*deviceOS*/, 0x3f9 /*deviceFirmware*/,
				//	"aquastream USB" /*deviceAccessName*/, "192.168.0.13" /*serverName*/, 
				//	SERVER_PORT, SERVER_PORT-1);
				
	 			sendXmlReplyHeader(requestHeader->requestIP, requestHeader->answerPort, 
	 				99999 /*deviceSerial*/,  "aquaero Revision 4"/*productName*/,
					"aquaero demo" /*deviceName*/, 400 /*deviceHardware*/, 0xc9 /*deviceOS*/, 0x1a5 /*deviceFirmware*/,
					"aquaero Revision 4" /*deviceAccessName*/, "192.168.0.13" /*serverName*/, 
					SERVER_PORT, SERVER_PORT);
					
				free(requestHeader);
			}
			else
			{
				printf("parseRequestHeader failed\n");
			}
			break;
		
		case DeviceData:
			printf("Received: DeviceData\n");
			parseXML(receiveBuffer+ptr, recvMsgSize-ptr);
			break;
		
		case RequestDeviceData:
			printf("Received: RequestDeviceData\n");
			parseXML(receiveBuffer+ptr, recvMsgSize-ptr);
			break;
		
		case SendDataToServer:
			printf("Received: SendDataToServer\n");
			parseXML(receiveBuffer+ptr, recvMsgSize-ptr);
			break;
		
		case UnknownPacketType:
			printf("Received unknown packet type\n");

			debug_buffer_hex((unsigned char *)receiveBuffer+ptr, recvMsgSize-ptr);
			debug_buffer(receiveBuffer+ptr, recvMsgSize-ptr);

			parseXML(receiveBuffer+ptr, recvMsgSize-ptr);
			break;
	}					
}

void start_listening()
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in servAddr; /* Local address */
    struct sockaddr_in clientAddr; /* Client address */
    unsigned short servPort;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */

    servPort = SERVER_PORT;  

    /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        dieWithError("socket() failed");
      
    /* Construct local address structure */
    memset(&servAddr, 0, sizeof(servAddr));   /* Zero out structure */
    servAddr.sin_family = AF_INET;                /* Internet address family */
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    servAddr.sin_port = htons(servPort);      /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        dieWithError("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        dieWithError("listen() failed");
	

    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(clientAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &clientAddr, 
                               &clntLen)) < 0)
            dieWithError("accept() failed");

        /* clntSock is connected to a client! */

        printf("Handling client %s\n", inet_ntoa(clientAddr.sin_addr));

        handleTCPClient(clntSock, clientAddr);
    }
    /* NOT REACHED */	
}

/**
 * ConvertInput:
 * @in: string in a given encoding
 * @encoding: the encoding used
 *
 * Converts @in into UTF-8 for processing with libxml2 APIs
 *
 * Returns the converted UTF-8 string, or NULL in case of error.
 */
xmlChar *ConvertInput(const char *in, const char *encoding)
{
	xmlChar *out;
	int ret;
	int size;
	int out_size;
	int temp;
	xmlCharEncodingHandlerPtr handler;
	
	if (in == NULL)
		return 0;
	
	handler = xmlFindCharEncodingHandler(encoding);
	
	if (!handler) 
	{
		printf("ConvertInput: no encoding handler found for '%s'\n",
		encoding ? encoding : "");
		return 0;
	}
	
	size = (int) strlen(in) + 1;
	out_size = size * 2 - 1;
	out = (unsigned char *) xmlMalloc((size_t) out_size);
	
	if (out != 0) 
	{
		temp = size - 1;
		ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
		if ((ret < 0) || (temp - size + 1)) 
		{
			if (ret < 0) 
			{
				printf("ConvertInput: conversion wasn't successful.\n");
			} 
			else 
			{
				printf("ConvertInput: conversion wasn't successful. converted: %i octets.\n", temp);
			}
			
			xmlFree(out);
			out = 0;
		} 
		else 
		{
			out = (unsigned char *) xmlRealloc(out, out_size + 1);
			out[out_size] = 0;  /*null terminating out */
		}
	} 
	else 
	{
		printf("ConvertInput: no mem\n");
	}
	
	return out;
}

