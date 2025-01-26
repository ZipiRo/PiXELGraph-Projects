#pragma once

enum Color
{
    Transparent,
    Black, Red, Green, Yellow, Blue, Purple, Cyan, White,
    IntenseBlack, IntenseRed, IntenseGreen, IntenseYellow,
    IntenseBlue, IntensePurple, IntenseCyan, IntenseWhite
};

std::string ToAnsi(Color color) {
    switch (color) {
        case Transparent: return "";
        case Black: return "\033[40m";
        case Red: return "\033[41m";
        case Green: return "\033[42m";
        case Yellow: return "\033[43m";
        case Blue: return "\033[44m";
        case Purple: return "\033[45m";
        case Cyan: return "\033[46m";
        case White: return "\033[107m";
        case IntenseBlack: return "\033[100m";
        case IntenseRed: return "\033[101m";
        case IntenseGreen: return "\033[102m";
        case IntenseYellow: return "\033[103m";
        case IntenseBlue: return "\033[104m";
        case IntensePurple: return "\033[105m";
        case IntenseCyan: return "\033[106m";
        case IntenseWhite: return "\033[47m";
        default: return ""; // Handle invalid input
    }
}