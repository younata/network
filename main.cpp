#include "main.h"

#include "net.h"
#include "graphics.h"

pthread_mutex_t networkPacketsMutex;

int main(int argc, char *argv[])
{
    char *device = NULL;
    if (argc == 2) {
        device = argv[1];
    }
    std::vector<packet> *packets = new std::vector<packet>();
    pthread_t networkThread;

    pthread_mutex_init(&networkPacketsMutex, NULL);

    struct argumentsToInitNet foo;
    foo.p = packets;
    foo.deviceName = device;
    
    pthread_create(&networkThread, NULL, initNet, (void *)&foo);

    initGL(packets, argc, argv);

    pthread_join(networkThread, NULL);
    pthread_mutex_destroy(&networkPacketsMutex);
}
