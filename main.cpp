#include "main.h"

#include "net.h"
#include "graphics.h"

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    std::vector<packet> *packets = new std::vector<packet>();
    pthread_t networkThread;

    pthread_mutex_init(packetsMutex, NULL);
    
    pthread_create(&networkThread, NULL, initNet, (void *)packets);
    //initNet((void *)packets);

    initGL(packets);

    pthread_join(networkThread, NULL);
    pthread_mutex_destroy(packetsMutex);
}
