#include <iostream>
#include <map>
#include <fstream>
#include <string>

// ======= INSPIRATION BIRD ======== (to inspire you to do better)
//        _ _.-''''''--._
//      .` `.  ...------. - Do better, pl0x. (Inspiration Bird, 2014)      
//     / |O :-`   _,.,---'
//    '      ;--''
//    | _.' (
//    ,' _,' `-.
//    : /       '.
//    |           '
//     `.|         `.
//       `-._	      .
/*           '.  ,-.   .
	     .__          _`. ..` 	.
	     ,  ''- . _,-'.,-'  ``: .
	     '-...._ (( (('-.._    . .
	     `--..      `'-. . .
	     `..     '   . .
	     `         `"
                    

	     ======= INSPIRATION BIRD ========


	     ======= PROJECT =================

	     Three objectives:
	     -Retrieve most frequently used words from a data set (not including stop words defined in "stop.txt"). 
	     This means take a frequency count for each word in a data set and report the top 5% in a file "report.txt".

	     -Compute the percentage of stop words.

	     -Print out all the words within each data set (using dictionary ordering). Provide number of occurrences 
	     within a data set (including stop words). 

	     ======= PROJECT =================
*/   
using namespace std;

/*
  ===== is_stopWord =====
  Checks if string s is a member of the stopWords list.
*/
bool is_stopWord( const string& s, const map<string,int> &stopWords ) {
  bool is_sWord = true;
  if(stopWords.find(s)==stopWords.end())
    is_sWord=false;
  //if( is_sWord == true ) cout << "FOUND STOP WORD!!" << endl;
  return is_sWord;
}

/* 
   ===== strip_punc ======
   Strips punctuation from the word, if any.
   And lowercase entire word
*/
void strip_punc( string& s ) {

    if( s[0] % 'A' > 25 && s[0] % 'a' > 25 && s[0] - '0' < 0 ) {
      s.erase(0,1);
      if( s[0] % 'A' > 25 && s[0] % 'a' > 25 && s[0] - '0' < 0 ) s.erase(0,1);
    }
    if( s[s.size() - 1] % 'A' > 25 && s[s.size() - 1] % 'a' > 25 && s[s.size() - 1] - '0' < 0 ) {
      s.erase(s.size()-1,1);
      if( s[s.size() - 1] % 'A' > 25 && s[s.size() - 1] % 'a' > 25 && s[s.size() - 1] - '0' < 0 ) s.erase(s.size()-1,1);
    }
  
}

void heapify(pair<int,string>* &arr,int c){
  for(int i=1;i<c;i++){
    pair<int,string> val=arr[i];
    int s=i; 
    int f=(s-1)/2 ;
    while(s>0 && arr[f].first<val.first){ 
      arr[s]=arr[f];
      s=f;
      f=(s-1)/2;
    } 
    arr[s] = val;
  }
}

// =========== main =============

int main(int argc,char *argv[]){
  int unique=0; //count of total unique words
  int total=0; //total number of words
  int stop=0; //number of stop words
  int titleCap=1;
  int titleCount=0;
  int stopCount=0;
  string title,buffer,*titleArr=new string[1]; //title name,standard string buffer
  map<string,int> stopWords; //dict holding stop words

  // ============ Setting up a dictionary of stop words from "stop.txt" =======
  fstream stopFile( "stop.txt", ios::in );
  
  if( stopFile.is_open() ) {
    int stopIter = 0;
    while( getline( stopFile, buffer, '\n' ) ) {
      stopWords.insert(pair<string,int>(buffer,0));
      stopIter++;
      stopCount++;
    }
  }
  stopFile.close();

  // ========= Filling dictionary ============
  // key of dictionary is "word:title"
  map<string,int> dict; //DICTIONARY

  // argv[0] should be the name of the file being analyzed
  fstream dictFile( argv[1] , ios::in );
  string entry; // Variable for each individual name in buffer

  if( dictFile.is_open() ) {
    while( getline( dictFile, buffer ) ) {
      //Find a better way to do this
      //Filter out the <TITLE> lines
      if( buffer.substr( 0, 1 ) == "<" ) { 
	title = buffer.substr( 8, buffer.size() - 16 ); //title is located between the 8th character from the start and the 9 character from the end
	if(titleCount==titleCap){
	  string* tmp=new string[titleCap*2];
	  for(int i=0;i<titleCap;i++)
	    tmp[i]=titleArr[i];
	  delete[] titleArr;
	  titleArr=tmp;
	  titleCap=titleCap*2;
	}
	titleArr[titleCount]=title;
	titleCount++;
      }
      //If not a <TITLE> line, parse through the line regularly
      else {
	for( int dictIter = 0; dictIter != string::npos; dictIter=buffer.find(' ',dictIter)+1) { 
	  
	  //If the loop hasn't reached the last word, take the next word in the line.
	  if( buffer.find(' ',dictIter) != string::npos ) {
	    entry = buffer.substr( dictIter, buffer.find(' ',dictIter) - dictIter );
	  }
	  else { //Else, take the last word.
	    entry = buffer.substr( dictIter, buffer.size() - dictIter );
	    break; //kinda makes the break condition for the for-loop useless, but whatever
	  }
	  
	  //Strip any punctuation
	  strip_punc(entry);	
	  
	  //Check if a stopWord or not
	  string s=entry;
	  for(int i=0;i<s.length();i++)
	    tolower(s[i]);
	  if(is_stopWord(s,stopWords))
	    stop++;
	  if( !(is_stopWord( entry, stopWords )) ) { 
	    string key=entry+":"+title;
	    //If the entry doesn't exist in the map, create a new entry (map a new key)
	    if( dict.find(key) == dict.end() ) {
	      dict[key] = 1;
	      unique++; //Although, I disregarded the number of unique stopWords. Idk if you want that.
	    }
	    else { //Else, increment the frequency count associated with that existing entry/key.
	      dict[key]+=1;
	    }
	  }
	  //If it is a stopWord, increment stopWord count.
	  else {
	    stopWords[entry]+=1;
	  }
	  total++;
	}
      }
    }
  }
  dictFile.close();

  //Opening report.txt
  ofstream outputFile;
  outputFile.open( "report.txt" );

  //Closing file
  outputFile.close();
  //part 3 printing out all words including occurances
  auto stopIter=stopWords.begin();
  auto dictIter=dict.begin();
  auto tmpIter=dict.begin();
  tmpIter++;
  /*
    while (dictIter!=dict.end() && stopIter!=stopWords.end()){
    if(stopIter==stopWords.end()){
    string word=dictIter->first.substr(0,dictIter->first.find(':'));
    int num=dictIter->second;
    while(tmpIter!=dict.end() && word==(tmpIter)->first.substr(0,(tmpIter)->first.find(':'))){
    num+=tmpIter->second;
    dictIter++;
    tmpIter++;
    }
    cout<<word<<" "<<num<<endl;
    dictIter++;
      
    dictIter++;
    }
    else if(dictIter!=dict.end() && (dictIter->first).compare(stopIter->first)>0){
    if(stopIter->second>0)
    cout<<stopIter->first<<" "<<stopIter->second<<endl;
    stopIter++;
    }
    else{
    string word=dictIter->first.substr(0,dictIter->first.find(':'));
    int num=dictIter->second;
    while(tmpIter!=dict.end() && word==(tmpIter)->first.substr(0,(tmpIter)->first.find(':'))){
    num+=tmpIter->second;
    dictIter++;
    tmpIter++;
    }
    cout<<word<<" "<<num<<endl;
    dictIter++;
    if(tmpIter!=dict.end())
    tmpIter++;
    }
    }
  //*/

 //INSERT TOP 5% WORDS REPORTING HERE
  //combine same words with diff titles
  string max;
  int tmpMax,tmpTotal;
  map<string,int> tmpDict;
  for(auto iter=dict.begin();iter!=dict.end();iter++){
    tmpMax=0;
    tmpTotal=0;
    string word=iter->first;
    string tmp=word.substr(0,word.find(':'));
    for(int n=0;n<tmp.length();n++)
      tmp[n]=tolower(tmp[n]);
    if(stopWords.find(tmp)==stopWords.end()){
      for(int j=0;j<titleCount;j++){
	string stmp=word.substr(0,word.find(':')+1)+titleArr[j];
	//	cout<<stmp<<endl;
	if(dict.find(stmp)!=dict.end()){
	  int itmp=dict.find(stmp)->second;
	  tmpTotal+=itmp;
	  if(tmpMax<itmp){
	    tmpMax=itmp;
	    max=stmp.substr(stmp.find(':')+1);
	  }
	}
      }
      string key=word.substr(0,word.find(':'))+'%'+max;
      tmpDict.insert(pair<string,int>(key,tmpTotal));
    }
  }
  dict.clear();
  dict=tmpDict;
  /*
for(auto iter=dict.begin();iter!=dict.end();iter++){
  cout<<iter->first<<" "<<iter->second<<endl;
 }  
  //*/
  pair<int,string>* freqDB=new pair<int,string>[unique];
  //insert into array
  int i=0;
  for(auto iter=dict.begin();iter!=dict.end();iter++,i++){
      freqDB[i]=make_pair(iter->second,iter->first);
  }
  //heapsort
  int num=(unique+stopCount)/20; //top 5%;
  heapify(freqDB,i);
  for (i=unique-1;i>unique-(num*2);i--){
    pair<int,string> tmp=freqDB[i];
    string word=tmp.second;
    for(int n=0;n<word.length();n++)
      tolower(word[n]);
    if(is_stopWord(word,stopWords))
      num++;
    freqDB[i]=freqDB[0];
    freqDB[0]=tmp;
    heapify(freqDB,i);
  }

  cout<<"unique: "<<unique<<endl;
  cout<<"stop: "<<stop<<endl;
  cout<<"total: "<<total<<endl;

  for(i=unique-1;i>=unique-num;i--){
    //put this in report.txt
    string word=freqDB[i].second.substr(0,freqDB[i].second.find('%'));
    string ti=freqDB[i].second.substr(freqDB[i].second.find('%')+1);
    cout<<word<<", "<<freqDB[i].first<<", "<<ti<<endl;
  }
  delete[] freqDB;

  //part 2 calculate percent are stop words
  cout<<(float(stop)/total)*100<<"% are stop words"<<endl;
  
  delete[] titleArr;
  return 0;
}

