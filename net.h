#ifndef __network_net__
#define __network_net__

#include "main.h"

#include <pcap.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class packet {
public:
    unsigned char destAddr[4];
    unsigned char sourceAddr[4];
    unsigned char type;
    packet(unsigned char *da, unsigned char *sa, unsigned char t);
};

void *initNet(void *arg);

#endif
