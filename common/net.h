#ifndef STEROIDS_NET_H
#define STEROIDS_NET_H

#include <cstdint>
#include <string>

namespace net {

void init();

void quit();

uint32_t resolveHost(std::string host, uint16_t port);

}

#endif //STEROIDS_NET_H
