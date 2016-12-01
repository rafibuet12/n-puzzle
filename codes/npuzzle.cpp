#include <bits/stdc++.h>

#define vVector vector< vector<int> >
#define pb push_back

#define to_string( x ) static_cast< std::ostringstream & >(\
        ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;

int N; // NxN tile

vVector startG,goalG; //global vector declaration for start state & goal state

vVector inputVector(vVector tempV)
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cin>>tempV[i][j];
        }
    }

    return tempV;
}

void printVector(vVector tempV)
{
    for(int i=0; i<tempV.size(); i++)
    {
        for(int j=0; j<tempV[i].size(); j++)
        {
            cout<<tempV[i][j]<<" ";
        }
        cout<<endl;
    }

}


bool validPos(int tempRow,int tempCol)
{
    return (tempRow<=N-1 && tempRow>=0 && tempCol<=N-1 && tempCol>=0);

}

vector<vVector> getNbr(vVector tempGraph)
{
    int rowZ,colZ;
    for(int i=0; i<tempGraph.size(); i++) //finding row,col of zero tile
    {
        for(int j=0; j<tempGraph[i].size(); j++)
        {
            if(tempGraph[i][j]==0)
            {
                rowZ=i;
                colZ=j;
            }
        }
    }

    int nbrMaskRow[]= {0,0,-1,+1};
    int nbrMaskCol[]= {-1,+1,0,0};

    vector<vVector> tempNbrVector;
    vVector tempNbr;

    for(int i=0; i<4; i++)
    {
        tempNbr=tempGraph;

        if(validPos(rowZ+nbrMaskRow[i],colZ+nbrMaskCol[i]))
        {
            swap(tempNbr[rowZ+nbrMaskRow[i]][colZ+nbrMaskCol[i]],tempNbr[rowZ][colZ]);

            tempNbrVector.pb(tempNbr);
        }

    }

    return tempNbrVector;


}

string idMaker(vVector tempV)
{
    string tempID="";

    for(int i=0; i<tempV.size(); i++)
    {
        for(int j=0; j<tempV[i].size(); j++)
        {
            tempID=tempID+"|"+to_string(tempV[i][j]);
        }
        //cout<<endl;
    }

    return tempID;

}

int calcHCost1(vVector tempStartV,vVector tempGoalV)
{
    //manhattan distance: |x1-x2|+|y1-y2|

    int sum=0;
    for(int i=0; i<tempStartV.size(); i++)
    {
        for(int j=0; j<tempStartV[i].size(); j++)
        {
            if(tempStartV[i][j]!=0)
            {
                for(int k=0; k<tempGoalV.size(); k++)
                {
                    for(int l=0; l<tempGoalV[k].size(); l++)
                    {
                        if (tempStartV[i][j]==tempGoalV[k][l])
                        {
                            sum+=abs(i-k)+abs(j-l);
                        }
                    }
                }
            }
        }
    }

    return sum;

}

int calcHCost2(vVector tempStartV, vVector tempGoalV)
{
    //hammingDistance= sum of mismatch in every bit
    int hammingDistance=0;

    for(int i=0; i<tempStartV.size(); i++)
    {
        for(int j=0; j<tempStartV[i].size(); j++)
        {
            hammingDistance+=tempStartV[i][j] ^ tempGoalV[i][j];
        }
    }

    return hammingDistance;

}

bool isSolvable(vVector tempStartV, vVector tempGoalV)
{
    map<int,int> tradGoalToNewGoalMap; //map[tradGoal]=actualGoal;
    map<int,int> newGoalToTradGoalMap; //map[actualGoal]=tradGoal;

    int count=1;
    for(int i=0;i<tempGoalV.size();i++)
    {
        for(int j=0;j<tempGoalV[i].size();j++)
        {
            newGoalToTradGoalMap[tempGoalV[i][j]]=count%(N*N);
            count++;
        }
    }

    count=1;
    for(int i=0;i<tempGoalV.size();i++)
    {
        for(int j=0;j<tempGoalV[i].size();j++)
        {
            tradGoalToNewGoalMap[count%(N*N)]=tempGoalV[i][j];
            count++;
        }
    }

    vVector newMappedStart;
    newMappedStart.assign(N,vector<int>(N));

    vector<int> plainArray;
    for(int i=0;i<tempStartV.size();i++)
    {
        for(int j=0;j<tempStartV[i].size();j++)
        {
            newMappedStart[i][j]= newGoalToTradGoalMap[tempStartV[i][j]];
            plainArray.pb(newMappedStart[i][j]);
        }
    }

    int inversionCount=0;
    for(int i=0;i<plainArray.size()-1;i++)
    {
        for(int j=i+1;j<plainArray.size();j++)
        {
            if(plainArray[i] && plainArray[j] && plainArray[i]>plainArray[j])
            {
                inversionCount++;
            }
        }
    }

    return (inversionCount%2==0);

}


class state
{
public:

    vVector infoVector;

    string id="";
    string parentID="";
    vector<vVector > nbrVector;

    int gCost;
    int hCost;
    int fCost;

    state()
    {

    }

    state(vVector tempInfo)
    {
        infoVector=tempInfo;
        gCost=0;
        hCost=calcHCost2(this->infoVector,goalG);    //need to be modified later
        fCost=gCost+hCost;

        id=idMaker(tempInfo);
        nbrVector=getNbr(tempInfo);
    }

    void printState()
    {
        printVector(infoVector);
    }

    void printNbr()
    {
        cout<<"Neighbours:"<<endl;
        for(int i=0; i<nbrVector.size(); i++)
        {
            printVector(nbrVector[i]);
            cout<<endl;
        }
    }

    bool operator<(const state &rhs) const //overloaded for set
    {
        if(fCost== rhs.fCost) return id<rhs.id;
        if(fCost>rhs.fCost) return false;
        return true;
    }


    state(const state &rhs) //copy constructor overloaded
    {
        this->infoVector=rhs.infoVector;
        this->nbrVector=rhs.nbrVector;
        this->gCost=rhs.gCost;
        this->hCost=rhs.hCost;
        this->fCost=rhs.fCost;
        this->id=rhs.id;
        this->parentID=rhs.parentID;

    }

};


map<string,string> navigatedNodes;

int moves;
int nodesExplored=1;    //cause explores start state at least

bool aStar(state start, state goal) //returns true if finds goal state
{
    map<string,int> objFCostMap;    //mapping of states to their fCost

    map<string,bool> closedSet; // explored nodes
    set<state> openSet;         //priority queue or fringe

    openSet.insert(start);
    objFCostMap[start.id]=start.fCost;  //whenever anything gets inserted in fringe, its cost is updated/checked if needed to update



    state current;  //to hold present node being explored

    set<state>::iterator it;
    while(!openSet.empty())
    {
        it=openSet.begin();
        current=*it;

        openSet.erase(it);
        closedSet[current.id]=true;  //gets removed from openset; exploration complete

        if (current.id==goal.id) return true;   //returns true only here




        int tentG; //needed for relaxation
        for(int i=0; i<current.nbrVector.size(); i++)   //taking each neighbour
        {
            if (closedSet[idMaker(current.nbrVector[i])]==true) continue;   //if already explored, no more exploration

            state tempNbrObj(current.nbrVector[i]);

            tentG=current.gCost+1;      // al neighbours of current node has this gCost now


            if (objFCostMap.find(tempNbrObj.id)==objFCostMap.end()) //if not found in openset,add them [as each node is mapped while being inserted, this approach is followed]
            {
                nodesExplored++;        //here, node explored count is kept

                tempNbrObj.gCost=tentG;
                tempNbrObj.fCost=tempNbrObj.gCost+tempNbrObj.hCost;
                tempNbrObj.parentID=current.id;

                navigatedNodes[tempNbrObj.id]=tempNbrObj.parentID;
                objFCostMap[tempNbrObj.id]=tempNbrObj.fCost;        //once inserted, cost of a node is mapped accordingly

                openSet.insert(tempNbrObj);
            }
            else                        //neighbour already lies in openSet,but unexplored. they might be relaxed
            {
                tempNbrObj.fCost=objFCostMap[tempNbrObj.id];    //this cost is set to find the old object already lying

                it=openSet.find(tempNbrObj);
                state foundObj=*it;

                if(tentG<foundObj.gCost)
                {
                    openSet.erase(foundObj);    //old object removed
                    foundObj.gCost=tentG;
                    foundObj.fCost=foundObj.gCost+foundObj.hCost;
                    foundObj.parentID=current.id;

                    navigatedNodes[foundObj.id]=foundObj.parentID;
                    objFCostMap[foundObj.id]=foundObj.fCost;    //new relaxed cost updated to the map

                    openSet.insert(foundObj);   //inserted as new object

                }

            }
        }

    }

    return false;

}


void idToStatePrint(string tempStr) //removes delimetre from state.id and prints as a state
{
    int count=0;
    for(int i=1;i<tempStr.size();i++)
    {
        if(tempStr[i]!='|')
        {
            cout<<tempStr[i];
        }
        else
        {
            cout<<" ";
            count++;
            if (count%N==0) cout<<endl;
        }
    }
    cout<<endl<<endl;

}

void pathPrint(string start, string current)
{
    moves++;    // no of total moves calculated
    if (current==start)
    {
        idToStatePrint(current);    //prints from start/parent node
        return;
    }

    pathPrint(start,navigatedNodes[current]);   //goes to parent
    idToStatePrint(current);    //prints current node after printing parent

}

int main()
{
    freopen("in.txt","r",stdin);

    int n;  //n= no of tiles except space
    cin>>n;

    N=sqrt(n+1);

    startG.assign(N,vector<int>(N)),goalG.assign(N,vector<int>(N));

    startG=inputVector(startG);
    goalG=inputVector(goalG);

    /*objectification*/

    state startObj(startG);
    state goalObj(goalG);

    string start=idMaker(startG);
    string goal=idMaker(goalG);

    //cout<<start<<goal;

    cout<<"Start State:"<<endl;
    startObj.printState();
    cout<<endl;

    cout<<"Goal State:"<<endl;
    goalObj.printState();
    cout<<endl;


    if(isSolvable(startG,goalG)==0)
    {
        cout<<"Cannot be solved"<<endl;
    }
    else
    {
        bool aStarCompleted=aStar(startObj,goalObj);

        if(aStarCompleted==true)
        {
            cout<<"Goal State Found!"<<endl<<endl;

            cout<<"Moves:"<<endl;
            pathPrint(startObj.id,goalObj.id);
            cout<<"Total No of Moves: "<<moves-1<<endl;     //cause moves variable included start state also
            cout<<"Total Nodes Explored: "<<nodesExplored<<endl;
        }
        else
        {
            cout<<"A star Search couldn't found GoalState."<<endl;
        }

    }


    return 0;
}
