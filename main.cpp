#include "main.h"

#include "net.h"
#include "graphics.h"

pthread_mutex_t networkPacketsMutex;

int main(int argc, char *argv[])
{
    // unit test!
/*
    point2d s,d;
    s.x = 0.0; s.y = 0.0;
    d.x = 5.0; d.y = 1.0;
    
   // unsigned char t[4]; t[0] = 50; t[1] = 97; t[2] = 114; t[3] = 119;
    struct point2d r;// = calculatePosition(t, 1, 1);
    r = calculateCurrentPosition(s, d, 1.0);
    printf("%f, %f\n", r.x, r.y);
    return 0;
//*/
    
    std::vector<packet> *packets = new std::vector<packet>();
    pthread_t networkThread;

    pthread_mutex_init(&networkPacketsMutex, NULL);
    
    pthread_create(&networkThread, NULL, initNet, (void *)packets);
    //initNet((void *)packets);

    initGL(packets, argc, argv);

    pthread_join(networkThread, NULL);
    pthread_mutex_destroy(&networkPacketsMutex);
}
