#include "net.h"

extern pthread_mutex_t networkPacketsMutex;

/*
class packet {
    unsigned char destAddr[4];
    unsigned char sourceAddr[4];
    unsigned int time;
};
*/

static std::vector<packet> *packets;

/* default snap length (maximum bytes per packet to capture) */
#define SNAP_LEN 1518

/* ethernet headers are always exactly 14 bytes [1] */
#define SIZE_ETHERNET 14

/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6

struct sniff_ethernet {
    u_char ether_dhost[6];
    u_char ether_shost[6];
    u_short ether_type;
};

#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)

struct sniff_ip {
    u_char  ip_vhl;                 /* version << 4 | header length >> 2 */
    u_char  ip_tos;                 /* type of service */
    u_short ip_len;                 /* total length */
    u_short ip_id;                  /* identification */
    u_short ip_off;                 /* fragment offset field */
    #define IP_RF 0x8000            /* reserved fragment flag */
    #define IP_DF 0x4000            /* dont fragment flag */
    #define IP_MF 0x2000            /* more fragments flag */
    #define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
    u_char  ip_ttl;                 /* time to live */
    u_char  ip_p;                   /* protocol */
    u_short ip_sum;                 /* checksum */
    unsigned char ip_src[4];
    unsigned char ip_dst[4];
    //struct  in_addr ip_src,ip_dst;  /* source and dest address */
};

#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)

/* TCP header */
typedef u_int tcp_seq;

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

void print_payload(const u_char *payload, int len);

void print_hex_ascii_line(const u_char *payload, int len, int offset);

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *pkt)
{
    const struct sniff_ethernet *ethernet;
    const struct sniff_ip *ip;
    const char *payload;

    int size_ip, size_tcp, size_payload;

    unsigned char type;

    ethernet = (struct sniff_ethernet*)(pkt);

    ip = (struct sniff_ip*)(pkt + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if (size_ip < 20) {
        return;
    }

    switch (ip->ip_p) {
        case IPPROTO_TCP:
            type = 0;
            break;
        case IPPROTO_UDP:
            type = 1;
			break;
		case IPPROTO_ICMP:
            type = 2;
			break;
		case IPPROTO_IP:
            type = 3;
			break;
		default:
            type = 4;
			break;;
	}
    unsigned char *s, *d;
    s = (unsigned char *)ip->ip_src;
    d = (unsigned char *)ip->ip_dst;

    for (int i = 0; i < 4; i++) {
        s[i] = s[i] & 0xFF;
        d[i] = d[i] & 0xFF;
        printf("%02x", s[i]);
    }
    printf("\n");

    packet p = packet(d, s, type);

    pthread_mutex_lock(&networkPacketsMutex);
    packets->push_back(p);
    pthread_mutex_unlock(&networkPacketsMutex);
/*
    printf("recieved a packet from ");
    for (int i = 0; i < 4; i++) {
        printf("%d", s[i]);
        if (i < 3) printf(".");
    }
    printf(" headed for ");
    for (int i = 0; i < 4; i++) {
        printf("%d", d[i]);
        if (i < 3) printf(".");
    }
    printf("\n");
//*/
}

void *initNet(void *arg)
{
    packets = (std::vector<packet>*)arg;
    char *dev = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    char filter_exp[] = "ip";
    struct bpf_program fp;
    bpf_u_int32 mask, net;

    dev = "en1";

/*
    if ((dev = pcap_lookupdev(errbuf)) == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        dev = "en1";
    }
*/

    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = mask = 0;
    }

    if ((handle = pcap_open_live(dev, SNAP_LEN, -1, 1000, errbuf)) == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        exit(-2);
    }

    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "error");
        exit(-3);
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "error");
        exit(-4);
    }

    pcap_loop(handle, -1, got_packet, NULL);
    printf("end loop\n");
    pcap_freecode(&fp);
    pcap_close(handle);
    return NULL;
}

packet::packet(unsigned char *da, unsigned char *sa, unsigned char t) : type(t)
{
    for (int i = 0; i < 4; i++) {
        destAddr[0] = *(da+i);
        sourceAddr[0] = *(sa+i);
    }
}
