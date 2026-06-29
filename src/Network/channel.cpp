


#include "channel.hpp"
#include "client.hpp"

Channel::Channel(const std::string& name)
    : _name(name),
      _topic(""),
      _password(""),
      _inviteOnly(false),
      _topicRestricted(false),
      _userLimit(0)
{}

const std::string& Channel::getName() const
{
    return _name;
}

const std::string& Channel::getTopic() const
{
    return _topic;
}

const std::string& Channel::getPassword() const
{
    return _password;
}

bool   Channel::isInviteOnly() const
{
    return _inviteOnly;
}

int    Channel::getUserLimit() const
{
    return _userLimit;
}

size_t Channel::getUserCount() const
{
    return _users.size();
}

void Channel::addUser(Client* client)
{
    _users.insert(client);
}

void Channel::removeUser(Client* client)
{
    _users.erase(client);
    _operators.erase(client);
}

bool Channel::hasUser(Client* client) const
{
    return _users.count(client) > 0;
}

void Channel::addOperator(Client* client)
{
    if (hasUser(client))
        _operators.insert(client);
}

void Channel::removeOperator(Client* client)
{
    _operators.erase(client);
}

bool Channel::isOperator(Client* client) const
{
    return _operators.count(client) > 0;
}

void Channel::setTopic(const std::string& topic)
{
    _topic = topic;
}

void Channel::setPassword(const std::string& password)
{
    _password = password;
}

void Channel::setInviteOnly(bool value)
{
    _inviteOnly = value;
}

void Channel::setTopicRestricted(bool value)
{
    _topicRestricted = value;
}

void Channel::setUserLimit(int limit)
{
    _userLimit = limit;
}
