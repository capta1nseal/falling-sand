#ifndef _FALLINGSANDSANDGRAIN
#define _FALLINGSANDSANDGRAIN

class SandGrain
{
public:
    bool exists;
    unsigned char r;
    unsigned char g;
    unsigned char b;

    SandGrain();
    SandGrain(bool value);
    SandGrain(int r, int g, int b);

    void operator=(const bool &other);

    bool operator==(bool other);

private:

};

#endif
