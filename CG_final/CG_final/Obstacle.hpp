#pragma once
#include <iostream>

class Obstacle {
public:

    Obstacle(){
        isFowrad = 1;
        moveHorizontal = 1;
        moveVertical = 0;
        movingSpeed = 100.0f;
    }

    int isFowrad;
    int moveHorizontal;
    int moveVertical;
    std::pair<int, int> particleIndexRange;
    std::pair<int, int> horizontalBound;
    std::pair<int, int> verticalBound;
    float movingSpeed;
};
