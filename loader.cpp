#include "header_loader.h"

int main()
{
system("clear");
int PLA[5],IPLA[5],VALUE,SLENGTH,ADDR,LENGTH,EXADDR,ID,LOC,NUM_RELOC,CONTENT,count,x = 1, NUM_SUB  ,PR_NUM = 0;//INTEGER VARS
int LESA[20];
cout<<"ENTER THE NUMBER OF SUBROUTINES\n";
cin>>NUM_SUB;
cout<<"ENTER THE INITIAL ADDRESSES OF THE SUBROUTINES "<<endl;
while(x <= NUM_SUB)
   {
	cin>>PLA[x];IPLA[x] = PLA[x];x++;
	
   }
//IPLA[1]= PLA[1] = 104; IPLA[2] = PLA[2] =168; 
char line[100];char str[10][20];
char card_type;
string symbol_name,ESD_type;
map <string,int> map_GEST;
map <string,int> :: iterator itr_GEST;
ifstream card_in("card2.txt",ios::in );
ofstream out_GEST("gest.txt",ios::out);
ofstream out_LOADCODE("loadedcode.txt",ios::out);
card_in.getline(line,100);
  while(!(card_in.eof()))
	{
	delim(str,line);
        card_type = str[1][0];
        switch(card_type)
		{
		case 'E':
                ESD_type = str[4];
                if(ESD_type == "SD"){PR_NUM++;VALUE = PLA[PR_NUM]; LENGTH = strtoint(str[7]);SLENGTH = LENGTH;}		
		else if(ESD_type == "LD"){ADDR = strtoint(str[6]);VALUE = PLA[PR_NUM] + ADDR ;}
                if(ESD_type == "SD" || ESD_type == "LD"){
 								symbol_name = str[3];
								itr_GEST = map_GEST.find(symbol_name);
								if(itr_GEST == map_GEST.end())
									{
									map_GEST.insert(pair<string,int>(symbol_name,VALUE));
									out_GEST<<symbol_name<<":"<<VALUE<<":"<<endl;
									}				       
								else cout<<"DUPLICATE USE OF START OF ENTRY NAME "<<symbol_name<<endl;
								} 
                break;
		case 'T':
                break;
		case 'R':
		break;
		default:;
		//PLA[PR_NUM] = PLA[PR_NUM] + SLENGTH;	
		}
        card_in.getline(line,100);
	} 
card_in.close();
						//PASS1 ENDS AND PASS 2 BEGINS
 EXADDR = IPLA[PR_NUM] ;PR_NUM = 0;
card_in.open("card2.txt", ios :: in);

card_in.getline(line,100);
 while(!(card_in.eof()))
	{
	delim(str,line);
	card_type = str[1][0];
	switch(card_type)
		{
		case 'E':
		ESD_type = str[4];
		if(ESD_type == "LD") break ;
		else if(ESD_type == "SD") {PR_NUM++;LENGTH = strtoint(str[7]);SLENGTH = LENGTH;ID = strtoint(str[5]);LESA[ID] = PLA[PR_NUM];}
			else if(ESD_type == "ER")
						{
						ID = strtoint(str[5]);
						symbol_name = str[3]; itr_GEST = map_GEST.find(symbol_name);
						if(itr_GEST != map_GEST.end()){ VALUE = itr_GEST->second ; LESA[ID] = VALUE;}	
						else cout<<"UNDEFINED EXTERNAL SYMBOL "<<symbol_name<<" OR SEGMENT MISSING "<<endl;						
						}
	
		break;
		case 'T':
		ADDR = strtoint(str[3]);LOC = PLA[PR_NUM] + ADDR;NUM_RELOC = strtoint(str[5]);count = 0;
		if(NUM_RELOC !=0)
		{
		CONTENT = strtoint(str[4]) ;
		while(count<NUM_RELOC)
		{		
		card_in.getline(line,100);delim(str,line);	count ++ ;	
		if(!(card_in.eof()))
			{
				 ID = strtoint(str[3]) ; VALUE = LESA[ID];
				 switch(char(str[5][0]))
				{
				case '+' :
				CONTENT = CONTENT + VALUE ;
				break;
				case '-' :
				CONTENT = CONTENT - VALUE ;
				break;
				default:
				cout<<"OPERATOR INVALID "<<line<<endl;cin>>x;
				}
			}
		}
					out_LOADCODE<<(PLA[PR_NUM]+ADDR)<<":"<<CONTENT<<":"<<endl;
		}
               
                else out_LOADCODE<<(PLA[PR_NUM]+ADDR)<<":"<<str[4]<<":"<<endl;		
		break;
		case 'D':
		if(str[2]!="") {EXADDR = PLA[PR_NUM] + strtoint(str[2]);}
	//	PLA[PR_NUM] = PLA[PR_NUM] + SLENGTH ;
		break;
		cout<<" CONTROL TRANSFERED TO  "<<EXADDR<<endl;cin>>x;
		}
	card_in.getline(line,100);
	}
	cout<<"The Loaded Code Is as Follows :\nADDRESS:CONTENT:\n";
      	printfile("loadedcode.txt");
return 0;
}
