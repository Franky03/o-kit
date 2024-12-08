#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <ilcplex/ilocplex.h>
#include "Data.h"

void BnC(Data *data, string instance, double ub){
    
}

int main(int argc, char** argv)
{
    double ub = numeric_limits<double>::max();

    if(argc < 2){
        printf("Correct command: ./bc data/instance\n");
        return 0;
    }
    else if(argc > 2){
        ub = stod(argv[2]);
    }

    Data * data = new Data(argc, argv[1]);
    data->readData();

    string instanceName = data->getInstanceName();
    
    return 0;
}