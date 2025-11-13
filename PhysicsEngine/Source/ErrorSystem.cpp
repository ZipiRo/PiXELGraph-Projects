#include "Console/ErrorSystem.h"

Error::Error(const std::string &message)
{
    this->message = message;
}

const char *Error::what() const noexcept
{
    return message.c_str();
}