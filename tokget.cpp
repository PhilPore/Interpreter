#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <vector>
#include <map>
#include "tokens.h"

using namespace std;
Token getNextToken(istream& in, int& linenum){  
    int lnm = linenum;
    istream* inch = &in;
    enum states{
     start,
        instring,
        incomment,
        inint,
        inID
    };
    char ch;
    string str;
     states check = start;
      
    
    while(inch->get(ch)){
        /*-----------starts at start state, and switches. each case will have a return value. Awesome stuff. use isspace my padawan */
         // cout << "this is a test" << endl;
      //  cout << "char: " << ch << endl;
        switch(check){
             
          case start:
                //logic here maybe to see ifi ts an operator before appending it ot a string. incase we watch something here
               // cout << "This is working." << endl;
               if (isspace(ch)){ 
                if (ch == '\n'){
                    linenum+=1;
                    lnm = linenum;
                  // cout << "This nl is working " << ch << " " << lnm <<  endl;
                    return Token(NL,"\n", lnm);
                    
                }
               }
                else if (ch == '+'){
                  return Token(PLUS, "+", lnm);   
                }
              else if (ch == '-'){
                    return Token(MINUS, "-", lnm);   
                }
               else if (ch == '*'){
                    return Token(STAR, "*", lnm);   
                }
               else if (ch == '/'){
                    return Token(SLASH, "/", lnm);   
                }
               else if (ch == '('){
                    return Token(LPAREN, "(", lnm);   
                }
               else if (ch == ')'){
                    return Token(RPAREN, ")", lnm);   
                }
               else if (ch == ';'){
                  return Token(SC,";", lnm);   
                }
               else if (isalpha(ch)){
                 str+=ch;
                 check = inID;   
                }
               else if (isdigit(ch)){
                 str+=ch;
                 check = inint;   
                }
                else if (ch == '\"'){
                  //str+=ch;
                  check = instring;}
                else if (ch =='#'){
                 str+=ch;
                    check = incomment;
                    
                }
                else{
                    str+=ch;
                  return Token(ERR, str, lnm);    
                }
                
              break;
          case instring: //"AAA 
                  if (ch == '\\'){
                  char newchar;
               if(inch->get(newchar)){
                   if (newchar == 'n')
                       str +='\n';
                   else 
                       str+=newchar;  
                       
               } 
                  
              }
                
               else if (ch == '\n'){
                   string anothererror = "\""+str+ch;
                   return Token(ERR, anothererror, lnm);
               }     
              
             else if (ch != '"'){
               str+=ch;   
              }
              else if (ch == '"'){
                  //string newstr = strcpy(str[1], str[str.length()-1]); string to get rid of the qoutation
               return Token(SCONST, str, lnm);   
              }
            
                          
              break;
                         
          case inint:
               if (isdigit(ch)){
                str+=ch;   
                   
               }
               else{
                if(isalpha(ch)){
                  string errorname = str+ch;   
                   return Token(ERR, errorname, lnm); 
                    
                }
                else{
                    inch->putback(ch); 
                    return Token(ICONST, str, lnm);
                }
               }
              break;
          case inID:
                    if (isalnum(ch)){
                      str+=ch;   
                    }
                    else{
                      inch->putback(ch);
                  /* if (ch == '\n'){
                         //lnm--;
                         inch->putback(ch);
                   } */
                     if (str == "loop")
                         return Token(LOOP, str, lnm);
                    
                    if (str == "print")
                         return Token(PRINT, str, lnm); 
                     
                    if (str == "set")
                         return Token(SET, str, lnm);
                     
                    if (str == "if")
                         return Token (IF, str, lnm);
                    
                    if (str == "begin")
                         return Token (BEGIN, str, lnm);
                    
                    if (str == "end")
                         return Token(END, str, lnm);
                     
                     //if(!isspace(ch))
                         //inch->putback(ch);
                     return Token(ID, str, lnm);  
                    }
              break;
          
                    case incomment:
                      if (ch == '\n'){
                          inch->putback(ch);
                          check = start;
                          continue;
                      }
                    break;
    
                      
                 }
         }
       if (check == instring){
           return Token(ERR, str, lnm);
           
       }
       else{
          return Token(DONE, "", lnm);   
                             
                             
           }
}          