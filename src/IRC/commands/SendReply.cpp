#include "../../Network/server.hpp"
#include "../../Network/client.hpp"

void Server::sendReply(int fd, const std::string &message)
{
    Client *client = getClientbyFD(fd);
    if (!client)
        return;

    client->appendOutBuffer(message + "\r\n");
    enableWritePoll(fd);  
}

void Server::sendNumericReply(int fd, const std::string &code, const std::string &params)
{
    Client *client = getClientbyFD(fd);
    std::string nick = (client && !client->getNickname().empty()) ? client->getNickname() : "*";

    std::string message = ":" + std::string(SERVER_NAME) + " " + code + " " + nick + " " + params;
    sendReply(fd, message);
}

void Server::broadcastToChannel(Channel *channel, const std::string &message, int excludeFd)
{
    std::set<Client*>::iterator it = channel->getUserList().begin();
    std::set<Client*>::iterator ite = channel->getUserList().end();

    for (; it != ite; ++it)
    {
        if ((*it)->getFd() != excludeFd)
            sendReply((*it)->getFd(), message);
    }
}


void Server::flushClient(int fd)
{
    Client *client = getClientbyFD(fd);
    if (!client)
        return;

    std::string &outBuf = client->getOutBuffer();
    if (outBuf.empty())
    {
        disableWritePoll(fd);
        return;
    }

    int bytes = send(fd, outBuf.c_str(), outBuf.size(), 0);

    if (bytes > 0)
    {
        outBuf.erase(0, bytes);
        if (outBuf.empty())
            disableWritePoll(fd);
    }
    else if (bytes < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        return;
    else
    {
        removeClient(fd);
    }
}

void Server::enableWritePoll(int fd)
{
    for (size_t i = 0; i < _fds.size(); i++)
    {
        if (_fds[i].fd == fd)
        {
            _fds[i].events |= POLLOUT;
            break;
        }
    }
}

void Server::disableWritePoll(int fd)
{
    for (size_t i = 0; i < _fds.size(); i++)
    {
        if (_fds[i].fd == fd)
        {
            _fds[i].events &= ~POLLOUT;
            break;
        }
    }
}