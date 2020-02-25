#include "parse.h"
#include "gettoken.h"
#include "tokens.h"
#include <typeinfo>
using namespace std;

bool debug = false;
static int error_count = 0;

void

ParseError(int line, string msg)
{
	++error_count;
	cout << line+1 << ": " << msg << endl;
}

// Prog is an Slist
ParseTree *Prog(istream& in, int& line)
{
	ParseTree *sl = Slist(in, line);
    
	if( GetToken::Get(in, line) != DONE ){
		ParseError(line, "Unrecognized statement");
        }
	if( sl == 0 ){
		ParseError(line, "No statements in program");
        }
	if( error_count )
		return 0;

	return sl;
}

// Ssep { Slist } | Stmt Ssep { Slist }
ParseTree *Slist(istream& in, int& line) {
	ParseTree *s = Stmt(in, line);
	if( s == 0 )
		return 0;

	Token t;
	if( (t = GetToken::Get(in, line)) != SC && t != NL ) {
		ParseError(line, "Missing statement separator");
		return 0;
	}

    return new StmtList(s, Slist(in,line));
}

ParseTree *Stmt(istream& in, int& line) {
	ParseTree *s = 0;

	Token t = GetToken::Get(in, line);
    if(debug){
  cout << "Stmt t = " << t.GetTokenType() << " line:" << line << endl;}
	switch( t.GetTokenType() ) {
	case IF:
		s = IfStmt(in, line);
		break;

	case PRINT:
		s = PrintStmt(in, line);
		break;

	case SET:
		s = SetStmt(in, line);
		break;

	case LOOP:
		s = LoopStmt(in, line);
		break;

	case ERR:
		ParseError(line, "Invalid token");
        //rreturn 0;
		break;

	case DONE:
		break;

	case NL:
            if (debug){ cout << "New line found, line: " << line << endl;}
	case SC:
         if(debug){
              cout << "in SC t = ";
         if (t.GetTokenType() == NL)
             cout << "New Line" << endl;
         else
             cout << "SemiColon" << endl;
         }
         
       s = Stmt(in, line);
		break;

	default:
		GetToken::PushBack(t);
		break;
	}

	return s;
}

ParseTree *IfStmt(istream& in, int& line) {
	// add code to parse an if
    ParseTree* tempval = Expr(in, line);
    if (tempval == 0){
     ParseError(line, "Missing expression after if");
     return 0;
    }
    Token t = GetToken::Get(in, line);
    if (t.GetTokenType() != BEGIN){
        ParseError(line, "Missing BEGIN after expression");
        return 0;
    }
   
    ParseTree* someval = Slist(in, line);
    if (someval == 0){
     return 0;   
    }
    t = GetToken::Get(in, line);
   //cout << "If Stmt T: " << t.GetTokenType() << endl;
    if (t.GetTokenType() != END){
        
      ParseError(line, "Missing END"); 
      return 0;
    }
    return new IfStatement(line, tempval, someval);
    
}

ParseTree *LoopStmt(istream& in, int& line) {
	// add code to parse a loop
    ParseTree* t1 = Expr(in, line);
    if(t1 == 0){
      ParseError(line, "Missing expression after print");
      return 0;   
    }
    Token T = GetToken::Get(in, line);
    if (T.GetTokenType() != BEGIN){
      return 0;   
    }
    ParseTree* t2 = Slist(in, line);
    if (t2 == 0){
      ParseError(line, "Missing statement list after BEGIN");   
      return 0;
    }
    T = GetToken::Get(in, line);
    if (T != END){
        ParseError(line, "Missing END");
        return 0;
    }
    return new LoopStatement(line, t1, t2);
}

ParseTree *SetStmt(istream& in, int& line) {
	// add code to parse a set
    Token tok = GetToken::Get(in, line);
    if (tok.GetTokenType() != ID){
      ParseError(line, "Missing ID after set");  
      return 0;
    }
    ParseTree* a1 = Expr(in, line);
    if (a1 == 0){
     ParseError(line, "Missing expression after id");
      return 0;   
    }
    return new SetStatement(line, tok.GetLexeme(), a1);
}

ParseTree *PrintStmt(istream& in, int& line) {
    if (debug){ cout << "In print, line: " << line << endl;}
    ParseTree *t1 = Expr(in, line);
    if (t1 == 0){
        ParseError(line, "Missing expression after print");
      return 0;   
    }
    ParseTree* Printstmt = new PrintStatement(line, t1);
    return Printstmt;
}

ParseTree *Expr(istream& in, int& line) {
	ParseTree *t1 = Prod(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Token t = GetToken::Get(in, line);

		if( t != PLUS && t != MINUS ) {
			GetToken::PushBack(t);
           
			return t1;
		}

		ParseTree *t2 = Prod(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == PLUS )
			t1 = new Addition(t.GetLinenum(), t1, t2);
		else
			t1 = new Subtraction(t.GetLinenum(), t1, t2);
	}
}

ParseTree *Prod(istream& in, int& line) {
	ParseTree *t1 = Primary(in, line);

	// needs more...
    if (t1 == 0){
      return 0;   
    }
    while ( true ) {
		Token t = GetToken::Get(in, line);

		if( t != STAR && t != SLASH ) {
			GetToken::PushBack(t);
            
			return t1;
		}

		ParseTree *t2 = Primary(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == STAR )
			t1 = new Multiplication(t.GetLinenum(), t1, t2);
		else
			t1 = new Division(t.GetLinenum(), t1, t2);
	}
}


ParseTree *Primary(istream& in, int& line) {
	Token t = GetToken::Get(in, line);
    if (debug){ cout << "Primary, token: " << t.GetLexeme() <<" line found, line: " << line << endl;}
 // cout << "Tprim: " << t.GetTokenType() << endl;
	switch(t.GetTokenType()){
        case ID:
            {
            return new Ident(t);
            }
        case ICONST:
            {
            
            return new IConst(t);
            }
        case SCONST:
            {
            return new SConst(t);
            }
        case LPAREN:
            {
          //  cout << "Tprim =" << t.GetTokenType() << endl;
            ParseTree *t2 = Expr(in, line);
            if (t2 == 0){
                ParseError(line,"Missing expression after (");
                return 0;
            }
           
           Token tem = GetToken::Get(in, line);
            if (tem.GetTokenType() != RPAREN){
                ParseError(line, "Missing ) after expression");
                return 0;
            }
            return t2;
         }
            break;
          case SC:
            {
                ParseError(line, "Primary expected");
                return 0;
        }
            break; 
        case NL:{
         
       //line--;  
         if(debug){
          cout << "In Newline for Prim, line: " << line << endl;   
         }
        // GetToken::PushBack(t);
         //ParseError(line, "Primary expected");
         //return 0;   
        } 
       
        default:
            {
            ParseError(line, "Primary expected");
           GetToken::PushBack(t);
            
            return 0;
            }
            break;
           
    //cout << "Primary: " << t.GetTokenType() << endl;
   /* if (t.GetTokenType() == ID){
         return new Ident(t);

    }
    else if(t.GetTokenType()==ICONST){
            return new IConst(t);
    }
    else if(t.GetTokenType() == SCONST){
      return new SConst(t);   */
    }
    return 0;
}
