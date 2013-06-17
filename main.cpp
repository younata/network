#include "main.h"

#include "net.h"
#include "graphics.h"

#include <string.h>
#include <unistd.h>

pthread_mutex_t networkPacketsMutex;

int main(int argc, char *argv[])
{
    char *device = NULL;

    bool use3d = false;
    int eger;
    while ((eger = getopt(argc, argv, "d:i:3")) != -1) {
        switch (eger) {
            case 'd':
                device = optarg;
                break;
            case 'i':
                device = optarg;
                break;
            case '3':
                use3d = true;
                break;
            default:
                fprintf(stderr, "Error, unexpected option \"-%c\"\n", eger);
                return -1;
        }
    }

    std::list<packet> *packets = new std::list<packet>();
    pthread_t networkThread;

    pthread_mutex_init(&networkPacketsMutex, NULL);

    struct argumentsToInitNet foo;
    foo.p = packets;
    foo.deviceName = device;
    
    pthread_create(&networkThread, NULL, initNet, (void *)&foo);

    initGL(packets, argc, argv, use3d);

    pthread_join(networkThread, NULL);
    pthread_mutex_destroy(&networkPacketsMutex);
}
