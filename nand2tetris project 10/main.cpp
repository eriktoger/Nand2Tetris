#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


/*
need to get directories to work
and correct name (now it is just test.xml)
my cod remove whitespaces in string constants

but all textcompares passed!

*/

void jackTokenizer( vector < pair <string,string> > &,string);

void compileEngine (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileEngine2 (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);

// the big ones
void compileClass (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileClassVarDec (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileSubroutineDec (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileParameterList (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileSubroutineBody (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileVarDec (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileStatements (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);

//  the statments

void compileLet (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileIf (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileWhile (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileDo (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileReturn (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);

// expressio, term and expressionList
void compileExpression (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileTerm (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);
void compileExpressionList (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);

//subrotuineCall
void compileSubroutineCall (const vector < pair<string,string> > &,int &, ofstream &,pair <int,int> &, pair <int,int> &,int &);

//other
void tabs(int, ofstream &);
void addMarkup(const string & ,string const &,int &,const int &,ofstream &file);

int main() {
  ifstream myfile;
  myfile.open ("new.jack");
  vector <char> symbol  {'{' , '}' , '(' , ')' , '[' ,']' ,'.', ',' , ';' , '+' , '-' ,'*' ,'/', '&' , '|' , '<' , '>' ,'=' ,'~'};

  vector < pair<string,string> > tokens;

  if(myfile.is_open()){
    string word;
    string eatLine;
    
    bool nestedComment = false;
    string partWord ="";
    while(myfile >> word ){
       
       if(partWord[0] =='"' && partWord[partWord.length() -1]!= '"' ){
                // dont reset word
                partWord+= " "; // the space is eaten up.
                // how do i handle multiple spaces?
                //right now, i dont
        }
        else{
          partWord ="";
        }

      // if word is bigger than 1 then we should check if its a comment. 
      //and if it is we should skip the line
      // if /* then we should skipp until we find */
      if(word.length() > 1 && word[0] == '/' && word[1] =='/'){
        //cout << "we found a  comment"<<endl;
        getline(myfile,eatLine);
      }
      else{
        // check if we have the beginning of a nested comment
        if(word.length()>1 && word[0] == '/' && word[1] == '*'){
          nestedComment = true;
        }
      if(!nestedComment){
        for(char c : word){
          if( find(symbol.begin(),symbol.end(),c)!=symbol.end() ){
            // we found a symbol;
            // this means that the current partWord (if not empty) should be tokenized
            // and the symbol
            if(partWord.length() > 0){
              //cout << partWord<<endl;
              jackTokenizer(tokens,partWord);
              
                partWord ="";
              
            }
            //cout << c <<endl;
            string cWord (1,c);
            jackTokenizer(tokens,cWord);
          }
          else{
            // no symbol
            // append to partword
            partWord += c;

          }

      }
      // if partWord is not empty it should be tokenized
        if(partWord.length() > 0){
          //cout << partWord<<endl;
          jackTokenizer(tokens,partWord);
        }
      }
      
      // check if we are exiting a nested comment
      if(word.length()>1 && word[word.length() -2] == '*' && word[word.length()-1] == '/'){
          nestedComment = false;
        }



    }
    }
  }
  myfile.close();

  for( pair<string,string> p : tokens){
    cout <<p.first<<" "<<p.second << endl;
  }

  ofstream outfile;
  string fileName = "test.xml";
  outfile.open ("test.xml");
  int counter = 0;
  int leftParenthesis = 0;
  int rightParenthesis = 0;
  int indentLevel = 0;
  pair <int,int> nrOfPara (0,0); 
  pair <int,int> nrOfCurl (0,0);

  for( pair<string,string> p : tokens){
    compileEngine(tokens,counter,outfile,nrOfPara, nrOfCurl, indentLevel);
    if(counter >= tokens.size()){
      break;
    }
    
  }
  cout <<"done!"<<endl;
}

void jackTokenizer(vector < pair<string,string> > &tokens, string word ){
pair <string,string> newToken;
newToken.first = word;

// check if word is a keyword,"static"
vector <string> keywords  {"class","constructor","function","method","field","static","var","int","char","boolean","void","true","false","null","this","let","do","if","else","while","return"};
if( find(keywords.begin(),keywords.end(),word)!=keywords.end() ){
  newToken.second ="keyword";
  tokens.push_back(newToken);
  return;

}

/*
if a word is more than 1 token.
a[i]; is 5 tokens
i think i want to split my words by my symbols (plus space)
*/
if (word.length() == 1 ){
  char charSymbol = word[0];
  vector <char> symbol  {'{' , '}' , '(' , ')' , '[' ,']' ,'.', ',' , ';' , '+' , '-' ,'*' ,'/', '&' , '|' , '<' , '>' ,'=' ,'~'};
  if( find(symbol.begin(),symbol.end(),charSymbol)!=symbol.end() ){
    newToken.second ="symbol";
    tokens.push_back(newToken);
    return;
  }
}

bool isDigit = true;
for (char c: word){
  if( !isdigit(c) ){
    isDigit = false;
    break;
  }
}

if(isDigit){
  newToken.second ="integerConstant";
  tokens.push_back(newToken);
  return;
}

//stringConstant no spaces
if(word.length() > 1 && word[0] == '"' && word[word.length() -1 ]== '"' ){
  // hacking of the " "
  newToken.first=newToken.first.substr(1,newToken.first.size()-2);
  newToken.second = "stringConstant";
  tokens.push_back(newToken);
  return;
}
// what if stringContats contains spaces?
if(word.length() > 1 && word[0] == '"' && word[word.length() -1 ]!= '"' ){
  //newToken.second = "StringConstant";
  //tokens.push_back(newToken);
  return;
}

// what if stringContats contains spaces?
if(word.length() > 1 && word[0] != '"' && word[word.length() -1 ]== '"' ){
  //newToken.second = "StringConstant";
  //tokens.push_back(newToken);
  return;
}




newToken.second = "identifier";
tokens.push_back(newToken);
}




void compileEngine (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){

  if(tokens[counter].first == "class"){
    compileClass(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);
  }

}

void compileClass (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){ 

/*
class
className
'{'
classVarDec*
subrutineDec*
'}'
*/


  tabs(indentLevel,file);
  file<<"<class>"<<endl;
  indentLevel++;

  // adding class
  if(tokens[counter].second !="keyword" ){
    cout << "Error in compileClass, not keyword"<<endl;
  }
  addMarkup(tokens[counter].first, tokens[counter].second, counter, indentLevel, file);

  // check if there is a main?
  if(tokens[counter].second !="identifier" ){
    cout << "Error in compileClass, not identifier"<<endl;
  }
  
  // add name of class
  addMarkup(tokens[counter].first, tokens[counter].second, counter, indentLevel, file);

  // add {
  if(tokens[counter].first !="{"){
    cout << "Error in compileClass, not {"<<endl;
  }

  addMarkup(tokens[counter].first, tokens[counter].second, counter, indentLevel, file);

  // classVarDec*
  while(tokens[counter].first=="static" || tokens[counter].first=="field"){

    compileClassVarDec(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  }
  

  // subrutineDec*
  while(tokens[counter].first=="constructor" || tokens[counter].first=="function" || tokens[counter].first=="method"){

    compileSubroutineDec(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  }

   // add }
  if(tokens[counter].first !="}"){
    cout << "Error in compileClass, not {"<<endl;
  }

  addMarkup(tokens[counter].first, tokens[counter].second, counter, indentLevel, file);


  // end of class
  indentLevel--;
  tabs(indentLevel,file);
  file<<"</class>";

  // we dont want an empty line at the end
  if(counter != (tokens.size()) ){
    file<<endl;
  }
  

}

void compileClassVarDec (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){ 

tabs(indentLevel,file);
file << "<classVarDec>"<<endl;
indentLevel++;

// add static or field
addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

//add type
// if I want to check this one i need the save class names.
addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

// add varName
addMarkup( tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

//possible more varNames
while(tokens[counter].first == ","){
  // add ,
  addMarkup( tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  
  // add varname
  addMarkup( tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
}

// add ;
if(tokens[counter].first !=";" ){
   cout << "Error in compileClassVarDec, not ;"<<endl;
}

addMarkup( tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

indentLevel--;
tabs(indentLevel,file);
file << "</classVarDec>"<<endl;
}

void compileSubroutineDec (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){ 

  tabs(indentLevel,file);
  file << "<subroutineDec>"<<endl;
  indentLevel++;

  // add constructor, function or method
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add void or type
  // add an if? need classname
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add subrotinName
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add (
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  
  // add parameterList
  compileParameterList(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  // add )
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add subrotuineBody
  compileSubroutineBody(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</subroutineDec>"<<endl;
  

}

void compileParameterList (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){ 
//( (type varname) (, type varName)* ) ?
  
  tabs(indentLevel,file);
  file << "<parameterList>" << endl;
  indentLevel++;


  if(tokens[counter].first != ")"){

    // add type
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add varname
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);


  }

  int counterParameterList = 0;
  while(tokens[counter].first != ")" ){

    //add ,
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add type
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add varName
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    counterParameterList++;
    if(counterParameterList > 200){
      cout << "break counterParameterList"<<endl;
      break;
    }


  }

  indentLevel--;
  tabs(indentLevel,file);
  file << "</parameterList>" << endl;
}

void compileSubroutineBody (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){
  // {
  // varDec*
  // statments
  // }

  tabs(indentLevel,file);
  file << "<subroutineBody>" << endl;
  indentLevel++;

  // addin {
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // vardec
  while(tokens[counter].first == "var"){
    compileVarDec(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);
  }
  

  // statments
  compileStatements(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  // addin }
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</subroutineBody>" << endl;

}

void compileVarDec (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){

  tabs(indentLevel,file);
  file << "<varDec>" << endl;
  indentLevel++;
  
  // add var
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add type
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  //add varName
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  if(tokens[counter].first != ";"){

    // add ,
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
    // add varName
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  }

  // add ;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</varDec>" << endl;
}

void compileStatements (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){

  tabs(indentLevel,file);
  file << "<statements>" << endl;
  indentLevel++;

  int counterStatements = 0;
  while(tokens[counter].first =="let" || tokens[counter].first =="if" ||
        tokens[counter].first =="while" || tokens[counter].first =="do" ||
        tokens[counter].first =="return"){

          if(counterStatements > 100){
            cout <<"counterStatements"<<endl;
            break;
          }
          counterStatements++;

          if(tokens[counter].first =="let" ){
            compileLet(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);
            continue;
          }

          if(tokens[counter].first =="if" ){
            compileIf(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);
            continue;
          }

          if(tokens[counter].first =="while" ){
            compileWhile(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);
            continue;
          }

          if(tokens[counter].first =="do" ){
            compileDo(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);
            continue;
          }

          if(tokens[counter].first =="return" ){
            compileReturn(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);
            continue;
          }

        }


  indentLevel--;
  tabs(indentLevel,file);
  file << "</statements>" << endl;

}

void compileLet (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){
  
  tabs(indentLevel,file);
  file << "<letStatement>" << endl;
  indentLevel++;

  // add let
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add varName
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // we have an index
  if(tokens[counter].first == "["){
    // add [
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add expression
    compileExpression(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

    // add ]
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  }

  //add =
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add expression
  compileExpression(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  // add ;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

 
  indentLevel--;
  tabs(indentLevel,file);
  file << "</letStatement>" << endl;

}

void compileIf (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){

  tabs(indentLevel,file);
  file << "<ifStatement>" << endl;
  indentLevel++;

  // add if
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add (
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add expression
  compileExpression(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  //add )
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add {
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add statements
  compileStatements(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  // add }
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  if(tokens[counter].first == "else"){
  
    //add else
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add {
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add statements
    compileStatements(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

    // add }
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);


  }

  indentLevel--;
  tabs(indentLevel,file);
  file << "</ifStatement>" << endl;

}

void compileWhile (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){
  
  tabs(indentLevel,file);
  file << "<whileStatement>" << endl;
  indentLevel++;

  //add while
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

   // add (
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

   // add expression
  compileExpression(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  //add )
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add {
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add statements
  compileStatements(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  // add }
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</whileStatement>" << endl;

}

void compileDo (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){
  tabs(indentLevel,file);
  file << "<doStatement>" << endl;
  indentLevel++;

  //add do
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add subroutineCall
  compileSubroutineCall(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  // add ;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</doStatement>" << endl;

}

void compileReturn (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){

  tabs(indentLevel,file);
  file << "<returnStatement>" << endl;
  indentLevel++;
  //add return
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  if(tokens[counter].first !=";"){
    // add expression
    compileExpression(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  }

  //add ;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</returnStatement>" << endl;

}


void compileExpression (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){

  tabs(indentLevel,file);
  file << "<expression>" << endl;
  indentLevel++;

  //  expression always starts with term
  compileTerm(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  while(tokens[counter].first == "+" || tokens[counter].first == "-"  ||
        tokens[counter].first == "*" || tokens[counter].first == "/" ||
        tokens[counter].first == "&" || tokens[counter].first == "|" ||
        tokens[counter].first == "<" || tokens[counter].first == ">"|| 
        tokens[counter].first == "="){

          if(tokens[counter].first == "&" ){
            // add &amp
            addMarkup("&amp;", tokens[counter].second,counter, indentLevel, file);

          } else if(tokens[counter].first == "<" ){
            // add &lt
            addMarkup("&lt;", tokens[counter].second,counter, indentLevel, file);

          } else if(tokens[counter].first == ">" ){
            // add &gt
            addMarkup("&gt;", tokens[counter].second,counter, indentLevel, file);

          } else {
            // all others
            // add op
            addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
          }

          // add term after op
         compileTerm(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

        }

  indentLevel--;
  tabs(indentLevel,file);
  file << "</expression>" << endl;

}

void compileTerm (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){

  tabs(indentLevel,file);
  file << "<term>" << endl;
  indentLevel++;

//intergerConstant
if(tokens[counter].second =="integerConstant" ){
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  
  indentLevel--;
  tabs(indentLevel,file);
  file << "</term>" << endl;
  return;
}

// stringConstant
if(tokens[counter].second =="stringConstant" ){
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
 
  indentLevel--;
  tabs(indentLevel,file);
  file << "</term>" << endl;
  return;
}

// keywordConstants
if(tokens[counter].first =="true" ||tokens[counter].first =="false" ||
   tokens[counter].first =="this" || tokens[counter].first =="null"){
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  
  indentLevel--;
  tabs(indentLevel,file);
  file << "</term>" << endl;
  return;
}

// varName, varname[Expression], subrotincall all starts with an identifier
// identifier || identifier [ || identifier (

// We have to look ahead to see what we got
if(tokens[counter].second =="identifier" ){
  
  // identifier [ aka varName[expression]
  if (tokens[counter + 1].first =="["){
    
    // add identifier
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  
    // add [
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add expression
    compileExpression(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

    // add ]
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    indentLevel--;
    tabs(indentLevel,file);
    file << "</term>" << endl;
    return;
  }

  if (tokens[counter + 1].first =="(" || tokens[counter + 1].first =="." ){
    // we now have a subroutineCall
     compileSubroutineCall(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

    indentLevel--;
    tabs(indentLevel,file);
    file << "</term>" << endl;
    return;
  }

  // add identifier (if its only varName)
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</term>" << endl;
  return;
}

// (expression)
if(tokens[counter].first == "(" ){

  // add (
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add expression
  compileExpression(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

  // add (
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</term>" << endl;
  return;
}

// unnaryOP term
if(tokens[counter].first == "-" || tokens[counter].first == "~" ){
  // add - or ~
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  //  add term
  compileTerm(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);
  
  indentLevel--;
  tabs(indentLevel,file);
  file << "</term>" << endl;
  return;
}

  

}

void compileExpressionList (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){

  tabs(indentLevel,file);
  file << "<expressionList>" << endl;
  indentLevel++;

  while(tokens[counter].first != ")"){

    // add expression
    compileExpression(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

    // if , add ,
    if(tokens[counter].first == ","){
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
    }

  }

  indentLevel--;
  tabs(indentLevel,file);
  file << "</expressionList>" << endl;

}

void compileSubroutineCall (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, pair <int,int> &nrOfPara, pair <int,int> &nrOfCurl, int &indentLevel){

    //tabs(indentLevel,file);
    //file << "<subroutineCall>" << endl;
    //indentLevel++;

    // add identifier
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    if (tokens[counter].first =="("){
      // (
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

      // add expressionList
      compileExpressionList(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

      // )
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
      
    }
    else{
      // add .
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

      // add subrotineName (identifier)
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

      // add (
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

      // add expressionList
      compileExpressionList(tokens,counter,file,nrOfPara,nrOfCurl,indentLevel);

      // add )
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
      
    }

    //indentLevel--;
    //tabs(indentLevel,file);
    //file << "</subroutineCall>" << endl;

}

/*
compileClass
compileClassVarDec
compileSubroutineDec
compileParameterList
compileSubroutineBody
compileVarDec
compileStatments

compileLet
compileIf
compileWhile
compileDo
compileReturn

compileExpression
compileTerm
compileExpressionList

compileSubrotineCall

*/

void tabs(int intendLevel, ofstream &file){
  for(int i =0;i <intendLevel;i++){
    file << '\t';
  }

}

void addMarkup(const string &first,const string &second,int &counter, const int &indentLevel, ofstream &file){

 tabs(indentLevel,file);

file << "<" << second << "> " <<
          first <<
          " </" << second << "> "<<endl;
  counter++;
}