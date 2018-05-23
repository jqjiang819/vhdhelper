#include "vhduuid.h"

#if defined(__unix) || defined(__linux) || defined(__APPLE__)
vhd_uuid_t uuid_get_uuid()
{
    uuid_t uuid_generated;
    vhd_uuid_t uuid;
    uuid_generate(uuid_generated);
    memcpy(&uuid, &uuid_generated, 16);
    return uuid;
}
#elif defined(_WIN32)
vhd_uuid_t uuid_get_uuid()
{
    GUID guid;
    vhd_uuid_t uuid;
    CoCreateGuid(&guid);
    uuid.f1 = (uint32_t)guid.Data1;
    uuid.f2 = (uint16_t)guid.Data2;
    uuid.f3 = (uint16_t)guid.Data3;
    memcpy(&uuid.f4, &guid.Data4, 8);
    return uuid;
}
#endif