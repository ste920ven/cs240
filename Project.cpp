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
const int STOP_WORD_COUNT = 49; // Unless Hartog/TA's trip us over with an entirely new set of stop words, I wc'd the list and got 49.

/*
  ===== is_stopWord =====

  Made this function to make main() less ugly. Checks if 
  string s is a member of the stopWords list.
 */
bool is_stopWord( const string& s, const string* stopWords ) {
  
  bool is_sWord = false;
  
  for( int i = 0; i < STOP_WORD_COUNT; i++ ) {
    if( s == stopWords[i] ) {
      is_sWord = true;
      break;
    }
  }

  //if( is_sWord == true ) cout << "FOUND STOP WORD!!" << endl;
  return is_sWord;
}

/* 
   ===== strip_punc ======

   Strips punctuation from the word, if any.
   We don't need to look through the entire word for any punctuation.
   Just make the simplifying case that punctuation occurs at the beginning or end,
   but don't forget that there could be 2 occurrences of punctuation on either end.
   So, I just checked twice.

*/
void strip_punc( string& s ) {
  if( s[0] % 'A' > 25 && s[0] % 'a' > 25 ) s.erase(0,1);
  if( s[0] % 'A' > 25 && s[0] % 'a' > 25 ) s.erase(0,1);
  if( s[s.size() - 1] % 'A' > 25 && s[s.size() - 1] % 'a' > 25 ) s.erase(s.size()-1,1);
  if( s[s.size() - 1] % 'A' > 25 && s[s.size() - 1] % 'a' > 25 ) s.erase(s.size()-1,1);
}

// =========== main =============

int main(int argc,char *argv[]){
  
  int unique=0; //count of total unique words
  int total=0; //total number of words
  int stop=0; //number of stop words
  string title,buffer; //title name,standard string buffer
  string stopWords[STOP_WORD_COUNT]; //array holding stop words

  // ============ Loading array of strings with stop words from "stop.txt" ==============
  fstream stopFile( "stop.txt", ios::in );
  
  if( stopFile.is_open() ) {
    int stopIter = 0;
    while( getline( stopFile, buffer, '\n' ) ) {
	stopWords[stopIter] = buffer;
	stopIter++;
    }
  }
  stopFile.close();

  // ========= Filling dictionary ============

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
	cout << "Title: " << title << endl;
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
	  
	  //Check if a stopWord or not
	  if( !(is_stopWord( entry, stopWords )) ) {
	    
	    //Strip any punctuation
	    strip_punc(entry);

	    //If the entry doesn't exist in the map, create a new entry (map a new key)
	    if( dict.find( entry ) == dict.end() ) {
	      dict[entry] = 1;
	      unique++; //Although, I disregarded the number of unique stopWords. Idk if you want that.
	    }
	    else { //Else, increment the frequency count associated with that existing entry/key.
	      dict[entry]+=1;
	    }
	  }
	  //If it is a stopWord, increment stopWord count.
	  else {
	    stop++;
	  }

	  total++;

	}// end of for-loop

      }//end of big else-block
      
    }// end of while-loop
  }

  dictFile.close();

  cout << "Number of stop words: " << stop << endl;
  cout << "Number of unique words: " << unique << endl;
  cout << "Number of words in total: " << total << endl;

  //cout << "Frequency of <TITLE>: " << dict["<TITLE>"] << endl;
  /*for( map<string,int>::iterator mapIter = dict.begin(); mapIter != dict.end(); mapIter++ ) {
    cout << mapIter->first << endl;
    }*/
  
  /*string d1 = "Hello,,";
  string d2 = "Hello";
  string d3 = ",\"Hello";
  string d4 = "\"Hello";
  string d5 = "Hell-o";
  string d6 = "-";
  strip_punc(d1);
  strip_punc(d2);
  strip_punc(d3);
  strip_punc(d4);
  strip_punc(d5);
  strip_punc(d6);
  cout << "d1: " << d1 << endl;
  cout << "d2: " << d2 << endl;
  cout << "d3: " << d3 << endl;
  cout << "d4: " << d4 << endl;
  cout << "d5: " << d5 << endl;
  cout << "d6: " << d6 << endl;*/
  

  return 0;



//data sturcture
//put stop.txt in a dict

//loop through file and build data structure
//strip punctuation

//output top 5% to report.txt

//print out alphabetically
}

