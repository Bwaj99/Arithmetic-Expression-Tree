#include "arithmeticExpression.h"
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <stack>

using namespace std;

arithmeticExpression::arithmeticExpression(const string &value) : infixExpression(value), root(nullptr) {}

//arithmeticExpression::~arithmeticExpression() {}

void arithmeticExpression::buildTree() {
    infixExpression = infix_to_postfix();
    stack <TreeNode*> stack;
    long unsigned int cntr = 0; //used to keep track of position in postfix operation
    char keyChar = 'a';

    while (cntr < infixExpression.size())
    {
        TreeNode *addNode = new TreeNode(infixExpression.at(cntr), keyChar);
        if (priority(infixExpression.at(cntr)) > 0)
        {
            addNode->right = stack.top();
            stack.pop();
            addNode->left = stack.top(); //If the current postfix expression character is an operator, the new node is set as the parent of the two nodes at the top of the stack
            stack.pop();
            stack.push(addNode);
            root = addNode;
        }
        else if (priority(infixExpression.at(cntr)) == 0)
        {
            stack.push(addNode);
        }

        ++keyChar;
        ++cntr;
    }  //stack should only contain the root node once loop is finished
}

void arithmeticExpression::infix() {
    infix(root);
}
void arithmeticExpression::prefix() {
    prefix(root);
}
void arithmeticExpression::postfix() {
    postfix(root);
}
void arithmeticExpression::visualizeTree(ofstream &, TreeNode *node) {}

int arithmeticExpression::priority(char op){ //assigns a priority value to each operator 
    int priority = 0;
    if(op == '('){
        priority =  3;
    }
    else if(op == '*' || op == '/'){
        priority = 2;
    }
    else if(op == '+' || op == '-'){
        priority = 1;
    }
    return priority;
}

string arithmeticExpression::infix_to_postfix(){
    stack<char> s;
    ostringstream oss;
    char c;
    for(unsigned i = 0; i< infixExpression.size();++i){
        c = infixExpression.at(i);
        if(c == ' '){
            continue;
        }
        if(c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')'){ //c is an operator
            if( c == '('){
                s.push(c);
            }
            else if(c == ')'){
                while(s.top() != '('){
                    oss << s.top();
                    s.pop();
                }
                s.pop();
            }
            else{
                while(!s.empty() && priority(c) <= priority(s.top())){
                    if(s.top() == '('){
                        break;
                    }
                    oss << s.top();
                    s.pop();
                }
                s.push(c); //loop will continue as long as the stack is not empty and the priority of c is less than or equal to the priority of the top operator in the stack
            }
        }
        else{ //c is an operand
            oss << c;
        }
    }
    while(!s.empty()){
        oss << s.top();
        s.pop();
    }
    return oss.str(); //removes any remaining operators from the stack and append them to the output string stream
}

void arithmeticExpression::infix(TreeNode* root) { //recursively traverses the binary expression tree and prints out the arithmetic expression stored in the tree in infix notation
    if (root != nullptr)
    {
        if (root->left != nullptr)
        {
            cout << "(";
        }

        infix(root->left); // recursively calls infix() on the left child node of root
        cout << root->data;
        infix(root->right);

        if (root->left != nullptr)
        {
            cout << ")";
        }
    }
}
void arithmeticExpression::prefix(TreeNode *root) { ////recursively traverses the binary expression tree and prints out the arithmetic expression stored in the tree in prefix notation
    if (root != nullptr)
    {
        cout << root->data;
        prefix(root->left);
        prefix(root->right);

    }
}
void arithmeticExpression::postfix(TreeNode* root) { ////recursively traverses the binary expression tree and prints out the arithmetic expression stored in the tree in postfix notation
    if (root != nullptr)
    {
        postfix(root->left);
        postfix(root->right);
        cout << root->data;
    }
}
void arithmeticExpression::visualizeTree(const string &outputFilename){ //creates visualization of the tree
    ofstream outFS(outputFilename.c_str());
    if(!outFS.is_open()){
        cout<<"Error opening "<< outputFilename<<endl;
        return;
    }
    outFS<<"digraph G {"<<endl;
    visualizeTree(outFS,root);
    outFS<<"}";
    outFS.close();
    string jpgFilename = outputFilename.substr(0,outputFilename.size()-4)+".jpg";
    string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
    system(command.c_str()); //generates a visualization of the binary expression tree in the form of a JPG image file
}