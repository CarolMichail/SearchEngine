#include <iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<set>
#include<map>
#include<unordered_map>
#include<algorithm>
#include<list>
#include<vector>
using namespace std;
class Graph
{
public:
    int j = 1;
    unordered_multimap<string,string> keywords;
    vector<pair<string, int>> clicks;
    vector<pair<string, int>> impressions;
    map<string,int>score;
    vector<string> results;
    vector<pair<string, int>> s;
    set<string> nodes;
    vector<vector<double>> rank;  //pagerank matrix
    map< string, list< string> > adjacencyList;
    string vertex;
    Graph();
    void addEdge(string v1, string v2);
    void search(string word);
    void writeFiles();
    void readFiles();
    void final_results();
    void print();
    vector<string> getPointingto(string web);
    void pageRank();
    void getscore();
    void CTR();
    void CTR2(string s);
    void All(string word);


};
Graph::Graph()
{
    readFiles();

}
void Graph::addEdge(string v1, string v2)
{
      adjacencyList[v1].push_back(v2);
  
}
void Graph::print()
{
    cout << endl << "The Adjacency List-" << endl;
    for (auto& value : adjacencyList) {
        vertex = value.first;
        list< string> adjacentVertices = value.second;
        list< string> ::iterator itr = adjacentVertices.begin();

        cout << "adjacencyList[" << vertex << "]";
        while (itr != adjacentVertices.end()) {
            cout << " -> " << (*itr);
            ++itr;
        }

        cout << endl;


    }
}
vector<string> Graph::getPointingto(string web)
{
    vector<string> pointers;
    for (auto& value : adjacencyList) {

        vertex = value.first;
        list< string> adjacentVertices = value.second;
        list< string> ::iterator itr = adjacentVertices.begin();

        while (itr != adjacentVertices.end())
        {
            if (web == *itr)
                pointers.push_back(vertex);
            itr++;
        }
    }
    return pointers;
}
void Graph::pageRank()
{
    map<string,double> previous; //store the previous iterations values and nodes
    double initial = 0;
    initial=(double)1/nodes.size(); //initialize the nodes
  //  vector<vector<double>> rank;  //page rank matrix
    vector<double> r; //helper vector for each iteration
    vector<string> p; //store the websites that are pointing to the current website
    int k = 0; 
    set<string>::iterator itr;
    auto it = adjacencyList.begin();
    auto pr = previous.begin();
    pair<string, list<string>> s;
    pair<string, double> pre;
    vector<double> final;
    double f = 0;
    int j = 0;

    for (int i = 0; i < nodes.size(); i++)
    {
        r.push_back(initial);
    
    }
    rank.push_back(r);
    r.clear();
    for (auto itr = nodes.begin(); itr != nodes.end(); itr++) {
      
        previous.insert(make_pair(*itr, rank[0][0]));
      
    }
   
   
    //second iteration
    for (itr = nodes.begin();itr != nodes.end(); itr++)
    {
        p = getPointingto(*itr);
      
        if (p.size() > 0) 
        {
            for (int i = 0; i < p.size(); i++)
            {
               it = adjacencyList.find(p[i]);
        
                s = *it;
                pr=previous.find(p[i]);
                pre = *pr;
                 f = pre.second / s.second.size();

                
                 if (p.size() > 1)
                 {
                     f = 0;
                     final.push_back( pre.second / s.second.size());
                     
                 }
            }
                 k = final.size()-1;
             
                 while (k >= 0) {
                     f += final[k];
                         k--;
                 }
               
             
           
          
        }
        r.push_back(f);
        f = 0;
        final.clear();
    }
    
    for (auto itr = nodes.begin(); itr != nodes.end(); itr++) {

        previous.insert(make_pair(*itr, rank[0][j]));
        j++;
      
    }

    double mini =1000000000;
    for (int i = 0; i < r.size(); ++i) {
        mini = min(r[i], mini);
    }
    for (int i = 0; i < r.size(); ++i) {
        r[i] =ceil(r[i] * 1/mini);
    }
    rank.push_back(r);
 
}
void Graph::CTR()
{
    pair<string, int> imp;
    string line;
    ifstream fin;

    int i = 0;
    int k = 0;

    fin.open("impressions.csv");
    if (fin.fail())
        cout << "failed to open";
    while (!fin.eof())
    {

        getline(fin, line);
        i = 0;
        stringstream ss(line);

        while (ss.good()) {

            getline(ss, line, ',');
            if (i % 2 == 0)
                imp.first = line;
            else {
                imp.second = stoi(line);
                clicks.push_back(imp);
            }
            i++;


        }
    }
    impressions = clicks;
    fin.close();
    for (int i = 0; i < clicks.size(); i++)
    {
        for (int k = 0; k < results.size(); k++)
        {

            if (clicks[i].first == results[k])
            {
                k++;
                to_string(clicks[i].second++);

            }
        }
    }

}
void Graph::getscore()
{
    auto iterator = score.begin();

    int i = 0;
    for (auto itr = nodes.begin(); itr != nodes.end(); itr++) {

        score.insert(make_pair(*itr, (0.4 * rank[1][i]) + (((1 - (0.1 * clicks[i].second) / (1 + 0.1 * clicks[i].second))
            * (rank[1][i]) + (1 - (0.1 * clicks[i].second) / (1 + 0.1 * clicks[i].second)) * clicks[i].second)) * 0.6));
        i++;
    }

    for (int i = 0; i < results.size(); i++) {
        iterator = score.find(results[i]);
        s.push_back(*iterator);
    }

    if (results.size() > 1)
    {
        for (int i = 0; i < s.size() - 1; i++)
        {
            if (s[i].second < s[i + 1].second)
                swap(s[i], s[i + 1]);
        }
        for (int i = 0; i < s.size(); i++)
        {
            cout << i + 1 << ".";
            cout << s[i].first << endl;
        }
    }
    else if (results.size() == 1)

        cout << j << "." << s[0].first << endl;
}
void Graph::CTR2(string s) 
{
    for (int i = 0; i < clicks.size(); i++)
    {
            if (clicks[i].first == s)
            {           
                to_string(clicks[i].second++);
            }
    }
}
void Graph::search(string word)
{
    vector<string>search;
    bool key = false;
    string l;
    if(word[0] == '"' && word.back() == '"')
    {
        word.erase(0, 1);
        word.resize(word.size() - 1);
        search.push_back(word);
    }
  
    
    else {
        for (int i = 0; i < word.size(); i++)
        {
            l += word[i];
            if (word[i] == ' ')
            {
                l.resize(l.size() - 1);
                search.push_back(l);
                l.clear();
            }
            if (i == search.size() - 1)
                search.push_back(l);

        }
        search.push_back(l);
    }
    if (search.size()==1|| word[0] == '"' && word.back() == '"')   //if only one word
    {
        auto itr = keywords.equal_range(word);;
        for (auto it = itr.first; it != itr.second; it++)
        {
        
                results.push_back(it->second);
        }
    }
    else
    {
        for (int i = 0; i < search.size(); i++)
        {

            if (search[i] == "AND") {                           //if there is an "AND"
                key = true;
                auto g = keywords.equal_range(search[i - 1]);
                auto h = keywords.equal_range(search[i + 1]);
                for (auto it = g.first; it != g.second; it++) {

          
                    results.push_back(it->second);
                }
                
                sort(results.begin(), results.end());
               
                int t = results.size() - 1;
                    
                   while(t>0&&t<results.size())
                   {
                       if (!results.empty() && results.size() > 0)
                       {

                           if (results[t] != results[t - 1])
                           {
                               results.pop_back();

                               t - 2;

                           }
                           else if (!results.empty() && results.size() > 1 && results[t] == results[t - 1])
                            
                               t --;

                         t--;
                       }
                    }
                
            }
            else if (search[i] == "OR") {        //if there is an "OR"
                key = true;
                auto g = keywords.equal_range(search[i - 1]);
                auto h = keywords.equal_range(search[i + 1]);
                for (auto it = g.first; it != g.second; it++) {

                    results.push_back(it->second);
                }
                for (auto it = h.first; it != h.second; it++) {
                    results.push_back(it->second);
                }
           
            }
           else if(key==false)
            {
               
                auto g = keywords.equal_range(search[i]);
                for (auto it = g.first; it != g.second; it++) {

                    results.push_back(it->second);
                }
               
              
            }
            
        }
    }
    if (results.size() == 0)
    {
        cout << "No results found\n";
    }
   
   
     if(results.size()>1) 
    {
           sort(results.begin(), results.end());
           results.erase(unique(results.begin(), results.end()), results.end());  
    }
}
void Graph:: writeFiles()
{
    ofstream fout;
    string write;
    fout.open("impressions.csv");
    if (fout.is_open())
    {
       
        for (int i = 0; i < clicks.size(); i++) {
            write = to_string(clicks[i].second);
            fout << clicks[i].first << "," << write << endl;
        }
        fout.close();
    }
   
}
void Graph::readFiles()
{
    string line;
    string temp1;
    string temp2;
    ifstream fin;
    string t;
    int num = 0;
    int j = 0;
    int i = 0;

    fin.open("websites.csv");

    while (!fin.eof())
    {
        getline(fin, line);
        t = line;
        stringstream s(line);
        j = 0;
        while (getline(s, temp1,',') )
        {
            if (j == 0) {
                temp2 = temp1;
            }
            j++;
           
           nodes.insert(temp1);
           if (temp1 != temp2)
           {
               addEdge(temp2, temp1);
           }
        }
       
    
    }
    fin.close();
 
    int k = 0;
    int l = 0;
    pair<string, string> m;
    fin.open("keyword.csv");
    
    while (!fin.eof())
    {
        i = 0;
        getline(fin, line);
       
        stringstream ss(line);
     
        while (ss.good()) {

           

            getline(ss, line, ',');
            if (i == 0) {
            
                m.second=line; 
            }
            
            else {
                m.first = line;
                k++;
                l++;
            }
            i++;
            if (k == 2)
            {
                keywords.insert(m);
              
                k = 1;
            }

             if(l==1)
                 keywords.insert(m);
                


        }
      
    }
    fin.close();
}
void Graph::final_results()
{
    auto iterator = score.begin();
    int j = 0;
    if (results.size() > 1)
    {
        for (int i = 0; i < s.size() - 1; i++)
        {
            if (s[i].second < s[i + 1].second)
                swap(s[i], s[i + 1]);
        }
        for (int i = 0; i < s.size(); i++)
        {
            cout << i + 1 << ".";
            cout << s[i].first << endl;
        }
    }
    else if (results.size() == 1)

        cout << j << "." << s[0].first<<endl;
}
void Graph::All(string word)
{

    search(word);
    pageRank();
    CTR();
    writeFiles();
    getscore();

}
int main()
{
    Graph g;
    Graph g2;
    string word;
    int input = 0;
    int input2 = 0;
    int input3 = 0;
    int input4 = 0;
    cout << "Welcome :)\nPlease Choose the number you want to proceed with:\n";
    cout << "1. New Search\n2. Exit\n";
    cin >> input;
    if (input == 2)
        return 0;
    else if(input==1) {
        cout << "Please enter the word you want to search for:\n";
        cin.ignore();
        getline(cin, word);
        g.All(word);
      
    }
    else {
        cout << "invalid input";
        return 0;
    }
    cout << "Would you like to\n 1.Choose a webpage to open\n 2. New Search\n 3.Exit\n";
    cin >> input2;
    if (input2 == 3)
    {
        return 0;
    }
    if (input2 == 2)
    {
        cout << "Please enter the word you want to search for:\n";
        cin.ignore();
        getline(cin, word);
        g.All(word);
       
    }
    else if(input2==1)
    {
        cout << "enter the number of the webpage you want:\n";
        cin >> input4;
        if (input4 == g.j)
        {
            g.CTR2(g.s[g.j-1].first);
        }
    }

    cout << "Would you like to\n 1.Go Back to the Search Results\n 2. New Search\n 3.Exit\n";
   
    cin >> input3;
    if (input3 == 3)
        return 0;
    else if(input3==2)
    {
        cout << "Please enter the word you want to search for:\n";
        cin.ignore();
        getline(cin, word);
        g2.All(word);
    
    }
    else if (input3 == 1)
    {
        g.final_results();
    }
    else {
        cout << "invalid input\n";
        return 0;
    }


}