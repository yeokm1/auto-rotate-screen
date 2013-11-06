#ifndef ORIENTSETTINGS_H
#define ORIENTSETTINGS_H

class OrientSettings
{


public:
    const int initialMin = 999;
    const int initialMax = 0;


    OrientSettings(){reset();}

    int min_x;
    int max_x;
    int min_y;
    int max_y;
    bool in_use;
    bool calibrated;


    void reset(){min_x = initialMin; max_x = initialMax; min_y = initialMin; max_y = initialMax;
                 in_use = true; calibrated = false;}
};

#endif // ORIENTSETTINGS_H
