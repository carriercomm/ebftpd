#ifndef __DB_STATS_PROTOCOL_HPP
#define __DB_STATS_PROTOCOL_HPP

#include "acl/types.hpp"

namespace stats
{
enum class Timeframe : unsigned;
}

namespace db { namespace stats
{

class Traffic;

void ProtocolUpdate(acl::UserID uid, long long sendBytes, long long receiveBytes);
Traffic ProtocolUser(acl::UserID uid, ::stats::Timeframe timeframe);
Traffic ProtocolTotal(::stats::Timeframe timeframe);

} /* stats namespace */
} /* db namespace */

#endif
