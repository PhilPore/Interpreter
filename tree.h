/*
 * parsetree.h
 */

#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <map>
#include <string>
#include "value.h"
//using namespace std;

using std::vector;
using std::map;
using std::string;
// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE };
static map<string, Value> symb;
// a "forward declaration" for a class to hold values
class Value;

class ParseTree {
	int		linenum;
	ParseTree	*left;
	ParseTree	*right;
//protected:
     //our symbol table
public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree() {
		delete left;
		delete right;
	}

	int GetLinenum() const { return linenum; }
	ParseTree *GetLeft() const { return left; }
	ParseTree *GetRight() const { return right; }

	int NodeCount() const {
		int nc = 0;
		if( left ) nc += left->NodeCount();
		if( right ) nc += right->NodeCount();
		return nc + 1;
	}
    int IntrCount() const {
		int lc = 0;
		if( left ){ lc+=left->IntrCount();}
		if( right ) {
            lc+=right->IntrCount();
        }
        if(left == 0 && right == 0){
            lc++;
        }
		return lc;
	}
    int DepthCount() const {
        int lft=0;
        int rght = 0;
        if( left ) {
            lft+=left->DepthCount();
            
        }
        if ( right){
         rght+= right -> DepthCount();
            
        }
        if (rght >= lft)
            return rght+1;
        else
            return lft+1;
    }
    int Strcount() const{
       int strcount = 0; 
       if( left ) {
            strcount+=left->Strcount();
            
        }
        if ( right){ 
        strcount+=right->Strcount();
        }
        if(IsString()){
         strcount++;   
        }
        return strcount;
    }
    int OPcount() const{
      int opcnt = 0;
      if (left){opcnt+=left->OPcount();}
       if ( right){ 
        opcnt+=right->OPcount();
        }
        if(IsOp()){
         opcnt++;   
        }
        return opcnt;
    }
	virtual bool IsString() const { return false; }
    virtual bool IsOp() const { return false; }
    virtual bool IsIConst() const { return false; }
    virtual Value Eval() const = 0;
};

class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    Value Eval() const{
    /* if(l){
      GetLeft()->Eval();}
     if(r){
      GetRight()->Eval();
     }*/
      Value lchild = GetLeft()->Eval();
      if(GetRight())
      { //or just getright since its an object
          Value rchild = GetRight()->Eval();
      }
      return Value();  //might need to do the rchild or lchild
    }

};


class IfStatement : public ParseTree {
public:
	IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
    //eval will check to see if the thing inside of it is number based and then follow through with it
    Value Eval() const{
     Value rchi = GetLeft() -> Eval();
      if(rchi.GetInt()!=0)
      {
           return GetRight()-> Eval();   
      }
     return Value(0);   
    }
};


class SetStatement : public ParseTree {
	string id;

public:
	SetStatement(int line, string id, ParseTree *expr) : ParseTree(line, expr), id(id) {}
    Value Eval() const
    {
     Value lval = GetLeft()->Eval();   
     symb[id] = lval;  
     return lval;
    }
};

class PrintStatement : public ParseTree {
public:
	PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}
    Value Eval() const{
        // try{
            Value exp = GetLeft()->Eval();
      // if(exp.isInt() || exp.isStr())
     //  {
        
            cout<<exp;
           
      //  }
        return Value(exp);
    }
};


class LoopStatement : public ParseTree {
public:
	LoopStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
    Value Eval() const {
     while(GetLeft()-> Eval().GetInt() != 0){
       GetRight()->Eval();   
     }
       return Value(0); 
    }
};


class Addition : public ParseTree {
public:
	Addition(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
         virtual bool IsOp() const { return true; };
        Value Eval() const{
         auto l1 = GetLeft() -> Eval();
         auto r1 = GetRight() -> Eval();
            return( l1 + r1);
        }
};


class Subtraction : public ParseTree {
public:
	Subtraction(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
     virtual bool IsOp() const { return true; };
    //eval needs an operator overload
    Value Eval() const{
         auto l1 = GetLeft() -> Eval();
         auto r1 = GetRight() -> Eval();
            return (l1 - r1);
        }

};


class Multiplication : public ParseTree {
public:
	Multiplication(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
         virtual bool IsOp() const { return true; };
    //eval needs an operator overload
    Value Eval() const{
         auto l1 = GetLeft() -> Eval();
         auto r1 = GetRight() -> Eval();
            return (l1 * r1);
        }

};


class Division : public ParseTree {
public:
	Division(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
     virtual bool IsOp() const { return true; };
    //eval needs an operator overload
    Value Eval() const{
         auto l1 = GetLeft() -> Eval();
         auto r1 = GetRight() -> Eval();
        if(r1.GetInt() == 0){
            throw std::logic_error("Divide by zero error");
        }
        //cout << l1.GetInt() << "/" << r1.GetInt() << endl;
            return (l1 / r1);
        }
};


class IConst : public ParseTree {
	int val;

public:
	IConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
       
	}
    Value Eval()const{
    return(Value(val));
    }
     bool IsIConst() const { return true;};
};


class SConst : public ParseTree {
	string val;

public:
	SConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}
    Value Eval()const{
    return Value(val);
    }

	bool IsString() const { return true; }
};


class Ident : public ParseTree {
	string id;

public:
	Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}
    Value Eval()const{
    if(symb.find(id) != symb.end()){
        return symb[id];
    }
    else{
    throw std::logic_error(("Symbol "+id+" not defined"));
    }
    }
	bool IsIdent() const { return true; }
};

#endif /* TREE_H_ */