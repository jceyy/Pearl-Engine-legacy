#include <iostream>

#include "PRL_handler.h"


PRL_Handler :: PRL_Handler()
{
    time.stopwatch.start();
}
PRL_Handler :: ~PRL_Handler()
{
    freeall();
}

void PRL_Handler :: freeall()
{
	;
}
