#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

#define IPV6_STRLEN_MAX         (4 * 8 + 7 + 1 + 4)
#define IPV4_STRLEN_MAX         (3 * 4 + 3 + 1 + 4)

#define INET_ADDR_SIZE          (sizeof(struct in6_addr))

static int is_valid_ipaddr(char *ipstr, int ipver)
{
        unsigned char buf[sizeof(struct in6_addr)];

        return (inet_pton(ipver, ipstr, buf) == 1);
}

static int socket_tcp_nodelay_set(int sockfd) {
        int flag = 1;
        if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)) < 0) {
                perror("setsockopt TCP_NODELAY failed");
                return -1;
        }
        return 0;
}
