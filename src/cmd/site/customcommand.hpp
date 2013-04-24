//    Copyright (C) 2012, 2013 ebftpd team
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __CMD_SITE_CUSTOMCOMMAND_HPP
#define __CMD_SITE_CUSTOMCOMMAND_HPP

#include "cmd/command.hpp"
#include "cfg/setting.hpp"
#include "plugin/hooks.hpp"

namespace cmd { namespace site
{

class CustomCommand : public Command
{
protected:
  cfg::SiteCmd custSiteCmd;
  
public:
  CustomCommand(cfg::SiteCmd& custSiteCmd, ftp::Client& client, 
                const std::string& argStr, const Args& args) :
    Command(client, client.Control(), client.Data(), argStr, args),
    custSiteCmd(custSiteCmd) { }

  virtual void Execute() = 0;
};

class CustomEXECCommand : public CustomCommand
{
public:
  CustomEXECCommand(cfg::SiteCmd& custSiteCmd, ftp::Client& client, 
                    const std::string& argStr, const Args& args) :
    CustomCommand(custSiteCmd, client, argStr, args) { }
    
  void Execute();
};

class CustomTEXTCommand : public CustomCommand
{
public:
  CustomTEXTCommand(cfg::SiteCmd& custSiteCmd, ftp::Client& client, 
                    const std::string& argStr, const Args& args) :
    CustomCommand(custSiteCmd, client, argStr, args) { }

  void Execute();
};


class CustomALIASCommand : public CustomCommand
{
public:
  CustomALIASCommand(cfg::SiteCmd& custSiteCmd, ftp::Client& client, 
                    const std::string& argStr, const Args& args) :
    CustomCommand(custSiteCmd, client, argStr, args) { }

  void Execute();
};

class PluginCommand : public cmd::Command
{
  plugin::Plugin& plugin;
  plugin::CommandHookFunction function;
  
public:
  PluginCommand(ftp::Client& client, const std::string& argStr, const cmd::Args& args,
                plugin::Plugin& plugin, const plugin::CommandHookFunction& function) :
    cmd::Command(client, client.Control(), client.Data(), argStr, args),
    plugin(plugin),
    function(function)
  {
  }
  
  void Execute();
};

} /* site namespace */
} /* cmd namespace */

#endif
