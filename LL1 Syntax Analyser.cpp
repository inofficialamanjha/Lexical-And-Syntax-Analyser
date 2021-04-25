#include<bits/stdc++.h>
using namespace std;

class Cfg_rule // a CFG Grammer rule object
{
    public :

        char root; // Eg. S
        vector<char> leaf; // Eg. A, B, C

        // makes S -> A B C ( a grammar rule )

        Cfg_rule(char root = 'e')
        {
            this->root = root; // initalizes the root as Epsilon
        }

        int is_present(char l) // to search an element in the leaf of a particular CFG Rule
        {
            for(int i=0;i<leaf.size();i++)
            {
                if(l==leaf[i])
                return i; // returns the index if its present in the Cfg_rule
            }

            return -1; // if it does not exist
        }

        void display()
        {
            cout<<root<<" --> ";

            for(int i=0;i<leaf.size();i++)
            {
                cout<<leaf[i];
            }
        }

};


set<char> First_single( char f,map< char , list<Cfg_rule> > &rules, set<char> &terminals, set<char> &non_terminals) // returns a character vector in the first set
{
    set<char> first;

    if(terminals.count(f))
    first.insert(f);

    else
    {
        queue<char> BFS;
        BFS.push(f);

        while(!BFS.empty())
        {

            char d = BFS.front();
            BFS.pop();
            
            if( (d == 'e' || terminals.count(d)!=0) && first.count(d)==0 ) // if the character is an epsilon, or a terminal and not present already in the set
            first.insert(d);

            else
            {
            for(auto node_1 : rules[d]) // iterating the list on Cfg_rule object
            {
                // node_1 is a Cfg_rule type object

                BFS.push(node_1.leaf[0]);
    
            }
            }

        }

    }

    return first;

}


map<char, set<char>> First_all( map< char , list<Cfg_rule> > &rules, set<char> &terminals, set<char> &non_terminals)
{

    map<char, set<char>> first;

    // for terminals , first(terminal) = terminal

    for(auto node : terminals)
        // cout<<"FIRST ( "<<node<<" ) : { " <<node<<" }"<<endl; ## Make use only if printing is necessary
        first[node].insert(node);

    
    // for non-terminals we need to find the first

    for( auto node : non_terminals)
    {   
        /*

        cout << "FIRST ( " << node << " ) : { ";

        set<char> first; // to store the first characters

        */

        queue<char> BFS; // applying a BFS-Search on the graph


        BFS.push(node);

        while(!BFS.empty())
        {

            char d = BFS.front();
            BFS.pop();
            
            if( (d == 'e' || terminals.count(d)!=0) && first[node].count(d)==0 ) // if the character is an epsilon, or a terminal and not present already in the set
            first[node].insert(d);

            else
            {
                for(auto node_1 : rules[d]) // iterating the list on Cfg_rule object
                {
                    // node_1 is a Cfg_rule type object

                    BFS.push(node_1.leaf[0]);
        
                }
            }

        }
        
        /*

        for(auto f : first)
        cout<<f<<", ";

        cout<<" }"<<endl;

        */
    }

    return first;
}


void Follow_single(char f, map< char, set<char> > &Follow, map< char , list<Cfg_rule> > &rules, set<char> &terminals, set<char> &non_terminals )
{

    for( auto node : rules ) // node is a map iterator with 'key : non-terminals' and 'value : list of Cfg_rule objects'
    {
        for( auto node1 : node.second ) // iterating the list of Cfg_rule objects
        {
            if(node1.is_present(f)!=-1) // if the non-terminal is present
            {
                int pos = node1.is_present(f); // acquired the position in the cfg_rule

                if( (pos+1) >= node1.leaf.size() )
                {
                    // of the form A --> αB

                    if(node1.root != f)
                    {
                        if(Follow.count(node1.root)==0)
                            Follow_single(node1.root, Follow, rules, terminals, non_terminals);

                        for(auto node2 : Follow[node1.root])
                            Follow[f].insert(node2);
                    }

                }
                else 
                {
                   // of the form A --> αBβ ; Now calculating first of β

                    bool only_epsilon = true;

                    for(auto node2 : First_single(node1.leaf[pos+1],rules,terminals,non_terminals))
                       if(node2 != 'e')

                    only_epsilon = false;

                    
                    if(only_epsilon == true)
                    {

                        // if only_epsilon == True --> means, the first of (β) contains only ε, i.e the grammar is of the form A --> αBβ where first of 'β' is 'e' or 'ε' or Epsilon

                        if(node1.root != f)
                        {
                            if(Follow.count(node1.root)==0)
                                Follow_single(node1.root, Follow, rules, terminals, non_terminals);

                            for(auto node2 : Follow[node1.root])
                                Follow[f].insert(node2);

                        }

                    }

                    else
                    {
                        // if only_epsilon == False --> means, the first of (β) contains more than only ε, i.e the grammar is of the form of the form A --> αBβ

                        for(auto node2 : First_single(node1.leaf[pos+1],rules,terminals,non_terminals))
                        {
                            if(node2 != 'e')
                            Follow[f].insert(node2);
                            
                        }
                    }

                }

            }
        }
    }


}


map<char, set<char>> Follow_all(char start_node,map< char , list<Cfg_rule> > &rules, set<char> &terminals, set<char> &non_terminals )
{
    map< char, set<char> > Follow; // the Follow map

    Follow[start_node].insert('$'); // Inserting '$' for the start node

    Follow_single(start_node,Follow,rules,terminals,non_terminals);

    for(auto node : non_terminals)
    {
        if(Follow.count(node)==0)
            Follow_single(node,Follow,rules,terminals,non_terminals);
    }

    /*

    ## Make the return of the fuction void and remove all the lines below this comment to see previous version of Applicaton

    for(auto node : Follow) // Printing all the Follow's
    {
        cout<<"FOLLOW ( " << node.first <<" ) : { ";

        for(auto node1 : node.second)
        {
            cout<<node1<<", ";
        }

        cout<<"}"<<endl;
    }

    */

   return Follow;
}


map<char, map<char,Cfg_rule>> LL1_Parsing_Table_Construct(map< char , list<Cfg_rule> > &rules, map<char, set<char>> &First, map<char, set<char>> &Follow, set<char> &non_terminals, set<char> &terminals)
{
    // Columns : Consists of all terminals and '$'

    // Rows : Consists of all non-terminals

    // No element will be present for error parser

    map<char, map<char, Cfg_rule>> LL1_Parser_Table;

    for( auto rule : rules )
    {
        for( auto rule1 : rule.second) // traversing all the rules
        {
            // Consider Rule/Production of the form A --> α

            char alpha = rule1.leaf[0];

            if(alpha != 'e') // alpha is not equal to 'Epsilon' or 'e'
            {

                for(auto node : First[alpha])
                {
                    if(node != 'e')
                    {
                        // STEP 1 : Add A --> α to M[A,a] where a belongs to FIRST(α)

                        LL1_Parser_Table[rule1.root][node] = rule1;
                    }
                    else
                    {
                        for(auto node1 : Follow[rule1.root] )
                        {
                            // STEP 2 : Add A --> α to M[A,b] where b belongs to FOLLOW(A)

                            LL1_Parser_Table[rule1.root][node1] = rule1;
                        }
                    }
                }

            }

            else
            {
                for(auto node1 : Follow[rule1.root] )
                {
                    // STEP 2 : Add A --> α to M[A,b] where b belongs to FOLLOW(A)

                    LL1_Parser_Table[rule1.root][node1] = rule1;
                }
            }
        }

    }

    return LL1_Parser_Table;
}


void Display_Parsing_Table_Construct(map<char, map<char,Cfg_rule>> &LL1_Parsing_Table)
{
    for(auto node : LL1_Parsing_Table)
    {
        for( auto node1 : node.second)
        {
            cout<<"M [ "<<node.first<<" , "<<node1.first<<" ] : ";
            node1.second.display();
            cout<<"\t\t\t,\t\t\t";
        }

        cout<<endl;
    }
}


bool LL1_Parse(string test, char start_node, map< char, map< char, Cfg_rule > > LL1_Parser)
{
    stack<char> S;
    S.push('$');S.push(start_node);

    int pos = 0; // pointer for test string

    test[test.size()] = '$';

    while(S.top()!='$')
    {
        if(S.top()==test[pos]) // if a Match is found
        {
            cout<<"Match for "<< test[pos];
            pos++;
            S.pop();
        }

        else
        {
            if(LL1_Parser.count(S.top()))
            {
                if(LL1_Parser[S.top()].count(test[pos]))
                {
                    // Check M [ S.top() , test[pos] ] and and enter the value into the stack
                    cout<<"Check cell M [ " << S.top() <<" , "<<test[pos]<<" ] and replace ";
                    Cfg_rule rule = LL1_Parser[S.top()][test[pos]];
                    S.pop();

                    
                    rule.display();


                    for(int i = rule.leaf.size()-1;i>=0;--i)
                    {
                        if(rule.leaf[i]!='e')
                            S.push(rule.leaf[i]);                     
                    }
                }
                else
                {
                    cout<<"No cell found for M [ "<<S.top()<<" , "<<test[pos]<<" ] and Hence returning Erorr !!";
                    break; // No match found
                }
            }
            else
            {
                cout<<"No cell found for M [ "<<S.top()<<" , "<<test[pos]<<" ] and Hence returning Erorr !!";
                break; // No match found
            }
        }

        cout<<endl;
    }

    // if the Stack has '$' and the test string has ended with '\0' the string is accepted
    if(S.top()=='$' && test[pos]=='$')
        return true;
    
    return false;
        
}


int main()
{
    cout<<"Enter number of test cases : ";
    int t; // test cases
    cin>>t;
    
    while(t--) // starts from t-1 to 0 ( t times )
    {

        cout << endl << "Enter the number of grammar rules : ";
        int nr ; // nr : number of grammer rules

        cin>>nr;

        map< char , list<Cfg_rule> > rules; // a map to store all rules ; key : a non-terminal character ; Value : a list of Cfg_rule objects

        set<char> terminals ; // a vector to store terminals

        set<char> non_terminals ; // a vector to store non-terminals

        cout<< endl << "\"Note : '$' is reserved; 'e' is reserved as Epsilon; 'i' is reserved for 'id'; 'n' is reserved for 'num'; 'x' is reserved for '*'\"\n\nStart Entering the Grammar Rules one-by-one in fromat \"S ABC\" which stands for \"S -> ABC\" :" << endl ;

        for(int i=0;i<nr;i++)
        {

            Cfg_rule node ; // Creating the rule object

            cin>>node.root; // Input the root Non-Terminal Node

            if(non_terminals.count(node.root)==0)
            non_terminals.insert(node.root); // assuming 'S' stands for stard and 'ϵ' stands for epsilon

            string leafs; // Input the leaf nodes
            cin>>leafs;

            for(int i=0;leafs[i]!='\0';i++)
            {
                if(leafs[i]=='e')
                {
                    // do nothing
                }
                
                else if(isupper(leafs[i]))
                {
                    if(non_terminals.count(leafs[i])==0)
                    non_terminals.insert(leafs[i]);
                }

                else if(islower(leafs[i]))
                {
                    if(terminals.count(leafs[i])==0)
                    terminals.insert(leafs[i]);
                }

                node.leaf.push_back(leafs[i]);
            }

            // made the node object

            // now enter the object into the map

            rules[node.root].push_back(node);

        }

        cout<<endl<<"Enter the Starting Node : ";
        char start_node;cin>>start_node; // start_node is the start for the grammar
        cout<<endl;


        map< char, set<char> > First = First_all( rules, terminals, non_terminals); // Calculates all the First's

        map< char, set<char> > Follow = Follow_all(start_node, rules, terminals,non_terminals ); // Calculates all the Follow's

        map< char, map< char, Cfg_rule > > LL1_Parsing_Table = LL1_Parsing_Table_Construct(rules,First,Follow,non_terminals,terminals); // A Double map to store the parsing table

        Display_Parsing_Table_Construct(LL1_Parsing_Table);

        fflush(stdin);
        string to_parse;
        cout<<endl<<"Enter the Test statement to pass through the LL1 Parser : ";
        cin>>to_parse;
        
        cout<<endl;
        if(LL1_Parse(to_parse, start_node, LL1_Parsing_Table))
            cout<<endl<<endl<<"String Accepted : No Syntax Error"<<endl;
        else
            cout<<endl<<endl<<"String Un-accepted : Syntax error is Present"<<endl;

    }

    cout<<endl<<endl;
    return 0;
}