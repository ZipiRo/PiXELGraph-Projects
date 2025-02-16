#pragma once

class Error : public std::exception
{
private:
    std::string message;

public:
    explicit Error(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }
};
