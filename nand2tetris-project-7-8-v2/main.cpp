#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;

/*
ALL test passed!
time to go back and see if everthing stilld passes
and clean up the code!
And read files from directories

fix list:
how comment is added is inconsistance
after project 8 clean up the code, 
should i keep comments? now i filter out starting comments.
mabye if "//" is found stop reading? substr it out.

i have a bad feeling about eq and jump statments. That true should be +1.
but i have passed the testes. and -1 comes from his code-example
bad feeling was right, dont know if it is fixed

change trimedVMCodevector to trimedVMCodeVector


go through https://github.com/xctom/Nand2Tetris/blob/master/projects/08/src/CodeWriter.java
and see if your code does the same.

How to use R11 to R14?? ho many 
R5-R12 is temp
13,14,15 is? i dont know but 13 is used in pop
R16 is the first variable i think

function labels should be named xxx.foo$label
Otherwise the class/file information is lost.
2-7  time 10:00

and booting write init 2-7 time 6

adding read direcotry (all vm.files in it)
*/


// next thing to do is to pop argument, this,that,temp, local

// the passer should take file name as argument and return trimedVMCode

void parser(string, vector<string>&);

void codeWriter (string, string, ofstream &);
void addPush(string, string, ofstream &);
//void addPushStatic(string, string, fstream &);
void addPop(string, string, ofstream &);
//void addPopStatic(string, string, fstream &);
void addAddSubAndOr(string,string, ofstream &);
void addEqLtGt(string, string, ofstream &);
void addNegNot(string,string,ofstream &);
void addLabel(string, ofstream &);
void addIfGoto(string, ofstream &);
void addGoto(string, ofstream &);
void addFunction(string, ofstream &);
void addReturn(string, ofstream &);
void addCall(string, ofstream &);
void addInit(ofstream &);
string trimFileName(string);

vector<string> splitStringtoVector (string);

int LabelCounter = 0;

int main() {
  ifstream vmCode;
  ifstream vmCode2;
  ifstream vmCode3;
  ofstream asmCode;
  string temp;
  string trimedVMCode ="";
  string trimedVMCode2 ="";
  string trimedVMCode3 ="";
  string fileName = "Sys.vm";
  string trimedFileName = trimFileName(fileName);
  cout << trimedFileName << endl;
  string trimedFileName2;
  string trimedFileName3;

  vector <string> trimedVMvector;
  vector <string> trimedVMvector2;
  vector <string> trimedVMvector3;

  vmCode.open (fileName);
  if ( !vmCode.is_open()){
    cout << "wrong file name! vmCode is not open"<<endl;
    return -1;
  }

 

  cout<<"1"<<endl;
  asmCode.open("test.asm", std::ofstream::out | std::ofstream::trunc);

   addInit(asmCode);
  
  if (vmCode.is_open()) {
    while (!vmCode.eof()) {

    //vmCode >> temp;
    getline(vmCode, temp);
    parser(temp,trimedVMvector);
    //asmCode << parser(temp,trimedVMvector);
 }
}
cout<<"2"<<endl;
 vmCode.close();

for (string trimedVMCode:trimedVMvector){
    codeWriter(trimedVMCode, trimedFileName,  asmCode);
  }

// do it again if two files are present?


fileName = "Class1.vm";
trimedFileName2 = trimFileName(fileName);
vmCode2.open (fileName);

  if (vmCode2.is_open()) {
    while (!vmCode2.eof()) {

    //vmCode >> temp;
    getline(vmCode2, temp);
    parser(temp,trimedVMvector2);
    //asmCode << parser(temp,trimedVMvector);
 }
}

for (string trimedVMCode:trimedVMvector2){
    codeWriter(trimedVMCode, trimedFileName2,  asmCode);
  }

vmCode2.close();
cout<<"3"<<endl;

// do it again if three files are present?
 
 fileName = "Class2.vm";
 trimedFileName3 = trimFileName(fileName);
 vmCode3.open (fileName);

  if (vmCode3.is_open()) {
    while (!vmCode3.eof()) {

    //vmCode >> temp;
    getline(vmCode3, temp);
    parser(temp,trimedVMvector3);
    //asmCode << parser(temp,trimedVMvector);
 }
}
vmCode3.close();

for (string trimedVMCode:trimedVMvector3){
    codeWriter(trimedVMCode, trimedFileName3,  asmCode);
  }


 
  // this trimedVMCode shall be passed to codeWriter
  //cout << trimedVMCode;
  
  // Init is just added ones
  cout<<"4"<<endl;
 


  

  cout<<"5"<<endl;

 
  
  cout<< "Code is written!"<<endl;
}

void parser( string temp,vector <string> & trimedVMvector){
  
  //remove leading space
  
  temp.erase(temp.begin(), std::find_if(temp.begin(), temp.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
  string command ="";
  if( temp.length() > 1 && temp[0] != '/' && temp[1] != '/'  ){
    // not a comment
    command = temp + '\n';
    //cout<<temp<<endl;
    trimedVMvector.push_back(temp);
  }
  
}

string trimFileName(string fileName){
  string trimedFileName;
  int pos = fileName.find('.');
  trimedFileName = fileName.substr(0,pos);
  return trimedFileName;

}

void codeWriter(string trimedVMCode, string fileName, ofstream &asmCode){
  // get the first command and sen the whole string to the right codewriter subfunction 
  int firstSpace = trimedVMCode.find(' ');
  string firstCommand;

  

  if(firstSpace !=-1){
    firstCommand = trimedVMCode.substr(0,firstSpace);
    //cout<<firstCommand<<endl;
  }
  else{
    firstCommand = trimedVMCode;
    //cout<<firstCommand<<endl;
  }

  if(firstCommand == "push"){
    addPush(trimedVMCode, fileName, asmCode);
  }

   if(firstCommand == "pop"){
    addPop(trimedVMCode,fileName, asmCode);
  }


  // I just added "D+M and M-D"
  // i hope its correct!
  // D is always the bottom number
  // and top number is first in calculations
  if(firstCommand == "add" ){
    addAddSubAndOr(trimedVMCode,"M+D", asmCode);
  }

  if(firstCommand == "sub" ){
    addAddSubAndOr(trimedVMCode,"M-D", asmCode);
  }

  if(firstCommand == "and" ){
    addAddSubAndOr(trimedVMCode,"M&D", asmCode);
  }
  if(firstCommand == "or" ){
    addAddSubAndOr(trimedVMCode,"D|M", asmCode);
  }

  if( firstCommand ==  "eq" ){
    addEqLtGt(trimedVMCode,"JEQ", asmCode);
    //I hade a very strange bug here
    // eq was 3 chars, then by adding and removing
    // a enter in the file it is now 2 chars
    // but 2 chars make it with with == "eg" (=
  }
  if (firstCommand == "lt"){
    addEqLtGt(trimedVMCode,"JLT", asmCode);
  }

  if (firstCommand == "gt"){
    addEqLtGt(trimedVMCode,"JGT", asmCode);
  }

  if(firstCommand == "not"){
    addNegNot(trimedVMCode, "not", asmCode);
  }

  if(firstCommand == "neg"){
    addNegNot(trimedVMCode, "neg", asmCode);
  }

  if(firstCommand == "label"){
    addLabel(trimedVMCode, asmCode);
  }
  if(firstCommand == "if-goto"){
    addIfGoto(trimedVMCode, asmCode);
  }
  if(firstCommand == "goto"){
    addGoto(trimedVMCode, asmCode);
  }
  if(firstCommand == "function"){
    addFunction(trimedVMCode, asmCode);
  }
  if(firstCommand == "return"){
    addReturn(trimedVMCode, asmCode);
  }
  if(firstCommand == "call"){
    addCall(trimedVMCode, asmCode);
  }


}

void addPush(string trimedVMCode, string fileName,  ofstream &asmCode){
  // a push command has 3 components, push segment and number.
   asmCode <<"//" + trimedVMCode<<"\n";

  vector <string> trimedVMCodeVector = splitStringtoVector(trimedVMCode);
  
  string asmCommand="";
  if(trimedVMCodeVector[1]=="constant"){
    asmCommand += "@" + trimedVMCodeVector[2] + '\n'; // get the number 
    asmCommand += "D=A\n"; // save the number
    asmCommand += "@SP\n"; // SP points to
    asmCommand += "A=M\n"; // this address
    asmCommand += "M=D\n"; // and that address gets the number
    asmCommand += "@SP\n"; //back to SP
    asmCommand += "M=M+1\n"; // add one
  }
  else if(trimedVMCodeVector[1]=="static"){
      asmCommand += "@"+ fileName + "." + 
      trimedVMCodeVector[2] +"\n"; 
      cout << fileName<<endl;
      asmCommand += "D=M\n"; // save the value to decrementet
      asmCommand += "@SP\n"; // SP points to
      asmCommand += "A=M\n"; // this address
      asmCommand += "M=D\n"; // and that address gets the number
      asmCommand += "@SP\n"; //back to SP
      asmCommand += "M=M+1\n"; // add one

    }
    else if(trimedVMCodeVector[1]== "pointer") {
        if(trimedVMCodeVector[2]=="0"){
          asmCommand += "@THIS\n"; // get THAT
        }
        else{
          asmCommand += "@THAT\n";
        }
        asmCommand += "D=M\n"; // save what THAT points to
        asmCommand += "@SP\n"; // get SP
        asmCommand += "A=M\n"; // get what adress SP points to
        asmCommand += "M=D\n"; // push that to stack
        asmCommand += "@SP\n"; // SP+1
        asmCommand += "M=M+1\n";

    }
    else{

      if(trimedVMCodeVector[1]=="this"){
        // jag måste hämta this + ofset
        asmCommand += "@THIS\n"; // getting this
        asmCommand += "D=M\n"; // saving the value this in D
      }
        

      if(trimedVMCodeVector[1]=="that"){
        // jag måste hämta this + ofset
        asmCommand += "@THAT\n"; // getting this
        asmCommand += "D=M\n"; // saving the value this in D
      }

      if(trimedVMCodeVector[1]=="temp"){
        // jag måste hämta this + ofset
        asmCommand += "@R5\n"; // getting temp
        asmCommand += "D=A\n"; // temp is special here
      }
        
      if(trimedVMCodeVector[1]=="local"){
        // jag måste hämta this + ofset
        asmCommand += "@LCL\n"; // getting this
        asmCommand += "D=M\n"; // saving the value this in D
      }

      if(trimedVMCodeVector[1]=="argument"){
        // jag måste hämta this + ofset
        asmCommand += "@ARG\n"; // getting this
        asmCommand += "D=M\n"; // saving the value this in D
      }     
      // i changed this from D=A 

        asmCommand += "@" + trimedVMCodeVector[2] + '\n'; // get the offset number 
        asmCommand += "D=D+A\n"; // adding this and offset
        asmCommand += "A=D\n"; // this is the adress to the number
        asmCommand += "D=M\n"; // saving the number do
        asmCommand += "@SP\n"; // SP points to
        asmCommand += "A=M\n"; // this address
        asmCommand += "M=D\n"; // and that address gets the number
        asmCommand += "@SP\n"; //back to SP
        asmCommand += "M=M+1\n"; // add one
      
      }
  
  asmCode << asmCommand;

}

void addPop(string trimedVMCode,string fileName, ofstream &asmCode){
//jag vill ta något från stacken och lägga i
// och sedan backa SP
  asmCode <<"//" + trimedVMCode<<endl;
    string asmCommand="";
  vector <string> trimedVMCodeVector = splitStringtoVector(trimedVMCode);

  if(trimedVMCodeVector[1]=="static"){
        asmCommand += "@SP\n"; // get SP
        asmCommand += "M=M-1\n"; // back one step
        asmCommand += "A=M\n"; // get adress from SP
        asmCommand += "D=M\n"; // number to pop
        asmCommand += "@"+ fileName + "." + trimedVMCodeVector[2] +"\n"; //where to pop
        asmCommand += "M=D\n"; //
  }
  else if(trimedVMCodeVector[1]=="pointer"){
    asmCommand += "@SP\n";
    asmCommand += "M=M-1\n"; // SP -1
    asmCommand += "A=M\n";
    asmCommand += "D=M\n";

    if(trimedVMCodeVector[2]=="0"){
      asmCommand += "@THIS\n"; 
    }
    else{
      asmCommand += "@THAT\n";

    }
    asmCommand += "M=D\n";


  }
  else{

    if(trimedVMCodeVector[1]=="this"){
      //börja med att läsa in SP-1 till D
      // hint 2) You need a temp. variable (you can use R13 - R15) for the popping task 
      // stores destination in R13
      asmCommand += "@THIS\n";// get this
      asmCommand += "D=M\n"; // saves what this/that points to
    }  

    if(trimedVMCodeVector[1]=="that"){
      asmCommand += "@THAT\n";// get that
      asmCommand += "D=M\n"; // saves what this/that points to
    }

    if(trimedVMCodeVector[1]=="temp"){
      asmCommand += "@R5\n";// get temp
      asmCommand += "D=A\n"; // temp is special here
    }  

    if(trimedVMCodeVector[1]=="local"){
      asmCommand += "@LCL\n";// get local
      asmCommand += "D=M\n"; // saves what this/that points to
    }  

    if(trimedVMCodeVector[1]=="argument"){
      asmCommand += "@ARG\n";// get local
      asmCommand += "D=M\n"; // saves what this/that points to
    }

    
    asmCommand += "@"+ trimedVMCodeVector[2] +"\n";// get ofset
    asmCommand += "D=D+A\n"; // saves pop destination
    asmCommand += "@R13\n"; // preparing to store D in R13.
    asmCommand += "M=D\n"; // stores the destination in R13

    // stores number we want to pop to that destination in D
    asmCommand += "@SP\n"; //get SP
    asmCommand += "M=M-1\n"; // back one
    asmCommand += "A=M\n"; // get address to the number we want
    asmCommand += "D=M\n"; // save the number that we want to pop 
    
    // pop it (SP is decrementet along the way)
    asmCommand += "@R13\n"; // R13 has the adress to this,that etc
    asmCommand += "A=M\n"; // now are we in this, that etc
    asmCommand += "M=D\n";

    }

  asmCode << asmCommand;
}

void addAddSubAndOr(string trimedVMCode,string addSubAndOr,  ofstream &asmCode){
  asmCode << "//" + trimedVMCode<<endl;
  string asmCommand="";
  asmCommand += "@SP\n"; // get SP
  asmCommand += "M=M-1\n"; // back one step
  asmCommand += "@SP\n"; // get new SP
  asmCommand += "A=M\n"; // and the adress it points to
  asmCommand += "D=M\n"; //saves the number SP points to
  
  asmCommand += "@SP\n"; // get SP
  asmCommand += "M=M-1\n"; // back one step
  asmCommand += "@SP\n"; // get new SP
  asmCommand += "A=M\n"; // and the adress it points to
  asmCommand += "D="+addSubAndOr+ "\n"; //saves the number SP points to
  
  asmCommand += "@SP\n"; // get SP
  asmCommand += "A=M\n"; // get address SP points to
  asmCommand += "M=D\n"; // saves D(added number)
  asmCommand += "@SP\n"; // back to SP
  asmCommand += "M=M+1\n"; // add one
  asmCode << asmCommand;
}
void addEqLtGt(string trimedVMCode,string jumpCommand, ofstream & asmCode){
  asmCode << "//" + trimedVMCode<<endl;
  // if there are two numbers on the stack
  // these two are compared
  // i think we take D-M and check if zero
  // should i make a method out 
  string asmCommand="";
  asmCommand += "@SP\n"; // get SP
  asmCommand += "M=M-1\n"; // back one step
  asmCommand += "@SP\n"; // get new SP
  asmCommand += "A=M\n"; // and the adress it points to
  asmCommand += "D=M\n"; //saves the number SP points to
  
  asmCommand += "@SP\n"; // get SP
  asmCommand += "M=M-1\n"; // back one step
  asmCommand += "@SP\n"; // get new SP
  asmCommand += "A=M\n"; // and the adress it points to
  asmCommand += "D=M-D\n"; // top - bottom
  // This should be right!

  // what i want now is to check if D == 0
  // and is 1
  // start looking at Unit 0.3: The Hack Computer and Machine Language ( I think 0.6 0.7 and 0.8 is enough)
  asmCommand +="@TRUE" + to_string(LabelCounter) + "\n"; // complains but work
  asmCommand +="D;"+ jumpCommand + "\n"; // jump if equal (D=0)
  
  asmCommand += "@SP\n";
  asmCommand += "A=M\n";
  asmCommand += "M=0\n"; // if no jump we end up here
  asmCommand +="@END" + to_string(LabelCounter) + "\n";
  asmCommand +="0;JMP\n"; // jump to END

  asmCommand += "(TRUE"+ to_string(LabelCounter) + ")\n";
  asmCommand += "@SP\n";
  asmCommand += "A=M\n";
  asmCommand += "M=-1\n"; // shouldnt true genereate +1?

  asmCommand +="(END" + to_string(LabelCounter) + ")\n";

  asmCommand +="@SP\n"; 
  asmCommand +="M=M+1\n"; // add SP
  asmCode << asmCommand;
  LabelCounter++;

}

void addNegNot(string trimedVMCode,string negNot,ofstream &asmCode){
  string asmCommand ="";
  asmCommand += "//"+negNot+"\n";
  asmCommand +="@SP\n"; 
  asmCommand +="M=M-1\n";
  asmCommand +="A=M\n";
  
  if(negNot == "not"){
    asmCommand +="M=!M\n";   
  }
  if(negNot =="neg"){
    asmCommand +="M=-M\n";
  }

  asmCommand +="@SP\n"; 
  asmCommand +="M=M+1\n";
  
  asmCode << asmCommand;

}

void addLabel(string trimedVMCode, ofstream &asmCode){
  string asmCommand ="//" + trimedVMCode + "\n";
  vector <string> trimedVMCodevector =  splitStringtoVector(trimedVMCode);
  asmCommand += "(" + trimedVMCodevector[1] + ")\n";
  asmCode << asmCommand;

}

void addIfGoto(string trimedVMCode, ofstream &asmCode){
  // i thnk if-goto takes one argument from the stack and check if positive.
  // if positive jump to label. else do nothing
  string asmCommand ="//" + trimedVMCode + "\n";
  vector <string> trimedVMCodevector =  splitStringtoVector(trimedVMCode);
  asmCommand +="@SP\n"; 
  asmCommand +="M=M-1\n"; // one step back
  asmCommand +="A=M\n"; // get adress from SP
  asmCommand +="D=M\n"; // save value to D
  asmCommand +="@" + trimedVMCodevector[1] +"\n";
  asmCommand +="D;JNE\n"; // changed frrom D;JGT if D > 0 jump to label
 // should this jump happen if D !=0? since true is -1
 // both JGT and JNE pass FibonacciSeries
 // http://nand2tetris-questions-and-answers-forum.32033.n3.nabble.com/How-can-I-choose-a-jump-command-for-if-goto-td4029887.html

  asmCode << asmCommand;
}

void addGoto(string trimedVMCode, ofstream &asmCode){
  string asmCommand ="//" + trimedVMCode + "\n";
  vector <string> trimedVMCodevector =  splitStringtoVector(trimedVMCode);
  asmCommand +="@" + trimedVMCodevector[1] +"\n";
  asmCommand +="D;JMP\n"; // if D >0 jump to label


  asmCode << asmCommand;
}
void addFunction(string trimedVMCode, ofstream &asmCode){
  string asmCommand ="//" + trimedVMCode + "\n";
  vector <string> trimedVMCodevector =  splitStringtoVector(trimedVMCode);
  int nrOfArgs = stoi(trimedVMCodevector[2]);
  
  asmCommand += "("+ trimedVMCodevector[1]+")\n";
  for(int i=0;i<nrOfArgs;i++){
    asmCommand += "@R0\n"; // get the number 
    asmCommand += "D=A\n"; // save the number
    asmCommand += "@SP\n"; // SP points to
    asmCommand += "A=M\n"; // this address
    asmCommand += "M=D\n"; // and that address gets the number
    asmCommand += "@SP\n"; //back to SP
    asmCommand += "M=M+1\n"; // add one
  }
  // generate labeol of function
  // push 0 (nr of argument times)

  asmCode << asmCommand;
}

void addReturn(string trimedVMCode, ofstream &asmCode){
  // handling return
  // 2-6 time 21:20

  // i passed SimpleFunction, but im not sure...
  // passed NestedCall alltso, but still not sure.
  // some source sys R11-R12 is forbidden
  // http://nand2tetris-questions-and-answers-forum.32033.n3.nabble.com/NestedCall-FibonacciElement-StaticsTest-Fail-but-in-different-ways-td4032529.html
  // staticsTest passed aswell
  // but im still not sure (=

   vector <string> trimedVMCodevector =  splitStringtoVector(trimedVMCode); 
  string asmCommand ="//" + trimedVMCode + "\n";

  asmCommand += "@LCL\n"; 
  asmCommand += "D=M\n";
  asmCommand += "@R11\n";
  asmCommand += "M=D\n"; 
  //Saves local in R11, why here? I dont know, saves RAM
  
  asmCommand += "@R5\n";
  asmCommand += "A=D-A\n"; // just changed this from D=D-A and now it works!
  asmCommand += "D=M\n";
  asmCommand += "@R12\n";
  asmCommand += "M=D\n";
  // saves return adress to R12, why here? i dont know, saves RAM

  asmCommand += "@SP\n";
  asmCommand += "AM=M-1\n";
  asmCommand += "D=M\n";
  // pop to D

  asmCommand += "@ARG\n";
  asmCommand += "A=M\n";
  asmCommand += "M=D\n";
  // pop to *arg

  asmCommand += "@ARG\n";
  asmCommand += "D=M\n";
  asmCommand += "D=D+1\n";
  asmCommand += "@SP\n";
  asmCommand += "M=D\n"; 
  // SP = ARG +1

  asmCommand += "@R11\n";
  asmCommand += "D=M-1\n"; // used to be AM=M-1
  asmCommand += "AM=D\n";
  asmCommand += "D=M\n";
  // get content of endFrame-1 and -1 to R11 

  asmCommand += "@THAT\n";
  asmCommand += "M=D\n";
  // THAT = *(endFrame -1)

  asmCommand += "@R11\n";
  asmCommand += "AM=M-1\n";
  asmCommand += "D=M\n";
  // get content of endFrame-2 and -1 to R11 

  asmCommand += "@THIS\n";
  asmCommand += "M=D\n";
  // THIS = *(endFrame -1)

  asmCommand += "@R11\n";
  asmCommand += "AM=M-1\n";
  asmCommand += "D=M\n";
  // get content of endFrame-3 and -1 to R11 

  asmCommand += "@ARG\n";
  asmCommand += "M=D\n";
  // ARG = *(endFrame -1)

  asmCommand += "@R11\n";
  asmCommand += "AM=M-1\n";
  asmCommand += "D=M\n";
  // get content of endFrame-4 and -1 to R11 

  asmCommand += "@LCL\n";
  asmCommand += "M=D\n";
  // THAT = *(endFrame -1)

  asmCommand += "@R12\n";
  asmCommand += "A=M\n";
  asmCommand += "0;JMP\n";
  // jump to return adress



 asmCode << asmCommand;
}

void addCall(string trimedVMCode, ofstream &asmCode){
   vector <string> trimedVMCodevector =  splitStringtoVector(trimedVMCode); 
  string asmCommand ="//" + trimedVMCode + "\n";
  // 2-6 time 12:31
  // this shouldnt be to hard, it is the reveres return.
  string returnLabel = "RETURNLABEL" + to_string(LabelCounter);
  LabelCounter++;

  asmCommand += "@"+returnLabel+"\n";
  asmCommand += "D=A\n";
  asmCommand += "@SP\n";
  asmCommand += "A=M\n";
  asmCommand += "M=D\n";
  asmCommand += "@SP\n";
  asmCommand += "M=M+1\n";
  // push return adress

  asmCommand += "@LCL\n";
  asmCommand += "D=M\n";
  asmCommand += "@SP\n";
  asmCommand += "A=M\n";
  asmCommand += "M=D\n";
  asmCommand += "@SP\n";
  asmCommand += "M=M+1\n";
  // push local 

  asmCommand += "@ARG\n";
  asmCommand += "D=M\n";
  asmCommand += "@SP\n";
  asmCommand += "A=M\n";
  asmCommand += "M=D\n";
  asmCommand += "@SP\n";
  asmCommand += "M=M+1\n";
  // push ARG 

  asmCommand += "@THIS\n";
  asmCommand += "D=M\n";
  asmCommand += "@SP\n";
  asmCommand += "A=M\n";
  asmCommand += "M=D\n";
  asmCommand += "@SP\n";
  asmCommand += "M=M+1\n";
  // push THIS

  asmCommand += "@THAT\n";
  asmCommand += "D=M\n";
  asmCommand += "@SP\n";
  asmCommand += "A=M\n";
  asmCommand += "M=D\n";
  asmCommand += "@SP\n";
  asmCommand += "M=M+1\n";
  // push THAT

  asmCommand += "@SP\n";
  asmCommand += "D=M\n";
  asmCommand += "@R5\n";
  asmCommand += "D=D-A\n";
  asmCommand += "@R" + trimedVMCodevector[2] +"\n";
  asmCommand += "D=D-A\n";
  asmCommand += "@ARG\n";
  asmCommand += "M=D\n";
  // ARG = SP - 5 - nARGs

  asmCommand += "@SP\n";
  asmCommand += "D=M\n";
  asmCommand += "@LCL\n";
  asmCommand += "M=D\n";
  // local = SP 

  asmCommand += "@"+ trimedVMCodevector[1] +"\n";
  asmCommand += "0;JMP\n";
  // goto functionName

  asmCommand += "("+ returnLabel + ")\n";
  //(returnAddress)

  asmCode << asmCommand;
}

void addInit(ofstream &asmCode ){
  string asmCommand ="//Init\n";
  asmCommand +="@256\n";
  asmCommand +="D=A\n";
  asmCommand +="@SP\n";
  asmCommand +="M=D\n"; 
  // set SP to 256
  cout <<"6"<<endl;
  string test = "function Sys.init 0";
  asmCode << asmCommand;
  addCall(test,asmCode);
  cout <<"7"<<endl;

 
}

vector<string> splitStringtoVector (string theString){

  stringstream ss(theString);
  istream_iterator <string> begin(ss);
  istream_iterator <string> end;
  vector<string> theVector(begin, end);

  return theVector;
}