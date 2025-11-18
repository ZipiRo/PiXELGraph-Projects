#pragma once

#include <exception>
#include <string>

class Error : public std::exception
{
private:
    std::string message;

public:
    explicit Error(const std::string &msg);
    const char *what() const noexcept override;
};
