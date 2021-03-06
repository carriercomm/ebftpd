#ifndef __FTP_TASK_TASK_HPP
#define __FTP_TASK_TASK_HPP

#include <vector>
#include <string>
#include <utility>
#include <future>
#include <memory>
#include "ftp/task/types.hpp"
#include "acl/types.hpp"
#include "acl/user.hpp"
#include "cfg/config.hpp"

namespace ftp 
{ 

class Server;
class Client;

namespace task
{

class Task : public std::enable_shared_from_this<Task>
{
public:
  virtual ~Task() { }
  virtual void Execute(Server& server) = 0;
  void Push();
};

class KickUser : public Task
{
  acl::UserID uid;
  bool oneOnly;
  std::promise<int> promise;
  
public:
  KickUser(acl::UserID uid, std::future<int>& future, bool oneOnly = false) : 
    uid(uid), oneOnly(oneOnly) { future = promise.get_future(); }
  void Execute(Server& server);
};

class LoginKickUser : public Task
{
public:
  struct Result
  {
    bool kicked;
    boost::posix_time::time_duration idleTime;
    unsigned logins;
    Result() : kicked(false), logins(0) { }
  };
  
private:
  acl::UserID uid;
  std::promise<Result> promise;
  
public:
  LoginKickUser(acl::UserID uid, std::future<Result>& future) : uid(uid)
  { future = promise.get_future(); }
  void Execute(Server& server);
};

class GetOnlineUsers : public Task
{
  std::vector<ftp::task::WhoUser>& users;
  std::promise<bool> promise;
  
public:
  GetOnlineUsers(std::vector<ftp::task::WhoUser>& users, std::future<bool>& future) : 
    users(users) { future = promise.get_future(); }
    
  void Execute(Server& server);
};

class ReloadConfig : public Task
{
public:
  enum class Result { Okay, Fail, StopStart };

private:
  std::promise<std::pair<Result, Result>> promise;
  
public:
  ReloadConfig(std::future<std::pair<Result, Result>>& future)
  { future = promise.get_future(); }
  
  void Execute(Server& server);
};

class Exit : public Task
{
public:
  void Execute(Server& server);
};

class UserUpdate : public Task
{
  acl::UserID uid;
  
public:
  UserUpdate(acl::UserID uid) : uid(uid) { }
  void Execute(Server& server);
};

class ClientFinished : public Task
{
  Client& client;
  
public:
  ClientFinished(Client& client) : client(client) { }
  void Execute(Server& server);
};

// end
}
}

#endif
