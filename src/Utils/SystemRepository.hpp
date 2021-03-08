#ifndef GFORCE_TRAINER_CONTROLLER_SYSTEMREPOSITORY_H
#define GFORCE_TRAINER_CONTROLLER_SYSTEMREPOSITORY_H

#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

namespace GForce::Utils {

class SystemRepository
{
    public:
        /**
         * Determines and returns the IPv4 address of the given interface
         */
        static std::string getNetworkAddress(std::string interface)
        {
            int fd;
            struct ifreq ifr;

            fd = socket(AF_INET, SOCK_DGRAM, 0);

            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ-1);
            ioctl(fd, SIOCGIFADDR, &ifr);

            close(fd);

            return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
        }
};

}

#endif //GFORCE_TRAINER_CONTROLLER_SYSTEMREPOSITORY_H
