#include <stdexcept>
#include <string>
using std::string;
#include <iostream>
#include <string>
using std::ostream;

#ifndef _VALUE_H_
#define _VALUE_H_

class Value {
public:
    enum ValType { ERRVAL, INTVAL, STRVAL };
    
private:
    ValType        theType;
    int                 theInt;
    string            theStr;
    
public:
    Value() : theType(ERRVAL), theInt(0) {}
    Value(int i) : theType(INTVAL), theInt(i) {}
    Value(string s) : theType(STRVAL), theInt(0) , theStr(s) {}
    
    bool isInt() const { return theType == INTVAL; }
    bool isStr() const { return theType == STRVAL; }
    bool isErr() const { return theType == ERRVAL; }
    
    int GetInt() const {
        if( isInt() ) return theInt;
        throw std::logic_error("Conditional is not an integer");
    }
    
    string GetStr() const {
        if( isStr() ) return theStr;
        throw std::logic_error("This Value is not a string");
    }
    
    Value operator+(const Value& o) const {
        if(o.isInt() && isInt()) {return Value(o.GetInt()+GetInt());}
        
        else if(o.isStr() && isStr()) {
            return Value(GetStr() + o.GetStr());
        } 
        else{
           throw std::logic_error("Type mismatch for arguments of +"); 
        }
        return Value();
    }
    Value operator-(const Value& o) const {
         if(o.isInt() && isInt()){
        return Value(GetInt() - o.GetInt());
        }
        else{
         throw std::logic_error("Type mismatch for arguments of -");   
        }
        return Value();
    }
    Value operator*(const Value& o) const{
        if(isInt() && o.isInt()){ 
             return(Value(GetInt()*o.GetInt()));   
        }
       else if(isStr() && o.isInt()){
            if(o.GetInt() >= 0){
              int tval = o.GetInt();
                string temp;
                for (int i = 0; i < tval; i++){
                    temp+=GetStr();
                    
                }
                return(Value(temp));
            }
            else{
                throw std::logic_error("Repetition count less than 0");
            }
       }
       else if(isInt() && o.isStr()){
           if(GetInt() >= 0){
               int nval = GetInt();
               string newtemp;
               for (int j = 0; j<nval;j++){
                   newtemp+= o.GetStr();
               }
               return(Value(newtemp));
           }
            else{
                throw std::logic_error("Repetition count less than 0");
            }
       
       }
         else{
             throw std::logic_error("Type mismatch for arguments of *");
         }
        return Value();
        }
   // }
    Value operator/(const Value& o) const{   
        if(o.isInt() && isInt()){
            if(o.isInt() == 0){
           //cout << GetInt() << "/" << o.GetInt() << endl;
             throw std::logic_error("Divide by zero error");
            return Value(o.GetInt());
            }
            return(Value(GetInt()/o.GetInt()));
        }
        else{
          throw std::logic_error("Type mismatch for arguement of /");
        }
      return Value();
    }
    
    friend ostream& operator<<(ostream& out, const Value& v) {
        switch( v.theType ) {
            case ERRVAL:
                out << "RUNTIME ERROR: " << v.theStr;
                break;
            case INTVAL:
                out << v.theInt;
                break;
            case STRVAL:
                out << v.theStr;
                break;
        }
        
        return out;
    }
};

#endif
