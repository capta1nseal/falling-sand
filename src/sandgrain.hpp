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
    SandGrain(int baseR, int baseG, int baseB);
    SandGrain(double baseH, double baseS, double baseV);

    void operator=(const bool &other);

    bool operator==(bool other);

private:

};

#endif
