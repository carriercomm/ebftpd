#include <boost/thread/thread.hpp>
#include "util/net/tlssocket.hpp"
#include "util/net/tcpsocket.hpp"
#include "util/net/tlserror.hpp"
#include "util/net/tlscontext.hpp"

#include <iostream>

namespace util { namespace net
{

TLSSocket::~TLSSocket()
{
  Close();
}

TLSSocket::TLSSocket() :
  session(nullptr)
{
}

TLSSocket::TLSSocket(TCPSocket& socket, HandshakeRole role, TLSSocket* id) :
  session(nullptr)
{
  Handshake(socket, role, id);
}

void TLSSocket::EvaluateResult(int result)
{
  switch (SSL_get_error(session, result))
  {
    case SSL_ERROR_WANT_READ    :
    case SSL_ERROR_WANT_WRITE   :
    {
      break;
    }
    case SSL_ERROR_SSL          :
    {
      throw TLSProtocolError();
    }
    case SSL_ERROR_ZERO_RETURN  :
    {
      throw EndOfStream();
    }
    case SSL_ERROR_SYSCALL      :
    {
      int error = ERR_get_error();
      if (error) throw TLSProtocolError();
      else if (!result) throw EndOfStream();
      else if (result == -1) 
      {
        if (errno == EWOULDBLOCK || errno == EAGAIN || errno == ETIMEDOUT)
          throw TimeoutError();
        else
          throw TLSSystemError(errno);
      }
    }
    default                     :
    {
      throw TLSError();
    }
  }
}

void TLSSocket::Handshake(TCPSocket& socket, HandshakeRole role, TLSSocket* id)
{

  SSL_CTX* ctx = role == Client ?
                 TLSClientContext::Get() :
                 TLSServerContext::Get();
                 
  if (!ctx) throw TLSError("TLS context not initialised.");

  session = SSL_new(ctx);
  if (!session) throw TLSProtocolError();
  
  if (SSL_set_fd(session, socket.Socket()) != 1) throw TLSProtocolError();
  
  if (id) SSL_copy_session_id(session, id->session);
  
  if (role == Client) SSL_set_connect_state(session);
  else SSL_set_accept_state(session);

  int result;
  while (true)
  {
    if (role == Client) result = SSL_connect(session);
    else result = SSL_accept(session);
    boost::this_thread::interruption_point();
    if (result == 1) break;
    else EvaluateResult(result);
  }
}

size_t TLSSocket::Read(char* buffer, size_t bufferSize)
{
  while (true)
  {
    int result = SSL_read(session, buffer, bufferSize);
    boost::this_thread::interruption_point();
    if (result > 0) return result;
    else EvaluateResult(result);
  }
}

void TLSSocket::Write(const char* buffer, size_t bufferLen)
{
  size_t written = 0;
  while (bufferLen - written > 0)
  {
    int result = SSL_write(session, buffer + written, bufferLen - written);
      boost::this_thread::interruption_point();
    if (result > 0) written += result;
    else EvaluateResult(result);
  }
}

void TLSSocket::Close()
{
  if (session)
  {
    SSL_shutdown(session);
    SSL_free(session);
    session = nullptr;
  }
}

std::string TLSSocket::Cipher() const
{
  if (!session) return "NONE";
  const char* cipher = SSL_get_cipher(session);
  if (!cipher) return "NONE";
  return cipher;
}

} /* net namespace */
} /* util namespace */
