#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define NTP_SERVER "pool.ntp.org"
#define NTP_PORT 123
#define NTP_PACKET_SIZE 48
#define NTP_TIMESTAMP_DELTA 2208988800ull

typedef struct {
    uint8_t li_vn_mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t reference_id;
    uint32_t ref_ts_sec;
    uint32_t ref_ts_frac;
    uint32_t orig_ts_sec;
    uint32_t orig_ts_frac;
    uint32_t recv_ts_sec;
    uint32_t recv_ts_frac;
    uint32_t trans_ts_sec;
    uint32_t trans_ts_frac;
} ntp_packet;

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    ntp_packet packet;
    time_t current_time;
    struct tm *local_time;

    // Print NTP server address
    printf("NTP_SERVER: %s\n", NTP_SERVER);

    // NTP Client Code

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
        error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(NTP_PORT);

    struct addrinfo *result;
    int rc = getaddrinfo(NTP_SERVER, NULL, NULL, &result);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        exit(EXIT_FAILURE);
    }

    memcpy(&serv_addr.sin_addr, &((struct sockaddr_in *)result->ai_addr)->sin_addr, sizeof(struct in_addr));
    freeaddrinfo(result);

    memset(&packet, 0, sizeof(packet));
    packet.li_vn_mode = (0x03 << 6) | (0x03 << 3) | 0x03;

    if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR sending packet");

    n = recv(sockfd, &packet, sizeof(packet), 0);
    if (n < 0)
        error("ERROR receiving response");

    close(sockfd);

    current_time = (ntohl(packet.trans_ts_sec) - NTP_TIMESTAMP_DELTA);
    printf("NTP time: %s", ctime(&current_time));

    // Local Time Client Code

    current_time = time(NULL);
    local_time = localtime(&current_time);
    printf("Local time: %s", asctime(local_time));

    // Calculate and print time difference
    time_t ntp_time = (ntohl(packet.trans_ts_sec) - NTP_TIMESTAMP_DELTA);
    double time_difference = difftime(current_time, ntp_time);
    printf("Time difference: %.3f seconds\n", time_difference);

    return 0;
}
