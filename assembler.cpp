#include "header_loader.h"
int main()
{
    system("clear");
    cout<<"The assembled code is as follows :\n";
    char s11[] ="pot.txt";char s12[] ="mot.txt";const char s13[] ="load1.txt";const char s10[16] = "symboltable.txt";
    ofstream out;out.open("symboltable.txt" , ios :: out);
    ofstream lit_out;lit_out.open("literaltable1.txt" , ios :: out );
    ofstream lit_out2;lit_out2.open("literaltable2.txt" , ios :: out);
    ofstream bt_out;bt_out.open("basetable.txt" , ios :: out);
    ofstream mc_out;mc_out.open("machinecode.txt" , ios :: out );
    ofstream lc_out;lc_out.open("loadedcode.txt" , ios :: out ); 
    ofstream out_card("card2.txt",ios::out);
    ifstream in_ASSEMBLY("load1.txt");    
    int ins_len,sym_val,val,lit_length,lit_val,ltorg_pos,sym_length,k,ar_len,index_reg,base_reg,ltorg_flag=0;int lc = 0,i=0,len=0;int count1=0,lit_size=0;int num_numbers ,INDEX =0,j,add_load;
    int IPLA =100 ,PLA,LENGTH[100],SLENGTH,EXADDR,ID=0,PR_NUM=1; PLA = IPLA; int LESA[100],SYM_ADDR,VALUE,ADDR,add_val,CONTENT;int add;LENGTH[0]=0; int LOC,x;
    char lit_relocation,sym_relocation ;int equ_flag;
    string str,literal,literal1,str_base_reg;string alp_labelfield;string ADDR_CONSTANT,alp_operationfield,alp_operandfield,prestring1,prestring2,SYMBOL;char word[200];char line[20];char STR[10][20];
    //GETTING ASSEMBLY , PSEUDO -OP TABLE , MACHINE OP TABLE MAPS FROM FILES 
    map <int,alp_attr> alp_map;    alp_map = getmap_asslang(s13);
    map <string,alp_attr> :: iterator p;
    map <string,mot_attr> mot_map;          mot_map = getmap_mot(s12);
    map <string,mot_attr> :: iterator itr_mot;
    map <string,pot_attr> pot_map;      pot_map = getmap_pot(s11);
    map <string,int> map_ID; map <string,int> :: iterator itr_ID;
    //----------------------------------------------------------------------
    map <string,pot_attr> :: iterator itr_pot;
    map <string,symtab_attr>  symtab_map[2];
    map <string,symtab_attr>  :: iterator itr_st;
    map <string,littab_attr>  littab_map;
    map <string,littab_attr>  :: iterator itr_lt;
    map <int,alp_attr> :: iterator itr_alp;
    map <int,int> base_table_map;
    map <int,int> :: iterator itr_btm;
    map <int,int> :: iterator smallest_itr_btm;
    map <string,int> GEST_map;
    map <string,int> :: iterator itr_GEST;
    //PASS 1 BEGINS
   in_ASSEMBLY.getline(line,100);
    while(!(in_ASSEMBLY.eof()))                                 //ITERATING THROUGH THE ASSEMBLY LANGUAGE MAP
    {
	 delim(STR,line); equ_flag=0;
	  alp_labelfield = STR[2];              //GETTING ATTRIBUTES FROM ASSEMBLY MAP           
        alp_operationfield = STR[3]; 
 	alp_operandfield = STR[4];
//          if(alp_operationfield=="ENTRY" || alp_operationfield == "EXTRN") goto READ ;
          if((alp_operationfield=="LTORG"))   {len=lit_size*4;ltorg_pos = lc;ltorg_flag=1;}  //LOCATING LTORG'S POSITION
          itr_pot = pot_map.find(alp_operationfield);              //SEARCHING THE OPERATION FIELD IN THE PSEUDO OP - TABLE 
           if(itr_pot!=pot_map.end())                               // operation field is a pseudo - operation 
              {
              if((alp_operationfield=="START"))                     //DEFINING OPERATIONS FOR THE VARIOUS PSEUDO - OPS 
              {lc=0;len = 0;}
              if((alp_operationfield=="USING")){len=0;}
              if((alp_operationfield=="DC"))
              {
		
              len=dlength_dc(alp_operandfield);len = len * 4;literal = getpoststr(alp_operandfield,'=');//L <- DLENGTH_DC
                 if(literal[literal.length()-1]==',')
                 { 
                    string str(literal);str=getpoststr(str,'\'');  string str1 ; int ar[100];                              
                    for(j=0;str!="";j++)                 //STORING THE ARRAY
                    {
                                       
 					str1  = get_prestr(str,',',0);
                                        ar[j] = strtoint(str1) ;
                                        str = getpoststr(str,',');
					num_numbers++;                                          
		   }                                                              
                  }
      
             
              }
              if((alp_operationfield=="DS"))
              {
                  //L <- DLENGTH_DS                          
              len=dlength_ds(alp_operandfield);ar_len=len;
              len = len * 4;literal = get_poststr(alp_operandfield,'F',0);
              if(literal!="")
                {
                  lit_val=4;lit_length=4;lit_relocation = 'R';
                  littab_attr s2(lit_val,lit_length,lit_relocation);
                  lit_out<<literal<<":"<<lit_val<<":"<<lit_length<<":"<<lit_relocation<<":"<<endl;
                }
              }
              if((alp_operationfield=="EQU"))
              {
                   len = 0;
                   sym_val=evaluate(alp_operandfield,lc);               //CALCULATING VALUE OF THE EXPRESSION 
                   sym_relocation=relocate(alp_operandfield);       //CALCULATING WHETHER THE SYMBOL IS RELOCATABLE / ABSOLUTE    
                   sym_length=1;
                
                  symtab_attr s2(sym_val,sym_length,sym_relocation);
                  symtab_map[PR_NUM].insert(pair<string,symtab_attr>(alp_labelfield,s2));
                  equ_flag = 1 ;
			
                  out<<alp_labelfield<<":"<<sym_val<<":"<<sym_length<<":"<<sym_relocation<<endl; 
                  //STREAMING THE SYMBOL ALONG WITH ITS ATTRIBUTES TO THE SYMBOL TABLE 
              }
              //ASSIGNING VALUE TO SYMBOL FIELD  
              if((alp_operationfield=="END"))
          {
		int x;LENGTH[PR_NUM++] = lc ;             
              if(ltorg_flag==0) ltorg_pos=lc;len =0;
           //        UPDATING THE LITERAL TABLE WHEN END IS ENCOUNTERED USING LTORG OR END POSITION 
                  { 
      ltorg_pos=ltorg_pos-4;
      string str2,str1,pstr;
      lit_out.close();
      ifstream lit_out;lit_out.open("literaltable1.txt" , ios :: in );
      if(!lit_out) {cout<<"Cannot open First Literal Table \n";exit(0);}
      lit_out.getline(word,200);
      while(!(lit_out.eof())) 
      {   
          str1=get_prestr(word,':',0);literal = str1;         //GETTING LITERAL'S ATTRIBUTES
          pstr=getpoststr(word,':');                                
          str2=get_prestr(pstr,':',0);
          lit_out.getline(word,200);  
          lit_val=strtoint(str2)+ltorg_pos;ltorg_pos=lit_val;
          lit_length=4;lit_relocation = 'R';
          littab_attr attrs2(lit_val,lit_length,lit_relocation);
          (littab_map).insert(pair<string,littab_attr>(str1,attrs2));
          lit_out2<<literal<<":"<<lit_val<<":"<<lit_length<<":"<<lit_relocation<<":"<<endl;
          
    }
          lit_out.close();                
                  }
              }
          } 
           else
               {
                                                   
                 itr_mot = mot_map.find(alp_operationfield);   //SEARCHING FOR THE OPERATION IN THE MACHINE OPERATION TABLE
                
                  if(itr_mot!=mot_map.end())             //IF OPERATION IS A MACHINE OP
                    {
                                   
                                    ins_len=  len=bin_strtoint(itr_mot->second.getins_len())*2;     //L <- LENGTH
                                      
                    }
		
                  // PROCESSING LITERAL ELEMENTS
                 literal = getpoststr(alp_operandfield,'=');                
                 lit_val=4;lit_length=4;lit_relocation = 'R' ;
                 				 	//IF LITERAL PRESENT ENTERING INTO THE LITERAL MAP    (LTSTO)
                 if((literal!="") && (literal[literal.length()-1]!=','))  
                 {
                 littab_attr s2(lit_val,lit_length,lit_relocation);
                 lit_out<<literal<<":"<<lit_val<<":"<<lit_length<<":"<<lit_relocation<<":"<<endl;lit_size++;
                 }  

               }//END ELSE 
                 
                  //IF SYMBOL EXISTS IN THE LABEL FIELD(STSTO)                                                                                                                                     
                  if(((alp_labelfield != "")) && equ_flag==0 && (alp_labelfield != " ") && (alp_operationfield != "ENTRY"))  
                  {
                     sym_val=lc;sym_length=ins_len;
                     sym_relocation='R'; 
                     symtab_attr s2(sym_val,sym_length,sym_relocation);
                     symtab_map[PR_NUM].insert(pair<string,symtab_attr>(alp_labelfield,s2));
                     out<<alp_labelfield<<":"<<sym_val<<":"<<sym_length<<":"<<sym_relocation<<":\n"; 
                   
                  }

                 count1++;lc=lc+len; //INCREASING THE LC BY LEN 
         	 in_ASSEMBLY.getline(line,100);
       }//END WHILE
     out.close();   
     lit_out.close();
     lit_out2.close();
	in_ASSEMBLY.close();
  //PASS 1 ENDS
  //PASS 2 BEGINS
   in_ASSEMBLY.open("load1.txt");   PR_NUM = 1;

  int LC=0,count=1;int reg,reg_value,symbol_value1,symbol_value2, smallest_offset,offset; string prestring,poststring;int flag=0, array_index=0;
  string str_array_index;
     in_ASSEMBLY.getline(line,100);
    while(!(in_ASSEMBLY.eof()))                                  //ITERATING THROUGH THE ASSEMBLY LANGUAGE MAP
    {
   	delim(STR,line); 
	  alp_labelfield = STR[2];              //GETTING ATTRIBUTES FROM ASSEMBLY MAP           
        alp_operationfield = STR[3]; 
 	alp_operandfield = STR[4]; 
		 //    if(alp_operationfield=="END") {}
           if(alp_operationfield=="START") {
		ID = 1;map_ID.insert(pair <string,int> (alp_labelfield,ID));
		out_card<<"E:"<<STR[1]<<":"<<alp_labelfield<<":"<<"SD:"<<ID<<":"<<LC<<":"<<LENGTH[PR_NUM]<<":"<<endl;		
		num_numbers =0;ar_len =0;
					}																
           else if(alp_operationfield=="ENTRY") {
		
		str = alp_labelfield;		
		SYMBOL = get_prestr(str,',',0) ;		
		while(SYMBOL.length() != 0)
	{		str = getpoststr(str,',');
			itr_st = symtab_map[PR_NUM].find(SYMBOL);
			if(itr_st != symtab_map[PR_NUM].end()) { LOC = itr_st->second.sym_value ;} else cout<<"SYMBOL "<<SYMBOL<<" NOT PRESENT IN THE PROGRAM "<<endl;
			out_card<<"E:"<<STR[1]<<":"<<SYMBOL<<":"<<"LD"<<"::"<<LOC<<"::"<<endl;		
			SYMBOL = get_prestr(str,',',0) ;			
	}
		
	}		
		else   if(alp_operationfield=="EXTRN") {
		str = alp_labelfield;		
		SYMBOL = get_prestr(str,',',0) ;
		while(SYMBOL.length() != 0)
		{	
		str = getpoststr(str,',');
		ID++;map_ID.insert(pair <string,int> (SYMBOL,ID));
		out_card<<"E:"<<STR[1]<<":"<<SYMBOL<<":"<<"ER:"<<ID<<":::"<<endl;		
		SYMBOL = get_prestr(str,',',0) ;
		}		
					}
	
        str_array_index = getpoststr(alp_operandfield,'(');str_array_index = get_prestr(str_array_index,')',0);
        //CHECKING IF THE OPERAND FIELD IS AN ARRAY 
        if(str_array_index!=""){ 
                                 itr_st = symtab_map[PR_NUM].find(str_array_index);  //FOR CALCULATING THE INDEX VALUE IN INDIRET ADDRESSING
                                                     if(itr_st!=symtab_map[PR_NUM].end())
                                                      {
                                                           array_index = (itr_st->second.sym_value);                      
                                                      }
                               }
         else         array_index = 0;
         if(alp_operationfield=="LTORG") //WE PLACE THE LITERAL FIELD WHERE LTORG IS ENCOUNTERED
                {    

                      LC +=lit_size*4; 
                      string str2,str1,pstr;char word[200];
                      mc_out<<count<<":";cout<<count<<" ";
                      ifstream lit_out;lit_out.open("literaltable2.txt" , ios :: in );
      if(!lit_out) {cout<<"Cannot open First Literal Table \n";exit(0);}
      lit_out.getline(word,200);int literal_val;string lit;
      while(!(lit_out.eof())) 
      {   
          str1=get_prestr(word,':',0);literal = str1;
          pstr=getpoststr(word,':');                                
          str2=get_prestr(pstr,':',0);lit_val=strtoint(str2);
          lit_out.getline(word,200); lit=get_prestr(getpoststr(literal,'('),')',0); 
          
          if(lit!="")
          {
           itr_st = symtab_map[PR_NUM].find(lit);
                                                     if(itr_st!=symtab_map[PR_NUM].end())
                                                      {
                                                           literal_val = (itr_st->second.sym_value);                      
                                                           mc_out<<":"<<lit_val<<":"<<literal_val<<endl;
                                                           cout<<"  "<<lit_val<<"  "<<literal_val<<endl;                                                      } 
          						
							 }                                                   
          else  {
		
		mc_out<<":"<<lit_val<<":"<<literal<<endl;
		cout<<"        "<<lit_val<<"   "<<literal<<endl;
 		}
	out_card<<"T"<<":"<<count<<":"<<lit_val<<":"<<literal<<":0:"<<endl;      
      }
          lit_out.close();
                                               
                                               }     
                                    
         itr_pot = pot_map.find(alp_operationfield);              //SEARCHING THE OPERATION FIELD IN THE PSEUDO OP - TABLE 
            if(itr_pot!=pot_map.end())                       //IF OPERATION IS PRESENT IN THE PSEUDO OP TABLE 
            {
                       
                  if(alp_operationfield=="END"){ 
		out_card<<"D"<<":"<<endl;    LC = 0;base_table_map.clear();littab_map.clear();PR_NUM++;
                   //AFTER END ENCOUNTERED WE PUT THE BASE TABLE INTO THE FILE FOR THE BASE TABLE AND EXIT                               
                                                 for(itr_btm = base_table_map.begin();itr_btm != base_table_map.end();itr_btm++)
               {
                           bt_out<<itr_btm->first<<"  "<<itr_btm->second<<endl;
              }    
      bt_out.close();//LENGTH[PR_NUM] = LC - LENGTH[PR_NUM-1];
      }      
                                                            
                if(alp_operationfield=="USING")
                  {                             
                   poststring = getpoststr(alp_operandfield,',');   //POSTSTRING GETS THE SECOND SYMBOL
                    if(isint(poststring))                    {reg = strtoint(poststring);  }          
                    
                    else  
                    {
                                            itr_st = symtab_map[PR_NUM].find(poststring);
                                                     if(itr_st!=symtab_map[PR_NUM].end())
                                                      {
                                                           reg = (itr_st->second.sym_value);  //CALCULATES REG VALUE FROM THE SYMBOL TABLE                     
                                                      };
                    }
                    prestring = get_prestr(alp_operandfield,',',0);    //PRESTRING GETS THE FIRST SYMBOL
                    if(prestring=="*"){reg_value=LC;}
                    else 
                       {
                                                     itr_st = symtab_map[PR_NUM].find(prestring);
                                                     if(itr_st!=symtab_map[PR_NUM].end())
                                                      {
                                                           reg_value = itr_st->second.sym_value;    //CALCULATES THE REG VALUE FROM THE LITERAL TABLE 
                                                      }
                       }       
                                       
                    itr_btm = base_table_map.find(reg);
                    if(itr_btm!=base_table_map.end()) itr_btm->second = reg_value ;
                    else {base_table_map.insert(pair <int,int>(reg,reg_value));} 
                 }
                  if(alp_operationfield=="DROP")              //BASE REGISTER IS DROPPED FROM THE BASE TABLE IF DROP IS ENCOUNTERED  
             {                             
                   poststring = alp_operandfield;
                   if((poststring[0])>47 && (poststring[0])<58)                    reg = strtoint(poststring);
                   else  
                    {
                                            itr_st = symtab_map[PR_NUM].find(poststring);
                                                     if(itr_st!=symtab_map[PR_NUM].end())
                                                      {
                                                           reg = (itr_st->second.sym_value);                      
                                                      };
                    }
                    
                    base_table_map.erase(reg); 
             }
             if(alp_operationfield=="DC")             //DISPLAYING ALL THE CONSTANTS WHEN DC IS ENCOUNTERED
                  {
                     poststring = getpoststr(alp_operandfield,'\'');
                     if(poststring[0]=='\0')    //ADDRESS CONSTANT IS PRESENT 
                          {
                         	int COUNT = 0 ;char ch ; ch ='+';CONTENT = 0;char temp_str[10];int num_addr_constant = 1;int tflag = 1;
                        	poststring = getpoststr(alp_operandfield,'(');
				poststring = get_prestr(poststring,')',0); 
				len = poststring.length();
				while(COUNT<len)
				{
					if(poststring[COUNT] == '+' || poststring[COUNT] == '-' )
					if(!((poststring[COUNT+1])>47 && poststring[COUNT+1]<58)) num_addr_constant ++;
					COUNT++;			
				}
				COUNT = 0;
				while(COUNT<len)
				{
					ADDR_CONSTANT = "";int j =0;
					while(COUNT < len && poststring[COUNT] != '+' && poststring[COUNT] != '-' )
					{temp_str[j++] = poststring[COUNT++];}
					temp_str[j] = '\0';ADDR_CONSTANT = temp_str;
					
					if(isint(ADDR_CONSTANT)) {SYM_ADDR = strtoint(ADDR_CONSTANT);
									}  //IF ADDR CONSTANT IS A CONSTANT INTEGER 
					else
					    {
						itr_st = symtab_map[PR_NUM].find(ADDR_CONSTANT);
                                                     if(itr_st!=symtab_map[PR_NUM].end())
                                                      {
							SYM_ADDR = itr_st->second.sym_value ;                                                  
						        mc_out<<count<<":"<<LC<<":"<<(itr_st->second.sym_value)<<":"<<endl;            
                                 			cout<<count<<" "<<LC<<" "<<(itr_st->second.sym_value)<<endl;            
                                			                        
                                                      }
							else 
						     {
							SYM_ADDR = 0;
							mc_out<<count<<":"<<LC<<":"<<"0"<<":"<<endl;            
                                 			cout<<count<<" "<<LC<<" "<<"0"<<endl;            
                                		     }	
						itr_ID = map_ID.find(ADDR_CONSTANT);
						if(itr_ID != map_ID.end())  ID = itr_ID->second;
						else { ID = 01 ;} 
					    }//END ELSE	
			
				if(ch == '+')	   CONTENT = CONTENT + SYM_ADDR;
				else if(ch == '-') CONTENT = CONTENT - SYM_ADDR;
				if (COUNT <len ) ch = poststring[COUNT++];
		  	          }	//END WHILE
					out_card<<"T"<<":"<<count<<":"<<LC<<":"<<CONTENT<<":"<<num_addr_constant<<":"<<endl;
					COUNT = 0;	ch ='+';
					while(COUNT<len)
				 {
					ADDR_CONSTANT = "";int j =0;
					while(COUNT < len && poststring[COUNT] != '+' && poststring[COUNT] != '-' )
					{temp_str[j++] = poststring[COUNT++];}
					temp_str[j] = '\0';ADDR_CONSTANT = temp_str;
					
					if(isint(ADDR_CONSTANT)) {SYM_ADDR = strtoint(ADDR_CONSTANT);
									}  //IF ADDR CONSTANT IS A CONSTANT INTEGER 
					else
					    {
						itr_st = symtab_map[PR_NUM].find(ADDR_CONSTANT);
                                                     if(itr_st!=symtab_map[PR_NUM].end())
                                                      {
							SYM_ADDR = itr_st->second.sym_value ;                                                  
				                        
                                                      }
							else 
						     {
							SYM_ADDR = 0;
						     }	
						itr_ID = map_ID.find(ADDR_CONSTANT);
						if(itr_ID != map_ID.end())  ID = itr_ID->second;
						else { ID = 01 ;} 
					    }//END ELSE	
			
		  	      	if(!(isint(ADDR_CONSTANT))) out_card<<"R"<<":"<<count<<":"<<ID<<":"<<4<<":"<<ch<<":"<<LC<<":"<<endl;
				if (COUNT <len ) ch = poststring[COUNT++];
				// IF POSTSTRING
				}                   			
					LC = LC + 4 ;                        
			}			
			else
                          {			 //ADDRESS CONSTANT IS NOT PRESENT 

                                poststring = get_prestr(poststring,'\'',0);
                                 prestring = get_prestr(poststring,',',0);
                                poststring = getpoststr(poststring,',');
                                mc_out<<count<<":"<<LC<<":"<<prestring<<":"<<endl;            
                                cout<<count<<" "<<LC<<" "<<prestring<<endl;  
				out_card<<"T"<<":"<<count<<":"<<LC<<":"<<prestring<<":"<<0<<":"<<endl;          
                                LC  = LC + 4;  
                             while(poststring!="")
                                {
                                prestring = get_prestr(poststring,',',0);
                                poststring = getpoststr(poststring,',');
                                mc_out<<count<<":"<<LC<<":"<<prestring<<":"<<endl;            
                                cout<<count<<" "<<LC<<" "<<prestring<<endl;   
				out_card<<"T"<<":"<<count<<":"<<LC<<":"<<prestring<<":"<<0<<":"<<endl;          
                               LC  = LC + 4;
                                }//END WHILE
			 }//END ELSE
                                     
                 } //END IF  DC
                if(alp_operationfield=="DS")         //SAVES WORD SPACE EQUAL TO OPERAND'S VALUE 
                  {
                                ar_len =0 ; 
                                poststring = get_prestr(alp_operandfield,'F',0); 
                                ar_len = strtoint(poststring);
                                ar_len = strtoint(poststring);
                                for(k=0;k<ar_len;k++)
                                {   
                                mc_out<<count<<":"<<LC<<":-:"<<endl;
                                cout<<count<<"   "<<LC<<"-"<<endl;          
                              LC  = LC + 4;
                                }        
                  }                                                                                            
            }//END IF

          itr_mot = mot_map.find(alp_operationfield);        //CHECKING WHETHER THE OPERATION IS A MACHINE OP 
           if(itr_mot!=mot_map.end())
            {
               poststring = getpoststr(alp_operandfield,',');  
               if(poststring[0]=='='){poststring = getpoststr(poststring,'=');flag=1;}else flag=0;
               prestring = get_prestr(alp_operandfield,',',0);
               if(isint(poststring)){symbol_value2=strtoint(poststring);}
               if(str_array_index!="") {
                                       if(flag==1){poststring=getpoststr(alp_operandfield,'=');}
                                       else 
                                       poststring = get_prestr(poststring,'(',0);
                                       }

                if(!(isint(poststring)))              //FOR BRANCHING THE SECOND SYMBOL IS AN INTEGER 
                {                           
                    itr_st = symtab_map[PR_NUM].find(poststring);
                                                 if(itr_st!=symtab_map[PR_NUM].end())
                                                      {
                                                           symbol_value2 = itr_st->second.sym_value;  
							                  
                                                      } 
                                                      else  
                                                      {
                                                      array_index = 0;
                                                       
                                                        itr_lt = littab_map.find(poststring);
                                                        if(itr_lt!=littab_map.end())
                                                           {
                                                             symbol_value2 = itr_lt->second.lit_value;                      
                                                           }
                                                           
                                                      }
                }            
                if(prestring==""){symbol_value1=0;}  //FOR BRANCHING THE PRESTRING ="" SO ASSIGNING SYMBOL_VALUE = 0 AS A FLAG                         
               itr_st = symtab_map[PR_NUM].find(prestring);
                                                     if(itr_st!=symtab_map[PR_NUM].end())
                                                      {
                                                           symbol_value1 = itr_st->second.sym_value;                      
                                                      } 
                                                      else
                                                      {
                                                            symbol_value1 = strtoint(prestring); 
                                                      }          
                //CALCULATING SMALLEST OFFSET
		
               smallest_itr_btm = base_table_map.begin();
               smallest_offset = ret_mod(symbol_value2 - smallest_itr_btm->second);    //ITR TO THE FIRST BASE
               for(itr_btm = base_table_map.begin();itr_btm != base_table_map.end();itr_btm++)
               {
                           offset = ret_mod(symbol_value2 - itr_btm->second);
                           if(smallest_offset > offset) {smallest_itr_btm = itr_btm ;smallest_offset=offset;} 
                           }                                     
                offset = smallest_offset ;base_reg = smallest_itr_btm->first ;//FOR R,R FORMAT = 0
                if(itr_mot->second.format=="0")
				{
		if(symbol_value1==0){symbol_value1=15;}
		mc_out<<count<<":"<<LC<<":"<<alp_operationfield<<":"<<symbol_value1<<","<<symbol_value2<<": \n";
		cout<<count<<" "<<LC<<" "<<alp_operationfield<<" "<<" \n";
		out_card<<"T"<<":"<<count<<":"<<LC<<":"<<alp_operationfield<<" "<<symbol_value1<<","<<symbol_value2<<":0: \n";
				}
                else
                {if(symbol_value1==0){symbol_value1=1;} //FOR  R,X FORMAT = 1
                mc_out<<count<<":"<<LC<<":"<<alp_operationfield<<":"<<symbol_value1<<","<<offset<<"("<<array_index<<","<<base_reg<<"):\n"; 
                cout<<count<<" "<<LC<<" "<<alp_operationfield<<" "<<symbol_value1<<","<<offset<<"("<<array_index<<","<<base_reg<<")\n"; 
		out_card<<"T"<<":"<<count<<":"<<LC<<":"<<alp_operationfield<<" "<<symbol_value1<<","<<offset<<"("<<array_index<<","<<base_reg<<"):0:\n"; 
                }
               LC = LC + bin_strtoint(itr_mot->second.ins_len)*2; //INCREASING THE LEAST COUNT BY THE BINARY VALUE OPERATIONS LENGTH
           }  
   
     in_ASSEMBLY.getline(line,100);count++;
    }//END WHILE 
      mc_out.close();  lc_out.close(); bt_out.close();lit_out.close();lit_out2.close();out.close();in_ASSEMBLY.close();  out_card.close();
      cout<<"The Symbol Table Is as Follows :\n";
      printfile("symboltable.txt");
      cout<<"The Literal Table Is as Follows :\n";
      printfile("literaltable2.txt");
return 0;
}
