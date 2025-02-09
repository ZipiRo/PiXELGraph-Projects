Color color1;
Color color2;
Color color3;
Color color4;

void ParseColorPalette(int index)
{
    if(index == 1)
    {
        color1 = Color(9, 18, 44);
        color2 = Color(135, 35, 65);
        color3 = Color(190, 49, 68);
        color4 = Color(225, 117, 100);
    }
    else if(index == 2)
    {
        color1 = Color(128, 136, 54);
        color2 = Color(255, 191, 0);
        color3 = Color(255, 154, 0);
        color4 = Color(209, 3, 99);
    }
    else if(index == 3)
    {
        color1 = Color(54, 116, 181);
        color2 = Color(87, 143, 202);
        color3 = Color(161, 227, 249);
        color4 = Color(209, 248, 239);
    }
    else if(index == 4)
    {
        color1 = Color(229, 32, 32);
        color2 = Color(251, 165, 24);
        color3 = Color(249, 203, 67);
        color4 = Color(168, 156, 41);
    }
    else if(index == 5)
    {
        color1 = Color(0, 9, 87);
        color2 = Color(52, 76, 183);
        color3 = Color(87, 123, 193);
        color4 = Color(255, 235, 0);
    }
    else if(index == 6)
    {
        color1 = Color(255, 32, 78);
        color2 = Color(160, 21, 62);
        color3 = Color(93, 14, 65);
        color4 = Color(0, 34, 77);
    }
    else
    {
        color1 = Color(247, 247, 247);
        color2 = Color(222, 222, 222);
        color3 = Color(57, 62, 70);
        color4 = Color(146, 154, 171);
    }
}