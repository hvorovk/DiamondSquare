#include <iostream>
#include "ds.h"

int main(int argc, char *argv[])
{
    DS generator(4);
    auto temp = generator.getMap();
    for(int i = 0; i < temp.size(); i++) {
        for(int j = 0; j <temp.size(); j++) {
            std::cout<< temp[i][j] << " ";
        }
        std::cout<< "\n";
    }
    return 0;
}
