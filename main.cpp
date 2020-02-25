#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include "parse.h"
#include "gettoken.h"
#include "tokens.h"
#include "tree.h"
#include "value.h"


using namespace std;

int main(int argc, char *argv[]){
   ifstream inp;
   istream* readr;
   int linum = 0;
    if(argc ==1){
       readr = &cin;
    }
    else if(argc == 2){
        inp.open(argv[1]);
        if (inp.is_open() == false){
            cerr << "COULD NOT OPEN " << argv[1] << endl;
        return -1;}
        readr = &inp;
    }
    else{
     cerr << "TOO MANY FILENAMES"  <<endl;
     return -1;
    }
    ParseTree *strt = Prog(*readr, linum); 
    if (strt == 0){
     return -1;  
    }
    try {
        strt->Eval();
        }
    catch( std::logic_error& e ) {
    cout << "RUNTIME ERROR " << e.what() << endl;
    } 
    
    //ryan i know you cant read this but i think youre a genuinely cool dude. love you
     
    
   
    

return 0;
}