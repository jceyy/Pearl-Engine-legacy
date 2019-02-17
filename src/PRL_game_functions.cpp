#include "PRL_game_functions.h"

PRL_Vector PRL_GetMaxDistanceBetween2AABBs(PRL_Rect obj1, PRL_Rect obj2) ///It could be used to scale camera. It works properly if there isn't any collision between the 2 objects considered.
{
    PRL_Vector returnVector;

    if (obj1.x>obj2.x) returnVector.x=obj1.x+obj1.w-obj2.x; // compute for the x coordinate
        else returnVector.x=obj2.x+obj2.w-obj1.x;

    if (obj1.y>obj2.y) returnVector.y=obj1.y+obj1.h-obj2.y;// compute for the y coordinate
        else returnVector.y=obj2.y+obj2.h-obj1.y;

    return returnVector;
}
