#include <errno.h>
#include <string.h>
#include "exception.hpp"

namespace util
{

  void errno_to_exception(const char* function, int _errno)
  {
    switch (_errno)
    {
    case EADDRINUSE     :
      throw util::address_in_use();
    case ECONNREFUSED   :
      throw util::connection_refused();
    case EISCONN        :
      throw util::already_connected();
    case ETIMEDOUT      :
      throw util::timeout_error();
    }

    char error_message[1024];
    memset(error_message, '\0', sizeof(error_message));
    strncpy(error_message, function, sizeof(error_message) - 1);
    strncat(error_message, ": ", sizeof(error_message) - 1);
    strncat(error_message, strerror(_errno), sizeof(error_message) - 1);
    throw util::unknown_network_error(error_message);
  }

}