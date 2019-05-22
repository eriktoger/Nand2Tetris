#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;


/*
need to get directories to work
and correct name (now it is just test.xml)
my code remove whitespaces in string constants

but all textcompares passed!

project 11
added symbolTables as arguments.
I still have no clue what to do in this project.

*/

struct symbolTable{
  string name;
  string type;
  string kind;
  int index;
  
  // var/local, argument,static,field,class,subrotine
  //vector<int> index{0,0,0,0};
};

void jackTokenizer( vector < pair <string,string> > &,string);

void compileEngine (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &, vector <struct symbolTable> &);
void compileEngine2 (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &);

// the big ones
void compileClass (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &, vector <struct symbolTable> &);

void compileClassVarDec (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &,vector <struct symbolTable> &);

void compileSubroutineDec (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &,vector <struct symbolTable> &);

void compileParameterList (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &,vector <struct symbolTable> &);

// this bool makes it so i can add the Main.Main # (number) in CSCB
// so if its a constructor i can add some stuff after
void compileSubroutineBody (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &,vector <struct symbolTable> &, string);

void compileVarDec (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &,vector <struct symbolTable> &);

void compileStatements (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &,vector <struct symbolTable> &);

//  the statments

void compileLet (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &,vector <struct symbolTable> &);
void compileIf (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &,vector <struct symbolTable> &);
void compileWhile (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &,vector <struct symbolTable> &);
void compileDo (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &, vector <struct symbolTable> &);
void compileReturn (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &, vector <struct symbolTable> &);

// expressio, term and expressionList
void compileExpression (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &, vector <struct symbolTable> &);
void compileTerm (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &, vector <struct symbolTable> &);
int compileExpressionList (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &, vector <struct symbolTable> &);

//subrotuineCall
void compileSubroutineCall (const vector < pair<string,string> > &,int &, ofstream &, ofstream &,int &, vector <struct symbolTable> &, vector <struct symbolTable> &);

//other
void tabs(int, ofstream &);
void addMarkup(const string & ,string const &,int &,const int &,ofstream &file);

// symbolTable
void symbolTable ();



int main() {
  ifstream myfile;
  myfile.open ("new.jack");
  vector <char> symbol  {'{' , '}' , '(' , ')' , '[' ,']' ,'.', ',' , ';' , '+' , '-' ,'*' ,'/', '&' , '|' , '<' , '>' ,'=' ,'~'};

  vector < pair<string,string> > tokens;

  if(myfile.is_open()){
    string word;
    string eatLine;
    
    bool nestedComment = false;
    bool isString = false;
    string partWord ="";
    while(myfile >> word ){
      
      //cout <<word <<endl;
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
      if(!nestedComment ){

        if(isString){
          partWord+=" ";
        }

    
        for(int i =0; i < word.length(); i++){

            if( word[i]=='"'){
              isString = true;
            }

            // this is to catch when comments are not seperated with //
            // let x = 1;//hello
            //let x = 1; //hello
            // nested comment at the end is also a problem 
            if(word[i] =='/'){
             if(word[i+1]=='/' && !isString && (i+1) < word.length()){
              partWord ="";
              getline(myfile,eatLine);
              break;
             }

             // i dont really know what to do here
             if(word[i+1]=='*' && !isString && (i+1) < word.length()){ 
               //nestedComment = true;
             }
              
            }
           

          if( !isString && find(symbol.begin(),symbol.end(),word[i])!=symbol.end() ){
            // we found a symbol;
             // this means that the current partWord (if not empty) should be tokenized
            // and the symbol
            if(partWord.length() > 0){
              //cout << partWord<<endl;
              jackTokenizer(tokens,partWord);
              
                partWord ="";
              
            }
            //cout << c <<endl;
            string cWord (1,word[i]);
            jackTokenizer(tokens,cWord);
          }
          else{
            // no symbol
            // append to partword
            partWord += word[i];

          }

          if(partWord.length() > 2 && partWord[0]=='"' &&partWord[partWord.length()-1]=='"'){
        // this means that we have built an string
        jackTokenizer(tokens,partWord);
        isString = false;
        partWord ="";
      }

      }

      //cout <<partWord<<endl;
      
      
      if(!isString && partWord.length() > 0){
         // if partWord is not empty it should be tokenized

          //cout << partWord<<endl;
          jackTokenizer(tokens,partWord);
          partWord ="";
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
    //cout <<p.first<<" "<<p.second << endl;
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

  ofstream vmFile;
  string vmFileName = "test.vm";
  vmFile.open(vmFileName);

  vector <struct symbolTable> classSymbolTable;
  vector <struct symbolTable> subroutineSymbolTable;

  for( pair<string,string> p : tokens){
    compileEngine(tokens,counter,outfile,vmFile, indentLevel,classSymbolTable,subroutineSymbolTable);
    if(counter >= tokens.size()){
      break;
    }
    
  }
  cout<< endl;
  cout <<"classSymbolTable"<<endl;
  cout << "name"<<" " << "type" << " " << "kind" << " "<< "#"<<endl;  
  for(struct symbolTable st: classSymbolTable){

    cout << st.name<< " "<< st.type<< " "<<st.kind << " "<<st.index<<endl;
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

void compileEngine (const vector < pair<string,string> > &tokens,int &counter,ofstream &file,ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){

  if(tokens[counter].first == "class"){
    compileClass(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
  }

}

void compileClass (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){ 

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
    cout << tokens[counter].first<<endl;
  }

  addMarkup(tokens[counter].first, tokens[counter].second, counter, indentLevel, file);

  // classVarDec*
   // this is were classSymbolTable is cleard.
  classSymbolTable.clear();

  while(tokens[counter].first=="static" || tokens[counter].first=="field"){

    compileClassVarDec(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

  }
  

  // subrutineDec*
 
  while(tokens[counter].first=="constructor" || tokens[counter].first=="function" || tokens[counter].first=="method"){

     // subroutineSymbolTable is cleard 
    subroutineSymbolTable.clear();

    string nameOfFunction = tokens[counter+2].first;

    compileSubroutineDec(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

    // print the subroutineSymbolTable?
  cout <<endl;
  cout << nameOfFunction<<endl;
  cout << "name"<<" " << "type" << " " << "kind" << " "<< "#"<<endl;  
  for(struct symbolTable st: subroutineSymbolTable){

    cout << st.name<< " "<< st.type<< " "<<st.kind << " "<<st.index<<endl;
  }

  }

   // add }
  if(tokens[counter].first !="}"){
    cout << "Error in compileClass, not }"<<endl;
    cout <<tokens[counter].first<<endl;
    cout <<tokens[counter+1].first<<endl;
    cout <<tokens[counter+2].first<<endl;
    cout <<tokens[counter+3].first<<endl;
    cout <<tokens[counter+4].first<<endl;
    cout <<tokens[counter+5].first<<endl;
    cout <<tokens[counter+6].first<<endl;
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

// is is where classSymbolTable is filled



void compileClassVarDec (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){ 

struct symbolTable tempClassTableInput;
string kind;
int index;

tabs(indentLevel,file);
file << "<classVarDec>"<<endl;
indentLevel++;


// get kind
tempClassTableInput.kind = tokens[counter].first;
kind = tokens[counter].first;
// add static or field
addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

//add type
tempClassTableInput.type = tokens[counter].first;
// if I want to check this one i need the save class names.
addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

// add varName
tempClassTableInput.name = tokens[counter].first;
addMarkup( tokens[counter].first, tokens[counter].second,counter, indentLevel, file);


// updates index
index = 0;
for(struct symbolTable cst : classSymbolTable){
  if (cst.kind == kind){
    index++;
  }
  
}
tempClassTableInput.index = index;

classSymbolTable.push_back(tempClassTableInput);

//possible more varNames
while(tokens[counter].first == ","){
  // add ,
  addMarkup( tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  
  // add varname
  tempClassTableInput.name = tokens[counter].first;
  addMarkup( tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  
  index++;
  tempClassTableInput.index = index;
  classSymbolTable.push_back(tempClassTableInput);
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

/*
struct symbolTable{
  string name;
  string type;
  string kind;
  int index;
};
*/
// where the subroutineSymbolTable is filled

void compileSubroutineDec (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){ 

  struct symbolTable tempSubTableInput;
  string kind;
  int index;
  string subType = tokens[counter].first;
  bool isConstructor = false;

  if( subType =="method"){
    tempSubTableInput.name ="this";
    //should be tokens[1].first that is class name
    tempSubTableInput.type = tokens[1].first;
    tempSubTableInput.kind ="argument";
    tempSubTableInput.index = 0;
    subroutineSymbolTable.push_back(tempSubTableInput);
  }
  if(subType =="constructor"){
    isConstructor = true;
  }

  

  tabs(indentLevel,file);
  file << "<subroutineDec>"<<endl;
  indentLevel++;

  // add constructor, function or method
  //vmFile << tokens[counter].first<<" ";
  vmFile << "function "; // everything is function in vm
  
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);


  

  // add void or type
  // add an if? need classname
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add subroutinName
  // functions is Main.function
  // but draw() inside the construtor in snake is just do draw():
  // but then this is implied.
  //string isNew = 
  // does this really work? what if its a different class?
  // but maybe im mistaken,

  // this should work well for functions and constructors.
  // i dont know if its in the right place

  // the problem is that the subroutineTable isnt updated.
  vmFile << tokens[1].first<< "."<< tokens[counter].first;
  //moved back to subrotineName
  //vmFile << " " << subroutineSymbolTable.size() + classSymbolTable.size() <<endl;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);


  // add (
    
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  
  // add parameterList
  compileParameterList(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

  
  // add )
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add subrotuineBody
  compileSubroutineBody(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable, subType);
  
  indentLevel--;
  tabs(indentLevel,file);
  file << "</subroutineDec>"<<endl;
  

}

void compileParameterList (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable, vector <struct symbolTable> &subroutineSymbolTable){ 
//( (type varname) (, type varName)* ) ?

  struct symbolTable tempArgument;
  
  tabs(indentLevel,file);
  file << "<parameterList>" << endl;
  indentLevel++;


  if(tokens[counter].first != ")"){

    // add type
    tempArgument.type = tokens[counter].first;
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add varname
    tempArgument.name =tokens[counter].first; 
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    //only time its index 1 is when its method
    //look like its working
    tempArgument.index = subroutineSymbolTable.size();
    //cout << tempArgument.index<<endl;

    tempArgument.kind = "argument";

    // push it
    subroutineSymbolTable.push_back(tempArgument);

  }

  

  int counterParameterList = 0;
  while(tokens[counter].first != ")" ){

    //add ,
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add type
    tempArgument.type = tokens[counter].first;
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add varName
    tempArgument.name = tokens[counter].first;
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // change index
    tempArgument.index++; // can I do this?

     // push it
    subroutineSymbolTable.push_back(tempArgument);

    counterParameterList++;
    if(counterParameterList > 200){
      cout << "break counterParameterList"<<endl;
      break;
    }

  }
  //adding the parameters to function class.functioName
  // this only added the arguments
  //vmFile << " "<<counterParameterList<<endl;

  indentLevel--;
  tabs(indentLevel,file);
  file << "</parameterList>" << endl;
}

void compileSubroutineBody (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable, vector <struct symbolTable> &subroutineSymbolTable,string subType){
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
    compileVarDec(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
  }
  
   // adding the length of symbolTable?
   // after the varDec the symbolTable is compileTerm
   // this is done to the ex function Main.main 
  // this was moved inte subroutecall compileSubroutineDec
  // and moved back
  int fieldCounter = 0;
  for(struct symbolTable st : subroutineSymbolTable){
    if(st.kind!= "argument"){

          fieldCounter++;
    }
    // arguments are pushed not allocated


  }
  //vmFile<<" "<< subroutineSymbolTable.size()<<endl;
  vmFile<<" "<< fieldCounter <<endl;
  // it needs to be here, so its after.
  // this wasnt correct )= see test.vm
  // how should we know that we are handling a constructor?
  // subType == "constructor"
  if(subType == "constructor"){
    // 5.6 13 min
    // but he dosent have constructor point.new 2
    // is this just a function thing?
    // ther is function Square.new 2 in Square.vm
    int fieldsCounter = 0;
    for(struct symbolTable st : classSymbolTable){
      if(st.kind =="field"){
        fieldsCounter++;
      }
    }

    vmFile <<"push constant "<<fieldsCounter<<endl;
    vmFile <<"call Memory.alloc 1"<<endl;
    //this call return a free space, and the free spaces adress
    // is oped to pointer 0 which is this 0.
    vmFile <<"pop pointer 0"<<endl;
    // why isnt there any information about functions!!!!
    // http://nand2tetris-questions-and-answers-forum.32033.n3.nabble.com/Differentiating-functions-methods-and-constructors-td4026430.html
    // http://nand2tetris-questions-and-answers-forum.32033.n3.nabble.com/Differentiating-between-function-and-method-calls-td4027251.html#a4027253
  }

  //if method
  //add push argument 0
  // push pop pointer 0
  //moved this to before parameterList
  // http://nand2tetris-questions-and-answers-forum.32033.n3.nabble.com/Project-11-gt-Pong-Error-this-segment-must-be-in-the-heap-range-td4030998.html#a4030999
  
  if( subType =="method"){
    vmFile << "push argument 0"<< endl;
    vmFile << "pop pointer 0"<<endl;
  }
  




  // statments
  compileStatements(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

  // addin }
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</subroutineBody>" << endl;

}

void compileVarDec (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable, vector <struct symbolTable> &subroutineSymbolTable){

  tabs(indentLevel,file);
  file << "<varDec>" << endl;
  indentLevel++;
  
  struct symbolTable tempLocal;
  tempLocal.kind ="local";

  // updates index
  int index = 0;
  for(struct symbolTable cst : subroutineSymbolTable){
    if (cst.kind == "local"){
      index++;
    }
  
  }
  tempLocal.index = index;

  // add var
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add type
  tempLocal.type = tokens[counter].first;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  //add varName
  tempLocal.name = tokens[counter].first; 
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  //push it
  subroutineSymbolTable.push_back(tempLocal);
  int counter1 = 0;
  while(tokens[counter].first != ";"){

    // add ,
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add varName
    tempLocal.name = tokens[counter].first; 
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    //update index
    tempLocal.index++;

    //push it
    subroutineSymbolTable.push_back(tempLocal);
    counter1++;
    if(counter1>100){
      cout <<"counter1"<<endl;
      //break;
    }
  }

  // add ;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</varDec>" << endl;
}

void compileStatements (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){

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
            compileLet(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
            continue;
          }

          if(tokens[counter].first =="if" ){
            compileIf(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
            continue;
          }

          if(tokens[counter].first =="while" ){
            compileWhile(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
            continue;
          }

          if(tokens[counter].first =="do" ){
            compileDo(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
            continue;
          }

          if(tokens[counter].first =="return" ){
            compileReturn(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
            continue;
          }

        }


  indentLevel--;
  tabs(indentLevel,file);
  file << "</statements>" << endl;

}

void compileLet (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){
  
  tabs(indentLevel,file);
  file << "<letStatement>" << endl;
  indentLevel++;

  // add let
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add varName
  string tempName = tokens[counter].first; 
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // we have an index
  if(tokens[counter].first == "["){
    
    // see 21:28 in 5.8 
    string tempKind;
    int tempIndex = -1;

    for(struct symbolTable st : subroutineSymbolTable){
      if(st.name == tempName){
        tempKind = st.kind;
        tempIndex = st.index;
        break;
      }
    }
    if(tempIndex == -1){
      for(struct symbolTable st : classSymbolTable){
        if(st.name == tempName){
          tempKind = st.kind;
          tempIndex = st.index;
        break;
        }
     }
    }

    if(tempKind == "field"){
      tempKind = "this";
    }

    vmFile<<"push "<<tempKind<<" "<<tempIndex<<endl;
    
    // add [
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add expression
    compileExpression(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
    // adding and saving to temp 0
    vmFile<<"add"<<endl;

    // add ]
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    //add =
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add expression
  compileExpression(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

  vmFile <<"pop temp 0"<<endl;
  vmFile << "pop pointer 1" << endl;
  vmFile << "push temp 0" << endl;
  vmFile << "pop that 0" << endl;
 
  // add ;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

 
  indentLevel--;
  tabs(indentLevel,file);
  file << "</letStatement>" << endl;

  }
  else{
    //add =
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add expression
  compileExpression(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
  string tempKind;
  int tempIndex = -1;
  for(struct symbolTable st : subroutineSymbolTable){
    if(tempName == st.name ){
      tempKind = st.kind;
      tempIndex = st.index;
      break;
    }
  }
  // if not in subroutineTavle, maybe in classTable
  if(tempIndex == -1){
    for(struct symbolTable st : classSymbolTable){
      if(tempName == st.name ){
        
        if(st.kind == "field"){
          tempKind = "this";
        }
        else{
          tempKind = st.kind; 
        }
        
        tempIndex = st.index;
        break;
      }
    }
  }
  // local is local
  // but field is this
  // static is stacic ???
  // argument is ???
  // when do we want temp?
  // when we just want to trash it
  vmFile<<"pop "<<tempKind<<" "<<tempIndex<<endl;

  // add ;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

 
  indentLevel--;
  tabs(indentLevel,file);
  file << "</letStatement>" << endl;

  }

  

}

void compileIf (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){

  tabs(indentLevel,file);
  file << "<ifStatement>" << endl;
  indentLevel++;

  // add if
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add (
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add expression
  compileExpression(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
  vmFile <<"not"<<endl;
  // https://drive.google.com/file/d/1DfGKr0fuJcCvlIPABNSg7fsLfFFqRLex/view
  // slide 26

  // need to add running number!
  string ifLabel= "IL" + to_string(counter);
  vmFile <<"if-goto " << ifLabel<<endl;

  //add )
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add {
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add statements
  compileStatements(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

  // add }
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  string elseLabel= "EL" + to_string(counter);
  //if we have done the if we need to skip the else.
  // it can be a empty else. goto elselabel direcetly followed 
  // by elselabel.
  vmFile <<"goto " << elseLabel<<endl;
  vmFile <<"label "<<ifLabel<<endl;

  if(tokens[counter].first == "else"){
  
    //add else
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add {
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // add statements
    compileStatements(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

    // add }
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);


  }
  vmFile <<"label "<<elseLabel<<endl;
  indentLevel--;
  tabs(indentLevel,file);
  file << "</ifStatement>" << endl;

}

void compileWhile (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){
  
  tabs(indentLevel,file);
  file << "<whileStatement>" << endl;
  indentLevel++;

  //add while
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

   // add (
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add label
  string whileLabel1 = "WL"+to_string(counter);
  vmFile<<"label " << whileLabel1<<endl;

  // add expression
  compileExpression(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
  
  string whileLabel2 = "WL"+to_string(counter);
  vmFile<<"not"<<endl;
  vmFile<<"if-goto "<<whileLabel2<<endl;

  //add )
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add {
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add statements
  compileStatements(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

  // add }
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  // go back to start
  vmFile <<"goto "<<whileLabel1<<endl;

  // when we jump we jump here
  vmFile <<"label "<<whileLabel2<<endl;
  indentLevel--;
  tabs(indentLevel,file);
  file << "</whileStatement>" << endl;

}

void compileDo (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){
  tabs(indentLevel,file);
  file << "<doStatement>" << endl;
  indentLevel++;

  //add do
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  // add subroutineCall
  compileSubroutineCall(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

  // http://nand2tetris-questions-and-answers-forum.32033.n3.nabble.com/pop-temp-zero-td4031744.html
  // why always pop temp 0?
  // beacause when it returns it leaves a trace on the stack.
  // the question is if where this trace is?
  //or do I hope that Main.fillmemory does it for me?
  // yes by contract every function do return somethung
  //see 5.11 15 min
  vmFile << "pop temp 0"<<endl;

  // add ;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</doStatement>" << endl;

}

void compileReturn (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){

  tabs(indentLevel,file);
  file << "<returnStatement>" << endl;
  indentLevel++;
  //add return
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  if(tokens[counter].first !=";"){
    // add expression
    compileExpression(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
      vmFile<<"return"<<endl;
  }
  else{
    // void has empty return statment!
    vmFile<<"push constant 0"<<endl;
    vmFile<<"return"<<endl;
    vmFile <<"pop temp 0"<<endl;
  }



  //add ;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</returnStatement>" << endl;

}


void compileExpression (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){

  tabs(indentLevel,file);
  file << "<expression>" << endl;
  indentLevel++;

  // if i have an expression i would like to generate vmcode
  // can i tell when this expression ends?
  // var int x = foo(2+u); here the expression is 2+u

  //  expression always starts with term
  compileTerm(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
  
  // we need to add the op after the expression
  string operation;

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

          // add op before term
          operation = tokens[counter-1].first;

          // add term after op
         compileTerm(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

         if(operation =="+"){
          vmFile<<"add"<<endl;
        }
        if(operation =="*"){
          vmFile<<"call Math.multiply 2"<<endl;
        }
        if(operation == "="){
          // only in if/while-statment
          // i hade problems with eq before???
          vmFile <<"eq"<<endl;
        }
        if(operation == "&"){
          vmFile <<"and"<<endl;
        }
        if(operation == "<"){
          vmFile <<"lt"<<endl;
        }
        if(operation == ">"){
          vmFile <<"gt"<<endl;
        }
        if(operation == "-"){
          vmFile <<"sub"<<endl;
        }
        if(operation == "|"){
          vmFile <<"or"<<endl;
        }
        if(operation == "/"){
          vmFile<<"call Math.divide 2"<<endl;
        }

        }
        // i think the if statements should be inside here?
        // why is it a while? beacause we can have alot of them in a statement/expression

        

  indentLevel--;
  tabs(indentLevel,file);
  file << "</expression>" << endl;

}

void compileTerm (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){

  tabs(indentLevel,file);
  file << "<term>" << endl;
  indentLevel++;

//intergerConstant
if(tokens[counter].second =="integerConstant" ){

  vmFile <<"push constant " << tokens[counter].first<<endl;
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  
  indentLevel--;
  tabs(indentLevel,file);
  file << "</term>" << endl;

  

  return;
}

// stringConstant
if(tokens[counter].second =="stringConstant" ){
  
  //push length of string
  vmFile<<"push constant " << tokens[counter].first.size()<<endl;

  vmFile << "call String.new 1"<<endl;

  int cInt;
  for(char c : tokens[counter].first ){
    cInt = c;
    vmFile << "push constant " << cInt<<endl;
    vmFile << "call String.appendChar 2"<<endl;
  }
  
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  
 
  indentLevel--;
  tabs(indentLevel,file);
  file << "</term>" << endl;
  return;
}

// keywordConstants
if(tokens[counter].first =="true" ||tokens[counter].first =="false" ||
   tokens[counter].first =="this" || tokens[counter].first =="null"){
  
  if(tokens[counter].first =="true"){
    vmFile<<"push constant 1"<<endl;
    vmFile<<"neg"<<endl;
  }
  if(tokens[counter].first =="false" ||tokens[counter].first =="null" ){
    vmFile<<"push constant 0"<<endl;
  }

  //under the assumption that this only appears
  // in return this.
  if(tokens[counter].first =="this"){
    vmFile <<"push pointer 0"<<endl;
  }


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
    // we want to push the value if arr[x] to the stack

    int tempIndex = -1;
    string tempKind;
    //check in subTable
    for(struct symbolTable st : subroutineSymbolTable){
      if(st.name == tokens[counter].first){
        tempIndex = st.index;
        tempKind = st.kind;
        break;
      }
    }
    //check in classTable
    if(tempIndex==-1){
    for(struct symbolTable st : classSymbolTable){
      if(st.name == tokens[counter].first){
        tempIndex = st.index;
        tempKind = st.kind;
      }
    } 
    }

    // fields held in local
    if(tempKind == "field"){
        tempKind = "this";
    }

    // push base address if arr
    vmFile << "push " << tempKind<< " "<< tempIndex<<endl;

    // 
    // add identifier
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
  
    // add [
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    // here base adress of the expression should be pushed.
    // add expression
    compileExpression(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

    //add base adress of arr and exp
    vmFile <<"add"<<endl;
    // align that
    vmFile <<"pop pointer 1"<<endl;
    // getting the number inside arr[x]
    vmFile <<"push that 0"<<endl;
    
    // add ]
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

    indentLevel--;
    tabs(indentLevel,file);
    file << "</term>" << endl;
    return;
  }

  if (tokens[counter + 1].first =="(" || tokens[counter + 1].first =="." ){
    // we now have a subroutineCall
     compileSubroutineCall(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

    indentLevel--;
    tabs(indentLevel,file);
    file << "</term>" << endl;
    return;
  }

  // add identifier (if its only varName)
  int tempIndex = -1;
  string tempKind;
  //check in subTable
  for(struct symbolTable st : subroutineSymbolTable){
    if(st.name == tokens[counter].first){
      tempIndex = st.index;
      tempKind = st.kind;
      break;
    }
  }
  //check in classTable
  if(tempIndex==-1){
   for(struct symbolTable st : classSymbolTable){
    if(st.name == tokens[counter].first){
      tempIndex = st.index;
      tempKind = st.kind;
    }
   } 
  }

  // fields held in local
  if(tempKind == "field"){
    tempKind = "this";
  }

  // how to know if we have an method?
 
  // but this is to late???
  // if method the object as to be stet
  // push argument 0
  // pop pointer 0


  vmFile << "push " << tempKind<< " "<< tempIndex<<endl;
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
  compileExpression(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

  // add (
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  indentLevel--;
  tabs(indentLevel,file);
  file << "</term>" << endl;
  return;
}

// unnaryOP term
string operation = tokens[counter].first;
if(tokens[counter].first == "-" || tokens[counter].first == "~" ){
  // add - or ~
  addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

  //  add term
  compileTerm(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);
  if(operation == "-"){
    vmFile <<"neg"<<endl;
  }
  if(operation == "~"){
    vmFile <<"not"<<endl;
  }

  indentLevel--;
  tabs(indentLevel,file);
  file << "</term>" << endl;
  return;
}

  

}
// return int so we now how many arguments to eat in the vmcode
int compileExpressionList (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel,vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){

  tabs(indentLevel,file);
  file << "<expressionList>" << endl;
  indentLevel++;

  int argumentCounter = 0;
  while(tokens[counter].first != ")"){

    // add expression
    compileExpression(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

    // if , add ,
    if(tokens[counter].first == ","){
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
    }
    argumentCounter++;
  }
  // all subroutinecalls ends with an expressionList
  // so after call class.method the number of arguments are added.

  indentLevel--;
  tabs(indentLevel,file);
  file << "</expressionList>" << endl;
  return argumentCounter;

}

void compileSubroutineCall (const vector < pair<string,string> > &tokens,int &counter,ofstream &file, ofstream &vmFile, int &indentLevel, vector <struct symbolTable> &classSymbolTable,vector <struct symbolTable> &subroutineSymbolTable){

    //tabs(indentLevel,file);
    //file << "<subroutineCall>" << endl;
    //indentLevel++;

    // add identifier
    // is it so that the name always is class.functionname?
    // if 
    string functionName = tokens[counter].first; // saves it for vmFile 
    addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
    int argumentCounter = 0;
    if (tokens[counter].first =="("){
      // (
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);
      
      //should be pushed first
      vmFile << "push pointer 0" <<endl;
      // add expressionList
      argumentCounter += compileExpressionList(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

      // )
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

       //need to add the call function after expression list so it comes after the arguments(in the expressionlist)
       // added the className here.
      // this asumes this is a method
      argumentCounter++;
      // is pointer 0 always the object?
      
      vmFile << "call "<<tokens[1].first<< "." <<functionName<< " "<<argumentCounter<<endl;
      
    }
    else{

      //if functionName is from a class 
      // it should be in the symbolTable
      // it seems so change stuff correct
      //methods should push the object
      int tempIndex = -1;
      for(struct symbolTable st : subroutineSymbolTable){
        if(functionName == st.name ){
          string stKind;
          if(st.kind =="field"){
            stKind = "this";
          }
          else{
            stKind = st.kind;
          }
          vmFile<<"push "<< stKind<<" "<<st.index<<endl;
           argumentCounter++;
          functionName = st.type;
          tempIndex = st.index;
         

          
          break;
        }
      }
      // if not in subroutineTavle, maybe in classTable
      if(tempIndex == -1){
        for(struct symbolTable st : classSymbolTable){
          if(functionName == st.name ){
            string stKind;
          if(st.kind =="field"){
            stKind = "this";
          }
          else{
            stKind = st.kind;
          }

          vmFile<<"push "<< stKind<<" "<<st.index<<endl;  
           argumentCounter++;
           
          functionName = st.type;
          tempIndex = st.index;
          break;
         }
        }
      }

      // add .
      functionName+= tokens[counter].first; //adds .
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

      // add subrotineName (identifier)
      functionName+=tokens[counter].first;
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

      // add (
      addMarkup(tokens[counter].first, tokens[counter].second,counter, indentLevel, file);

      // add expressionList
      argumentCounter += compileExpressionList(tokens,counter,file,vmFile,indentLevel,classSymbolTable,subroutineSymbolTable);

      //need to add the call function after expression list so it comes after the arguments(in the expressionlist)
      vmFile << "call "<<functionName<<" "<<argumentCounter<<endl;

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

void symbolTable (){

}