#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>
#include<cstring>
#include<map>
#define sys ;
using namespace std ;
//FUNCTIONS USED 
int strtoint(string);   
int bin_strtoint(string );                    //CONVERTS STRING TO INTEGER  
string trim(char *);                           // DELIMITS STRING BY SPACES             
int evaluate(string s,int lc);                //EVALUATING EXPRESSION 
char relocate(string s);                      //CALULATES RELOCATION VALUE
int dlength_dc(string );                      //CALCULATES LENGTH FROM DC FORMAT 
int dlength_ds(string );                      //CALCULATE LENGTH FROM DS FORMAT
string get_prestr(string ,char,int);          //RETURNS THE STRING BEFORE THE GIVEN CHARACTER  
bool stringcmp(char*,char*);                  //COMPARES STRINGS 
int ret_mod(int x);                             //RETURNS MODULUS 
string get_poststr(string s1,char s,int k);       //RETURNS STRING AFTER THE CHARACTER S 
int isint(string s);                            //CHECKS WHTETHER A STRING IS AN INTEGER OR NOT 
void delim(char str[][20],string s);
void printfile(const char *s);

//--------------------------------------------------------------------------------------
class statement{
      int statenum;
      public:
              statement() {(statenum=0);}
             statement(int s) {(statenum=s);}
            void  getstatenum(int s) {(statenum = s); }
             int get() { return statenum; }
      };
bool operator < (statement a, statement b)
{
     return (a.get()<b.get()) ;
 }
//------------------------------------------------------------------------------------------
//CLASS FOR STORING THE ASSEMBLY LANGUAGE ATTRIBUTES
class alp_attr    
{
      string labelfield;
      string operationfield;
      string operandfield;
      public:
        alp_attr() { (labelfield="");(operationfield="");(operandfield="");}      
        alp_attr(string s1,string s2,string s3) { (labelfield=s1);(operationfield=s2);(operandfield=s3); }     
     void  putlabelfield(string s) {(labelfield=s); }
      string getlabelfield() { return labelfield; }
     void putoperationfield(string s) {(operationfield=s); }
      string getoperationfield() { return operationfield; }             
    void putoperandfield(string s) {(operandfield=s); }
       string getoperandfield() { return operandfield; }         
      };    
//------------------------------------------------------------------------------------------      
//FUNCTION FOR GETTING THE MAP FROM THE FILE 
map <int,alp_attr>  getmap_asslang(const char * s)
 {
      char word[200];string s2, s3;int y;int x;
      map<int,alp_attr> alp_map;
      statement stat1;alp_attr attr1;ifstream alc;    
      char str[100];int i=0,j=0;    
      alc.open(s,ios::in);
      if(!alc) {cout<<"Cannot open Assembly Language Source Program\n";exit(0);}
      alc.getline(word,200);
      while(!(alc.eof())) 
      {  
	//cout<<word<<endl;
           string s1(word); 
          i=0;j=0;
           while(i<4)
           {
           s2 = (get_prestr(s1,':',j));
           j+=s2.length()+1;
           //s2 = trim(s2);
           if(i==0) {y = strtoint(s2);}
           if(i==1) attr1.putlabelfield(s2);
           if(i==2) {attr1.putoperationfield(s2);}
           if(i==3) {attr1.putoperandfield(s2);}
           i++;
          }
      alp_map.insert(pair<int,alp_attr>(y,attr1));
      alc.getline(word,200);
      }
          alc.close(); 
          //cout<<"length"<<alp_map.size();cin>>x; 
          return alp_map;             
 }
 //------------------------------------------------------------------------------------------
//CLASS FOR STORING THE MACHINE OPERATION TABLE ATTRIBUTES 
class mot_attr{
      public:
      string hexadecimal;
      string ins_len;
      string format;

        mot_attr() { (hexadecimal=""); (ins_len="");(format="");}      
        mot_attr(string s1,string s2,string s3) {(hexadecimal=s1);(ins_len=s2);(format=s3); }     
     void  puthexadecimal(string s) {(hexadecimal=s); }
      string gethexadecimal() { return hexadecimal; }
     void putins_len(string s) {(ins_len=s); }
      string getins_len() { return ins_len; }             
    void putformat(string s) {(format=s); }
       string getformat() { return format; }      
      };
 //------------------------------------------------------------------------------------------     
//FUNCTION THAT STORES MOT MAP FROM FILE 
map <string,mot_attr>  getmap_mot(char * s)
 {
      string s1,s3,s2;char word[100];
      map<string,mot_attr> mot_map;
      mot_attr attr1;ifstream imot;    
      char str[100];int i=0,j=0;    
      imot.open(s,ios::in);
      if(!imot) {cout<<"Cannot open Machine Operation Table Source File\n";exit(0);}
      imot.getline(word,200);
      while(!(imot.eof())) 
      {  
           i=0;j=0;s1 =word;
         // s1=trim(word);
          
          while(i<4)
           {
          
          s2 = get_prestr(s1,':',j);
           j+=s2.length()+1;
          
           if(i==0) s3=(s2);
           if(i==1) attr1.puthexadecimal(s2);
           if(i==2) attr1.putins_len(s2);
           if(i==3) attr1.putformat(s2);
           i++;
      }
      mot_map.insert(pair<string,mot_attr>(s3,attr1));    
      imot.getline(word,200);  //cout<<word;sys
      }
          imot.close();//cout<<"machineoperation table mapped from file";sys 
          return mot_map;             
           }   
//------------------------------------------------------------------------------------------
//CLASS THAT STORES THE PSEUDO OPERATIONS ATTRIBUTES
    class pot_attr{
     string address;
      public :
        pot_attr() { (address=""); }      
        pot_attr(string s1) { (address=s1);}     
        void  putaddress(string s) {(address=s); }
      string get() { return address; }
      };    
//------------------------------------------------------------------------------------------
//FUNCTION FOR STORING THE POT FROM THE FILE                                    
map <string ,pot_attr>  getmap_pot(char * s)
 {
      char word[200];string s2;
      map<string ,pot_attr> pot_map;
      string  stat1;pot_attr attr1;ifstream ipot;    
      char str[100];int i=0,j=0;    
      ipot.open(s,ios::in);
      if(!ipot) {cout<<"Cannot open Psuedo-Operation Table Source File\n";exit(0);}
      ipot.getline(word,200);
      while(!(ipot.eof())) 
      {   //cout<<"ravi";sys                        
          // cout<<word<<endl;system("pause");
           i=0;j=0;string s1(word); 
        //  s1=trim(word);
          //cout<<"after trimming \n"<<s1<<strlen(s1)<<endl;system("pause");
          while(i<4)
           {
           //cout<<"ravi"<<endl;sys
           s2 = get_prestr(s1,':',j);
            j+=s2.length()+1;
          // cout<<s2;sys
           if(i==0) stat1=(s2);
           if(i==1) attr1.putaddress(s2);
           i++;
      }
      pot_map.insert(pair <string ,pot_attr>(stat1,attr1));  
      ipot.getline(word,200);  //cout<<word;sys
      }
          ipot.close();  //cout<<"pseudo operation table mapped from file";sys   
          return pot_map;          
 }
//------------------------------------------------------------------------------------------
//CLASS THAT STORES THE SYMBOL TABLE AND ATTRIBUTES 
 class symtab_attr{
       public:
      int sym_value;
      int sym_len;
      char sym_relocation;
      
        symtab_attr() { (sym_value=0); (sym_len=0);(sym_relocation='\0');}      
        symtab_attr(int s1,int s2,char s3) { (sym_value=s1);(sym_len=s2);(sym_relocation=s3); }     
     void  putsym_value(int s) {(sym_value=s); }
      int getsym_value() { return sym_value; }
     void putsym_len(int s) {(sym_len=s); }
      int getsym_len() { return sym_len; }             
    void putsym_relocation(char s) {(sym_relocation=s); }
       char getsym_relocation() { return sym_relocation; }      
      };  
//------------------------------------------------------------------------------------------
//CLASS THAT STORES THE LITERAL TABLE AND ATTRIBUTES 
 class littab_attr{
       public :
      int lit_value;
      int lit_len;
      char lit_relocation;
      littab_attr() { (lit_value=0); (lit_len=0);(lit_relocation='\0');}      
        littab_attr(int s1,int s2,char s3) { (lit_value=s1);(lit_len=s2);(lit_relocation=s3); }     
     void  putlit_value(int s) {(lit_value=s); }
      int getlit_value() { return lit_value; }
     void putlit_len(int s) {(lit_len=s); }
      int getlit_len() { return lit_len; }             
    void putlit_relocation(char s) {(lit_relocation=s); }
       char getlit_relocation() { return lit_relocation; }      
      };
                 
void insert_into_st(map <string,symtab_attr> * symtab_map,string lf,int lc,int len ,char relocation);
void insert_into_lt(map <string,littab_attr> * littab_map,string lf,int lc,int len ,char relocation);   
//------------------------------------------------------------------------------------------
int strtoint(string s)
{
 int a,len,p=1,sum=0,i=0 ;int k = 0;
 if (s[0] =='-') k = 1;   // for negative integers we go up to the 2nd location in the string  
 len=(s).length();i=len-1;
 while(i>=k)
 {
 a=int(s[i])-48;
 sum = sum + a*p;
 p=p*10; i--;
 }                  
  if (s[0] =='-') return (-sum);
  else  return sum;              
}
//------------------------------------------------------------------------------------------
int bin_strtoint(string s)
{
 int a,len,p=1,sum=0,i=0 ;
 len=(s).length();i=len-1;
 while(i>=0)
 {
 a=int(s[i])-48;
 sum = sum + a*p;
 p=p*2; i--;
 }                  
  return sum;              
}
//------------------------------------------------------------------------------------------
int evaluate(string s,int lc)
{
    if((s=="*")) return lc;
    if(s[0]>48 && s[0]<58) return(strtoint(s)) ;    
}
//------------------------------------------------------------------------------------------
char relocate(string s)
{
 if((s=="*")) return ('R');
    if(s[0]>48 && s[0]<58) return('A') ;
 }
//------------------------------------------------------------------------------------------
void get_substr(string s,string s1,char a)
{
s1[0]='\0';     
int i,j,len=(s).length();
for(i=0;i<len;i++)        if(s[i]=='a') break ;
i++ ;
j=0;          
for(;i<len;i++)            s1[j]=s[i] ;                  
}
//------------------------------------------------------------------------------------------
int dlength_ds(string s)
{
int num;

string s2;
s2 = get_prestr(s,'F',0);
num = strtoint(s2);

return num;
}
//------------------------------------------------------------------------------------------
int dlength_dc(string s)
{

int i,n=1;
for(i=0;i<(s).length();i++)
{
if(s[i]==',') n++;
}
if(n>1) return (n-1);
return(n);
}
//------------------------------------------------------------------------------------------
string trim(char *s1)
 {
  char s2[100];     
 //cout<<"in trim"<<endl;sys
  int i=0,j=0 ;int len = strlen(s1);
  for(i=0;i<len;i++)
  {
  s2[j]=s1[i];
   if(s1[i]==' ' && s1[i+1]==' '&& int(s1[i-1])>47 && int(s1[i-1])<58 && i>=1){s2[j]=' ';s2[j+1]=' ';s2[j+2]=' ';i+=1;j=j+2;}
      while(s1[i]==' ' && s1[i+1]==' ' && i<len-2) i++; 
     j++ ;
   }  
   if(s1[len-1]!=' ') s2[j++]=' ';
   s2[j]='\0'; 
   string s(s2);//cout<<"s"<<s;
   return s;
// cout<<"end trim"<<endl;
    }
//------------------------------------------------------------------------------------------    
string get_prestr(string s1,char s,int k)
{
       
 // cout<<"in prestr\n"<<s1;
  char s2[100];      
 int i=k,j=0;
 if(s1[k]==' ') { s2[0]=' ';s2[1]='\0'; string s3(s2); return (s3);}
 while(s1[i]!=s && i < (s1).length()) s2[j++]=s1[i++] ;
 s2[j++]='\0';
 string s3(s2);
// cout<<"prestr\n"<<s3;
 return (s3);
}
//------------------------------------------------------------------------------------------
string get_poststr(string s1,char s,int k)
{
       
 // cout<<"in prestr\n"<<s1;
  char s2[100];      
 int i=k,j=0;
 if(s1[k]==' ' || s1[k]=='\0' ) { s2[0]='\0'; string s3(s2); return (s3);}
 while(s1[i]!=s && i < (s1).length()) i++ ;
 if(s1[i]==s && s1[i+1]=='\''){
 while(i < (s1).length()) s2[j++]=s1[i++];
 }
 //while(i < (s1).length()) s2[j++]=s1[i++];
 s2[j++]='\0';
 string s3(s2);
// cout<<"prestr\n"<<s3;
 return (s3);
}
//------------------------------------------------------------------------------------------
string getpoststr(string s1,char s);
string getpoststr(string s1,char s)
{
       
 char s2[100];
 int i=0,j=0;
 if(s1[i]==' ' || s1[i]=='\0' ) { s2[0]='\0'; string s3(s2); return (s3);}
 while(s1[i]!=s && i < (s1).length()) i++ ;
 if(s1[i]==s)
 {
              
 while(i < (s1).length()) {i++;s2[j++]=s1[i];}
 }
 //while(i < (s1).length()) s2[j++]=s1[i++];
 s2[j++]='\0';
 string s3(s2);
// cout<<"prestr\n"<<s3;
 return (s3);
}
//------------------------------------------------------------------------------------------
int ret_mod(int x)
{
    if(x>=0) return x;
    else return -x;
}
//------------------------------------------------------------------------------------------
int isint(string s)
{
     if(s[0]>47 && s[0]<58) return 1;
     else return 0;
     }
 //------------------------------------------------------------------------------------------    void delim(char str[][20],string s)
{

int count =0;int len= s.length();int i,j;
i=1;j=0;
while(count<len)
	{
          while(s[count]!=':' && count<len)	{str[i][j] = s[count];count ++;j++;}
          str[i][j]='\0';i++;count++;j=0;
	}
}
//------------------------------------------------------------------------------------------    
void printfile(const char * s)
 {
	
      char word[200];  
      ifstream alc;      
      alc.open(s,ios::in);
      if(!alc) {cout<<"Cannot open "<<s<<" File \n";exit(0);}
      alc.getline(word,200);
      while(!(alc.eof())) 
      {  
	cout<<word<<endl;alc.getline(word,200);	
      }
      
}
