#pragma once

namespace Color
{
    typedef std::string Color;

    const Color Transparent("");

    const Color Black("\033[40m");
    const Color Red("\033[41m");
    const Color Green("\033[42m");
    const Color Yellow("\033[43m");
    const Color Blue("\033[44m");
    const Color Purple("\033[45m");
    const Color Cyan("\033[46m");
    const Color White("\033[107m");

    const Color IntenseBlack("\033[100m");
    const Color IntenseRed("\033[101m");
    const Color IntenseGreen("\033[102m");
    const Color IntenseYellow("\033[103m");
    const Color IntenseBlue("\033[104m");
    const Color IntensePurple("\033[105m");
    const Color IntenseCyan("\033[106m");
    const Color IntenseWhite("\033[47m");
}