#ifndef PRL_TESTZONE_H_INCLUDED
#define PRL_TESTZONE_H_INCLUDED

#include "PRL.h"

void printAnimDiagnostics(PRL_Animation* anim);

class TestClass : PRL_Displayable, PRL_Collidable
{
public:
    TestClass();
    ~TestClass();

private:

};

#endif // PRL_TESTZONE_H_INCLUDED
