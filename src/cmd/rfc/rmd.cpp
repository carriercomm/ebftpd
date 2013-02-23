#include "cmd/rfc/rmd.hpp"
#include "fs/directory.hpp"
#include "cmd/error.hpp"
#include "db/index/index.hpp"
#include "acl/path.hpp"
#include "cfg/get.hpp"
#include "logs/logs.hpp"
#include "fs/path.hpp"
#include "acl/user.hpp"

namespace cmd { namespace rfc
{

void RMDCommand::Execute()
{
  fs::VirtualPath path(fs::PathFromUser(argStr));
  util::Error e = fs::RemoveDirectory(client.User(),  path);
  if (!e)
  {
    control.Reply(ftp::ActionNotOkay, argStr + ": " + e.Message());
    throw cmd::NoPostScriptError();
  }

  const cfg::Config& config = cfg::Get();
  
  if (config.IsIndexed(path.ToString()))
    db::index::Delete(path.ToString());
  
  if (config.IsEventLogged(path.ToString()))
  {
    logs::Event("DELDIR", "path", path.ToString(), "user", client.User().Name(), 
                "group", client.User().PrimaryGroup(),
                "tagline", client.User().Tagline());
  }

  control.Reply(ftp::FileActionOkay, "RMD command successful.");   
}

} /* rfc namespace */
} /* cmd namespace */
