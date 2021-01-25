#include "local_msg.hpp"

local_msg::local_msg()
{
    strncpy(this->defpath, "../../cache/local_message/", strlen("../../cache/local_message/"));
}
