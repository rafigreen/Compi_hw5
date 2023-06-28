#ifndef COMPI_HW3_TYPE_H
#define COMPI_HW3_TYPE_H

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::pair;

enum BranchLabelIndex {FIRST, SECOND};

typedef vector<std::pair<int, BranchLabelIndex>> BPList;

class Node {
public:
    string value;

    Node(const string value = "") : value(value) {
    };
    Node(const Node &node): value(node.value){
    };
    virtual ~Node() = default;
};

#define YYSTYPE Node*

class Funcs : public Node {
    virtual ~Funcs() = default;
};

class Label: public Node{
public:
    Label();
    virtual ~Label() = default;
};

class Type : public Node {
public:
    string type;

    Type(const string type);

    virtual ~Type() = default;
};

class RetType : public Node {
public:
    string type;

    RetType(const string type);

    virtual ~RetType() = default;
};

class FormalDecl : public Node {
public:
    string type;

    FormalDecl(Type *type, Node *node);
    FormalDecl(FormalDecl* formal);

    virtual ~FormalDecl(){
    };
};

class FormalsList : public Node {
public:
    vector<FormalDecl *> formals_list;

    FormalsList(Node *node);

    FormalsList(Node *node, FormalsList *formals_list);

    virtual ~FormalsList() {
        for (auto it = formals_list.begin(); it != formals_list.end(); it++)
            delete[] (*it);
    }
};

class Formals : public Node {
public:
    vector<FormalDecl *> formals_list;

    Formals();

    Formals(FormalsList *formals_list);

    virtual ~Formals() {
        for (auto it = formals_list.begin(); it != formals_list.end(); it++)
            delete[] (*it);
    }
};




class Exp : public Node {
public:
    string type;
    bool is_var=false;
    vector<string> overriden_types;
    string reg = "";
    BPList true_list = {};
    BPList false_list = {};
    BPList next_list = {};

    Exp() : Node(""),type("void") {}

    Exp(Node *terminal, string type);

    Exp(Node *terminal1, Node *terminal2, const string op, const string type, const string label);

    Exp(bool is_var, Node *terminal);

    Exp(Node *exp, Node *type);

    Exp(Exp *exp);

    virtual ~Exp() = default;
};

class OverrideDecl : public Node{
public:
    bool is_overriden;
    OverrideDecl(bool overriden);
       
    virtual ~OverrideDecl() = default;
};

class FuncDecl : public Node {
public:
    FuncDecl(RetType *return_type, Node *id, Formals *params, OverrideDecl *is_overriden);

    virtual ~FuncDecl() = default;
};

class ExpList : public Node {
public:
    vector<Exp*> expressions;
    ExpList(): expressions(){}
    ExpList(Node *exp);

    ExpList(Node *exp_list, Node *exp);

    virtual ~ExpList() {
        for (auto it = expressions.begin(); it != expressions.end(); it++)
            delete[] &(*it);
    }
};


class Call : public Node {
public:
    string type;
    string reg = "";
    BPList true_list = {};
    BPList false_list = {};
    BPList next_list = {};
    Call(Node *terminal);

    Call(Node *terminal, Node *exp_list);

    virtual ~Call() = default;
};

class Statement : public Node {
public:
    BPList cont_list = {};
    BPList break_list = {};

    Statement(): Node(){}
    Statement(Node *node);

    Statement(Type *type, Node *id);

    Statement(Type *type, Node *id, Exp *exp);

    Statement(Node *id, Exp *exp);

    Statement(Statement* statement, Exp *exp, Label* label);
    Statement(const string& name, Exp *exp, Label* exp_label, Label* true_label, Statement* statement);
    Statement(Statement* statement1, Statement* statement2, Exp *exp, Label* true_label, Label* false_label);

    Statement(Exp *exp, bool is_return=false);

    Statement(Call *call);
    void merge_lists_statements(Node* node);
    virtual ~Statement() = default;
};

class Statements : public Node {
public:
    BPList cont_list;
    BPList break_list;
    Statements(Statement *statement);

    Statements(Statements *statements, Statement *statement);

    virtual ~Statements() = default;
};

class Program : public Node {
public:
    Program();

    virtual ~Program() = default;
};

void check_bool(Node* node);

#endif
