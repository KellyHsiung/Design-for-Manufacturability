#include "gurobi_c++.h"
#include "../bits/stdc++.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <map>
#include <algorithm>
using namespace std;

class Vertex
{
  public:
    Vertex(string _name = "", int _value = 0.0)
        : name(_name), value(_value), acIdList(new set<pair<int, int>>()) {}
    ~Vertex()
    {
        delete acIdList;
    }
    string name;
    int value;
    set<pair<int, int>> *acIdList;
};
class Edge
{
  public:
    Edge(int _s, int _t, int _value) : s(_s), t(_t), value(_value) {}

    int s;
    int t;
    int value;
};
static unordered_map<string, int> mapName2Id;
static vector<Vertex *> vecVertices;
static vector<Edge *> vecEdge;
static vector<int> selectpeople;
static vector<int> selectedge;
static ifstream ifnv, ifas;
static ofstream ofrpt;
int edgesize;

void printUsage(string exe)
{
    cerr << "Usage: " << exe << " [.nv] [.as] [.rpt]" << endl;
    exit(EXIT_FAILURE);
}

void fileOpenError(string name)
{
    cerr << "Error: cannot open file \"" << name << "\"" << endl;
    exit(EXIT_FAILURE);
}

void parseCmd(int &argc, char **argv)
{
    if (argc != 4)
        printUsage(argv[0]);
    ifnv.open(argv[1], ios::in);
    if (!ifnv.is_open())
        fileOpenError(argv[1]);
    ifas.open(argv[2], ios::in);
    if (!ifas.is_open())
        fileOpenError(argv[2]);
    ofrpt.open(argv[3], ios::out);
    if (!ofrpt.is_open())
        fileOpenError(argv[3]);
}

void readNv()
{
    string name;
    int value;
    int i = 0;
    while (ifnv >> name >> value)
    {
        if (!mapName2Id.count(name))
        {
            mapName2Id[name] = i++; //name -> first, i -> second
            vecVertices.push_back(new Vertex(name, value));
        }
    }
}

void readAs()
{
    string name1, name2;
    int value;
    while (ifas >> name1 >> name2 >> value)
    {
        bool isNotExist = false;
        auto it1 = mapName2Id.find(name1);
        auto it2 = mapName2Id.find(name2);
        if (it1 == mapName2Id.end())
        {
            cerr << "\tWarning: \"" << name1 << "\" is not is the .nv file\n";
            isNotExist = true;
        }
        if (it2 == mapName2Id.end())
        {
            cerr << "\tWarning: \"" << name2 << "\" is not is the .nv file\n";
            isNotExist = true;
        }
        if (isNotExist)
            continue;
        vecVertices[it1->second]->acIdList->insert(make_pair(it2->second, value));
        vecVertices[it2->second]->acIdList->insert(make_pair(it1->second, value));
        vecEdge.push_back(new Edge(it1->second, it2->second, value));
    }
}
int totalValue = 0;
void solveILP()
{
    try
    {
        // Create gurobi environment & model
        GRBEnv env = GRBEnv();
        GRBModel model = GRBModel(env);
        // Set problem name
        model.set(GRB_StringAttr_ModelName, "ILP1");

        // Create variables
        // Set binary decision variables x[] (constraint 6)
        // Set x[] names
        vector<GRBVar> x_v;
		x_v.resize(vecVertices.size());
		x_v.shrink_to_fit();
        for(size_t i=0; i<vecVertices.size(); ++i){
            x_v[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY);
        }
        
        // Add variable y (constraint 7)
        vector<GRBVar> y_e;
		y_e.resize(vecEdge.size());
		y_e.shrink_to_fit();
        for(size_t i=0; i<vecEdge.size(); ++i){
            y_e[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY);
        }

        // Set binary decision variables z[] (constraint 8)
        // Set z[] names
        vector<GRBVar> z_e;
		z_e.resize(vecEdge.size());
		z_e.shrink_to_fit();
        for(size_t i=0; i<vecEdge.size(); ++i){
            z_e[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY);
        }

        // Set objective
        GRBLinExpr* obj = new GRBLinExpr();
        for(size_t i=0; i<vecVertices.size(); ++i){
            *obj += x_v[i] * vecVertices[i]->value;
        }
        for(size_t i=0; i<vecEdge.size(); ++i){
            *obj += z_e[i] * vecEdge[i]->value;
        }
        model.setObjective(*obj, GRB_MINIMIZE);


        // Add constraint (constraint 1)
        for(size_t i=0; i<vecEdge.size(); ++i){
            GRBLinExpr* con = new GRBLinExpr();
            *con += x_v[vecEdge[i]->s] + x_v[vecEdge[i]->t];
            model.addConstr(*con, GRB_LESS_EQUAL, 1.0);
            delete con;
        }
        

        // Add constraint (constraint 2)
        for(size_t i=0; i<vecEdge.size(); ++i){
            GRBLinExpr* con = new GRBLinExpr();
            *con += x_v[vecEdge[i]->s] + x_v[vecEdge[i]->t];
            model.addConstr(*con, GRB_EQUAL, y_e[i]);
            delete con;
        }

        // Add constraint (constraint 3)
        for(size_t i=0; i<vecEdge.size(); ++i){
            GRBLinExpr* con = new GRBLinExpr();
            model.addConstr(z_e[i], GRB_LESS_EQUAL, y_e[i]);
            delete con;
        }

        // Add constraint (constraint 4)
        // for(unsigned i=0; i<vecVertices.size(); ++i){
        //     GRBLinExpr* con = new GRBLinExpr();
        //     *con += x_v[i];
        //     // all neighbor vertex
        //     for(unsigned j=0; j<vecEdge.size(); ++j){
        //         if(vecEdge[j]->s == i || vecEdge[j]->t == i)
        //             *con += x_v[vecEdge[j]->t];  
        //     }
        //     model.addConstr(*con, GRB_GREATER_EQUAL, 1);
        //     delete con;
        // }
        for(unsigned i=0; i<vecVertices.size(); ++i){
            GRBLinExpr* con = new GRBLinExpr();
            *con += x_v[i];
            // all neighbor vertex
            for(auto it = vecVertices[i]->acIdList->begin(); it != vecVertices[i]->acIdList->end(); ++it){
                *con += x_v[it->first];  
            }
            model.addConstr(*con, GRB_GREATER_EQUAL, 1.0);
            delete con;
        }

        // Add constraint (constraint 5)
        for(unsigned i=0; i<vecVertices.size(); ++i){
            GRBLinExpr* con = new GRBLinExpr();
            *con += x_v[i];
            // all connected edge
            for(unsigned j=0; j<vecEdge.size(); ++j){
                if(vecEdge[j]->s == i || vecEdge[j]->t == i)
                    *con += z_e[j];
            }
            model.addConstr(*con, GRB_GREATER_EQUAL, 1.0);
            delete con;
        }

        // Optimize model
        model.optimize();
        // Write formula to a file
        model.write("ILP1.lp");


        for(unsigned i=0; i<vecVertices.size(); ++i){
            if(x_v[i].get(GRB_DoubleAttr_X) == 1)
                selectpeople.push_back(i);
        }
        for(unsigned i=0; i<vecEdge.size(); ++i){
            if(z_e[i].get(GRB_DoubleAttr_X) == 1)
                selectedge.push_back(i);
        }


        // Check optimal
        if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL)
        {
            double objval = model.get(GRB_DoubleAttr_ObjVal);
            cout << "/////////////////////////Optimal objective value = " << objval << endl;
            totalValue = objval;
        }
        else
            cout << "The model is infinity or unbounded or infeasible.\n";
    }
    catch (GRBException e)
    {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    }
    catch (...)
    {
        cout << "Exception during optimization" << endl;
    }
}

int cmp(int a, int b)
{
    return (vecVertices[a]->name < vecVertices[b]->name);
}
void writeRpt()
{
    
    // for (auto &i : selectpeople)
    //     totalValue += vecVertices[i]->value;
    // for (auto &i : selectedge)
    //     totalValue += vecEdge[i]->value;
    sort(selectpeople.begin(), selectpeople.end(), cmp);
    ofrpt << "Cost function = " << totalValue << endl;
    ofrpt << endl;
    ofrpt << "Dominating Independent Set : " << endl;
    ofrpt << endl;
    for (auto &i : selectpeople)
    {
        ofrpt << vecVertices[i]->name << " ";
        ofrpt << endl;
    }
}

int main(int argc, char **argv)
{
    parseCmd(argc, argv);
    readNv();
    readAs();
    solveILP();
    writeRpt();
}
