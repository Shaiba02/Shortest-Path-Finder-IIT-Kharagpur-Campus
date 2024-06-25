#include<iostream>
using namespace std;
#include<algorithm>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include<stack>
#include<unordered_map>

void openFile(ifstream &inFile, string file_name);
void processFile(ifstream &inFile_graph, ifstream &inFile_nodes, vector<vector<double>> &graph, 
            vector<string> &nodes_name, unordered_map<string, int> &mpp, string pair, 
            string line_graph, string line_nodes, int i);
void make_distance_parent_matrix(int n, vector<vector<double>> graph, vector<vector<double>> &dist,
            vector<vector<double>> &parent);
void update_distance_parent_matrix(int n, vector<vector<double>> &dist, 
            vector<vector<double>> &parent);
string makeChoice();
void choose_src_dest(vector<vector<double>> dist, vector<vector<double>> parent, 
            unordered_map<string, int> &mpp, string &source_node, string &destination_node);
double findPath(string source_node, string destination_node, vector<vector<double>> dist, 
            vector<vector<double>> parent, unordered_map<string, int> mpp, 
            vector<string>nodes_name, vector<string> &path);

int main(){
    int n = 31;
    vector<vector<double>> graph(n, vector<double>(n, -1));
    vector<vector<double>> dist(n, vector<double>(n, 0));
    vector<vector<double>> parent(n, vector<double>(n,-1));
    vector<string> nodes_name;
    string source_node; string destination_node;
    vector<string> path;
    unordered_map<string, int> mpp;
    string pair; string line_graph; string line_nodes; int i=1; 

    ifstream inFile_graph;
    ifstream inFile_nodes;

    //opening files
    openFile(inFile_graph, "file_graph.txt");
    openFile(inFile_nodes, "file_nodes_name.txt");

    //reading files
    processFile(inFile_graph, inFile_nodes, graph, nodes_name, mpp, pair, line_graph, line_nodes, i);
    //making distance and parent matrix
    make_distance_parent_matrix(n, graph, dist, parent);

    //updating distance and parent matrix
    update_distance_parent_matrix(n, dist, parent);

    // user input
    string choice = makeChoice();
    if(choice == "2"){
        cout<<"THANK YOU!";            
        exit(-1);
    }
    cin.ignore();

    // take source and destination as input
    choose_src_dest(dist, parent, mpp, source_node, destination_node);
    //cout<<source_node<<" "<<destination_node<<endl;
    
    //find minimum distance and path from source to destination
    double minDist = findPath(source_node, destination_node, dist, parent, mpp, nodes_name, path);

    //printing minimum distance and path from source to destination
    cout<<endl<<"Minimum distance from "<<source_node<<" to "<<destination_node<<" is "<<minDist<<" km "<<endl<<endl;
    cout<<"Best Path is: "<<endl;
    for(int i=0;i<path.size();i++){
        if(i!=path.size()-1) cout<<path[i]<<" -> ";
        else cout<<path[i]<<endl<<endl;
    }

}


// All the functions used are below: 
void openFile(ifstream &inFile, string file_name){
    inFile.open(file_name);
    if(!inFile.is_open()){
        cout<<"Error! Failed to open file" <<endl;
        exit(-1);
    }
}

void processFile(ifstream &inFile_graph, ifstream &inFile_nodes, vector<vector<double>> &graph, 
            vector<string> &nodes_name, unordered_map<string, int> &mpp, string pair, string line_graph, string line_nodes, int i){
    nodes_name.push_back("");
    while(getline(inFile_graph, line_graph) && getline(inFile_nodes, line_nodes)){
        nodes_name.push_back(line_nodes);
        mpp[line_nodes] = i;
        stringstream iss(line_graph);
        string pair;
        while(iss>>pair){
            stringstream ss(pair);
            string v; string wt;
            getline(ss, v, ',');
            getline(ss, wt, ',');
            int num_v = stoi(v); double num_wt = stof(wt);
            graph[i][num_v] = num_wt;
            //cout<<num_v<<" "<<num_wt<<" ";
        }
        i++;
        //cout<<endl;
    }
}
void make_distance_parent_matrix(int n, vector<vector<double>> graph, vector<vector<double>> &dist, 
            vector<vector<double>> &parent){
    for(int i=1;i<n;i++){
        for(int j=1; j<n; j++){
            if(graph[i][j] == -1){
                //implies there's no path from i to j
                dist[i][j] = 1e9;   
            }
            else if(graph[i][j]!=0){
                //i->j(edge from i to j) implies i is the parent of j
                parent[i][j] = i;        
                //copying values from graph to distance matrix 
                dist[i][j] = graph[i][j];
            }
        }
    }
}
void update_distance_parent_matrix(int n, vector<vector<double>> &dist, 
            vector<vector<double>> &parent){
    for(int via = 1; via<n; via++){
        for(int i=1; i<n; i++){
            for(int j=1; j<n; j++){
                if(dist[i][via] + dist[via][j] < dist[i][j]){
                    //updating minimum distance
                    dist[i][j] = dist[i][via] + dist[via][j]; 
                    //updating immediate parent of node
                    parent[i][j] = via;                         
                }
            }
        }   
    }
}
string makeChoice(){
    cout<<"1. Search Path "<<endl<<"2. Exit "<<endl;
    cout<<"Enter your choice(1 or 2) : ";
    string choice; cin>>choice;
    if(choice.size()==0 || choice.size()>1){
        cout<<"Invalid input! Choose again"<<endl;
        choice = makeChoice();
    }
    char ch = choice[0];
    if(isdigit(ch)){
        if(ch=='2' || ch=='1') return choice;
        else{
            cout<<"Invalid input! Choose again"<<endl;
            choice = makeChoice();
        }
    }
    else{
        cout<<"Invalid input! Choose again"<<endl;
        choice = makeChoice();
    }
    return "";
}


void choose_src_dest(vector<vector<double>> dist, vector<vector<double>> parent, 
    unordered_map<string, int> &mpp, string &source_node, string &destination_node){
    for(auto it: mpp) cout<<it.first<<endl;
    cout<<endl;
    while(true){
        cout<<"Enter source: "; 
        getline(cin, source_node);
        transform(source_node.begin(), source_node.end(), source_node.begin(), ::toupper);
        cout<<"Enter destination: "; 
        getline(cin, destination_node);
        transform(destination_node.begin(), destination_node.end(), destination_node.begin(), ::toupper);
        if(mpp.find(source_node)==mpp.end() || mpp.find(destination_node)==mpp.end()){
            cout<<"Invalid! Please choose from the List"<<endl<<endl;
        }
        else if(source_node==destination_node){
            cout<<"Source and destination cannot be the same. Please choose again "<<endl<<endl;
        }
        else {
            break;
        }
    }
}
double findPath(string source_node, string destination_node, vector<vector<double>> dist, 
            vector<vector<double>> parent, unordered_map<string, int> mpp, 
            vector<string> nodes_name, vector<string> &path){
    int src_node_num = mpp[source_node];
    int dest_node_num = mpp[destination_node];
    stack<int> st;
    int node = dest_node_num;
    st.push(node);
    while(parent[src_node_num][node]!=src_node_num){
        node = parent[src_node_num][node];
        st.push(node);
    }
    st.push(src_node_num);
    while(!st.empty()){
        int index = st.top();
        path.push_back(nodes_name[index]);
        st.pop();
    }  
    return dist[src_node_num][dest_node_num]; 

} 


