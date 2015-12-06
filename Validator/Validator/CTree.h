#ifndef _CTREE_H
#define _CTREE_H
#include "common.h"

struct CTreeNode{
  string name;
  vector<CTreeNode> children;
  double value;
  bool visited;

  CTreeNode(const char* _name):
    name(_name), value(), children(), visited(false){}

  CTreeNode& Step(string name){
      for (int i=0; i<this->children.size(); i++){
        if (!(this->children[i].visited) && name == this->children[i].name){
          this->children[i].visited = true;
          return this->children[i];
        }
      }
      throw invalid_argument( "Tree structure is bad" );
  }
};

struct CTree{
    CTreeNode root;
    double coef1;
    double coef2;
    double coef3;
    double coef4;
    CTree(string type):root(CTreeNode("root")){
        if (type=="quad"){
            root.children.push_back(CTreeNode("eq"));
            root.children[0].children.push_back(CTreeNode("plus"));
            root.children[0].children[0].children.push_back(CTreeNode("times"));
            root.children[0].children[0].children[0].children.push_back(CTreeNode("power"));
            root.children[0].children[0].children[0].children[0].children.push_back(CTreeNode("ci"));
            root.children[0].children[0].children[0].children[0].children.push_back(CTreeNode("cn"));
            root.children[0].children[0].children[0].children.push_back(CTreeNode("cn"));
            root.children[0].children[0].children.push_back(CTreeNode("times"));
            root.children[0].children[0].children[1].children.push_back(CTreeNode("ci"));
            root.children[0].children[0].children[1].children.push_back(CTreeNode("cn"));
            root.children[0].children[0].children.push_back(CTreeNode("cn"));
            root.children[0].children.push_back(CTreeNode(CTreeNode("cn")));
        }
        else if (type=="sum"){
            root.children.push_back(CTreeNode("sum"));
            root.children[0].children.push_back(CTreeNode("bvar"));
            root.children[0].children[0].children.push_back(CTreeNode("ci"));
            root.children[0].children.push_back(CTreeNode("lowlimit"));
            root.children[0].children[1].children.push_back(CTreeNode("cn"));
            root.children[0].children.push_back(CTreeNode("uplimit"));
            root.children[0].children[2].children.push_back(CTreeNode("cn"));
            root.children[0].children.push_back(CTreeNode("ci"));
        }
        else if (type=="prod"){
            root.children.push_back(CTreeNode("product"));
            root.children[0].children.push_back(CTreeNode("bvar"));
            root.children[0].children[0].children.push_back(CTreeNode("ci"));
            root.children[0].children.push_back(CTreeNode("lowlimit"));
            root.children[0].children[1].children.push_back(CTreeNode("cn"));
            root.children[0].children.push_back(CTreeNode("uplimit"));
            root.children[0].children[2].children.push_back(CTreeNode("cn"));
            root.children[0].children.push_back(CTreeNode("ci"));
        }
    }

    void setCoefQuad(){
        coef4 = root.children[0].children[1].value;
        coef3 = root.children[0].children[0].children[2].value;
        coef2 = root.children[0].children[0].children[1].children[1].value;
        coef1 = root.children[0].children[0].children[0].children[1].value;
        cout<<coef1<<" "<<coef2<<" "<<coef3<<" "<<coef4<<endl;
    }
    void setCoefLimits(){
        coef1 = root.children[0].children[1].children[0].value;
        coef2 = root.children[0].children[2].children[0].value;
        cout<<coef1<<" "<<coef2<<endl;
    }

};
#endif
