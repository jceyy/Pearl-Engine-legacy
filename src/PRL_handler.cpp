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

void PRL_Handler :: loadAnimation(const std::string& file_path)
{
	PRL_Animation* anim = new PRL_Animation(file_path, renderer[0]);
	animation.push_back(anim);
}
