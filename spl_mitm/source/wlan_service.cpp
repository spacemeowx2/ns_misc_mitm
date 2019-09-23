#include <switch.h>
#include "debug.hpp"

#include "wlan_service.hpp"

Result WlanMitMService::Connect(ConnectParams p) {
    LogFormat("WlanMitMService::Connect");
    LogHex(&p, sizeof(p));

    return ResultAtmosphereMitmShouldForwardToSession;
}
