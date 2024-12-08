#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <ilcplex/ilocplex.h>
#include "Data.h"

void Solver(Data *data, string instance, double ub){
    IloEnv env;
    IloModel model(env);

    env.setName("Branch and Cut");
    model.setName("Symmetrical Traveling Salesman Problem");

    int dimension = data->getDimension();
    /******** Creating one variable ’x’ for each edge *******/
    IloArray <IloBoolVarArray> x(env, dimension);

    for (int i = 0; i < dimension; i++) {
        IloBoolVarArray array(env, dimension);
        x[i] = array;
    }

    /********** Adding variables ’x’ to the model **********/
    char var[100];
    for (int i = 0; i < dimension; i++){
        for (int j = i + 1; j < dimension; j++){
            sprintf(var, "X(%d,%d)", i, j);
            x[i][j].setName(var);
            model.add(x[i][j]);
        }
    }

    /******************************************************/
    
    /**************** Objective Function ******************/
    IloExpr obj(env);
    for (int i = 0; i < dimension; i++) {	
		for (int j = i + 1; j < dimension; j++) {
			obj += data->getDistance(i, j)*x[i][j];
		}
	}
    model.add(IloMinimize(env, obj));
    /******************************************************/
    
    /******************** Constraints *********************/

    IloRange r;
    char name[100];

    for (int i = 0; i < dimension; i++){
        IloExpr sumX(env);
        for (int j = 0; j < dimension; j++){
            if (j < i) {
				sumX += x[j][i];
			}
            if (i < j){
                sumX += x[i][j];
            }
        }
        r = (sumX == 2);
        sprintf(name, "c_%d", i);
		r.setName(name);
		model.add(r);
    }
    /******************************************************/
    


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