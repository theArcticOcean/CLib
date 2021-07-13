#include <iostream>
#include "BasicIO.h"

using namespace std;

int main(int argc, char *argv[])
{
    auto folderPath = argv[1];
    BasicIO::RemoveDir( folderPath );
    return 0;
}
