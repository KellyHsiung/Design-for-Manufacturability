#include "../bits/stdc++.h"
using namespace std;

class Vertex {
    public:
        Vertex(string _name = "", int _value = 0.0)
            : name(_name), value(_value), isInMVWMDS(false),acIdList(new set<pair<int, int> >()) {}
        ~Vertex() {
            delete acIdList;
        }
        string name;
        int value;
        bool isInMVWMDS;
        set<pair<int, int> >* acIdList;
};
class Edge {
    public:
        Edge(int _s, int _t, int _value): s(_s),t(_t),value(_value){}

        int s;
        int t;
        int value;
};

static unordered_map<string, int> mapName2Id;
static vector<Vertex*> vecVertices;
static vector<pair<int,int>> TonyStark;
static vector<Edge*> vecEdge;
static unordered_map<int, bool> MVWMDS;
static ifstream ifnv, ifas, ifrpt;
static size_t numVertices;
static int totalValue, realTotalValue;
static vector<int> selectpeople;
static vector<int> selectedge;
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

void parseCmd(int& argc, char** argv)
{
    if (argc != 4) printUsage(argv[0]);
    ifnv.open(argv[1], ios::in);
    if (!ifnv.is_open())fileOpenError(argv[1]);
    ifas.open(argv[2], ios::in);
    if (!ifas.is_open()) fileOpenError(argv[2]);
    ifrpt.open(argv[3], ios::in);
    if (!ifrpt.is_open()) fileOpenError(argv[3]);
}

void readNv()
{
    string name;
    int value;
    int i = 0;
    while (ifnv >> name >> value) {
        if (!mapName2Id.count(name)) {
            mapName2Id[name] = i++;
            vecVertices.push_back(new Vertex(name, value));

        }
    }
}

void readAs()
{
    string name1, name2;
    int value;
    while (ifas >> name1 >> name2 >> value) {
        bool isNotExist = false;
        auto it1 = mapName2Id.find(name1);
        auto it2 = mapName2Id.find(name2);
        if (it1 == mapName2Id.end()) {
            cerr << "\tWarning: \"" << name1 << "\" is not is the .nv file\n";
            isNotExist = true;
        }
        if (it2 == mapName2Id.end()) {
            cerr << "\tWarning: \"" << name2 << "\" is not is the .nv file\n";
            isNotExist = true;
        }
        if (isNotExist) continue;
        vecVertices[it1->second]->acIdList->insert(make_pair(it2->second,value));
        vecVertices[it2->second]->acIdList->insert(make_pair(it1->second,value));
        vecEdge.push_back(new Edge(it1->second,it2->second,value));
    }
}

bool readRpt()
{
    bool isPass = true;
    string str;
    ifrpt >> str >> str >> str>>totalValue;
    ifrpt >> str >> str >> str >> str;
	string leadername;
    while (ifrpt >> str) {
            auto it = mapName2Id.find(str);
			leadername = str;
            if (it == mapName2Id.end()) {
                cerr << "Error: \"" << str << "\" is not in the .nv file\n";
                isPass = false;
            continue;
            }
            int id = it->second;
            MVWMDS[id] = true;
            vecVertices[id]->isInMVWMDS = true;
    }
    return isPass;
}

bool isNumVerticesMatched()
{
    return numVertices == MVWMDS.size();
}

bool isTotalValueMatched()
{
    realTotalValue = 0;
    for (auto& i : MVWMDS) realTotalValue += vecVertices[i.first]->value;
	for (auto& i : MVWMDS){
		for (auto& j : TonyStark){
			if(vecVertices[i.first]->name == vecVertices[j.first]->name){
				for (auto& k : vecEdge){
					if((k->s == j.first && k->t == j.second)||(k->t == j.first && k->s == j.second)){
						realTotalValue += k->value;
					}
				}
			}
		}
	}
    return totalValue == realTotalValue;
}

bool isSolutionFeasible()
{
    bool isPass = true;
    int f = 0;
    for (auto& u : vecVertices) {
        bool isFeasible = false;
        f = 0;
        //ind check
        if (u->isInMVWMDS){
            for (auto& v : *(u->acIdList)){
                if (vecVertices[v.first]->isInMVWMDS) {
                    f = 1;
                }
            }
            if(!f) continue;
        };
        //domin check
        for (auto& v : *(u->acIdList)) {
            if (vecVertices[v.first]->isInMVWMDS) {
                isFeasible = true;
                break;
            }
        }
        if (!isFeasible || f ==1){
			if(!isFeasible)
				cerr << "\tError: Dominating set violation\n";
			if(f == 1) 
				cerr << "\tError: Independent set violation\n";
			isPass = false;
			break;
		}
    }
    return isPass;
}

int main(int argc, char** argv)
{
    parseCmd(argc, argv);
    cout << "Read .nv file...\n";
    readNv();
    cout << "Read .as file...\n";
    readAs();
    cout << "Read .rpt file...\n";
    bool isPass = readRpt();
    cout << "Check if the solution is feasible...\n";
    if (!isSolutionFeasible()) {
        isPass = false;
    }
	if(isPass)
		cout << "Success: Your result satisfy the dominating independent set\n";
	if(!isPass)
		cout << "Error: Your result is infeasible (Not the dominating independent set)\n";

    // cout << "Check if the total value is feasible...\n";
    // if (!isTotalValueMatched()) {
    //     cout << "Error: Total value is wrong " << totalValue << "\t" << realTotalValue << "\n";
    // }else{
    //     cout << "Success: Your total value is correct\n";
    // }
}
