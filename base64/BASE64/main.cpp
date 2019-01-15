#include <iostream>
#include "base64.h"
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <string.h>
#include <conio.h>

#define loop(n) for(int LOOPER = 0; LOOPER<n; ++LOOPER)

//using namespace std;
bool is_base64(std::string filename);

int main(int args, char *argv[])
{
    bool decode_out = false;
	if(args == 3 && (argv[2][0] == 'e' || argv[2][0] == 'd')) { std::cout<<"Plik wejsciowy: "<<argv[1]<<std::endl; }
    else if (args == 4 && argv[3][0] == 'd') { std::cout<<"Plik wejsciowy: "<<argv[1]<<"\nPlik wyjsciowy: "<<argv[2]<<std::endl; decode_out = true; }
	else { std::cout<<"Musisz podac plik wejsciowy\n"; return -1; }
	
	std::ifstream pFileIn;
	pFileIn.open(argv[1], std::ios::binary);
	if(!pFileIn.good() && !pFileIn.is_open()) { std::cout<<"Nie da sie otworzyc pliku "<<argv[1]<<std::endl; return -1; }
	else std::cout<<"otwarto "<<argv[1]<<std::endl;
	
	std::string work_file = argv[1];
	char type = (decode_out) ? argv[3][0] : argv[2][0];
	std::string newFile = "";
	if (type == 'e') {
		if(is_base64(work_file)) {
			std::cout<<"plik ma zle rozszerzenie !\n";
			pFileIn.close();
			std::cout<<"zamknieto "<<argv[1]<<std::endl;
			return -1;
		}
		else {
			for(int i=0;argv[1][i] != 0; ++i) newFile += argv[1][i];
			newFile = newFile.substr(0,newFile.find('.')) + ".base64";
		}
	}
	else if (type == 'd' )  {
		if(!is_base64(work_file)) {
			std::cout<<"plik ma zle rozszerzenie !\n";
			pFileIn.close();
			std::cout<<"zamknieto "<<argv[1]<<std::endl;
			return -1;
		}
		else if (!decode_out) {
			for(int i=0;argv[1][i] != 0; ++i) newFile += argv[1][i];
			newFile = newFile.substr(0,newFile.find('.')) + "."; //newFile = newFile.substr(0,newFile.find('.')) + ".txt";
		}
        else {
            for(int i=0;argv[2][i] != 0; ++i) newFile += argv[2][i];
        }
	}
	else {
		std::cout<<"FATALNY BLAD !!\n\n\n";
		pFileIn.close();
		return -1;
	}
	
	
	std::ofstream pFileOut;
	std::ifstream exsist_test;
	
	exsist_test.open(newFile.c_str(), std::ios::binary);
	if (exsist_test.is_open()) {
		std::cout<<"plik juz istnieje, chcesz go nadpisac?\n[y/n]\n";
		char choice;
		jeszcze_raz:
		choice = getch();
		switch(choice)
		{
			case 'y':
			case 'Y':
			case 't':
			case 'T':
				exsist_test.close();
				pFileOut.open(newFile.c_str(),std::ios::trunc | std::ios::binary); //erase file before open
				std::cout<<newFile<<" zostal wyczyszczony!\n";
				break;
			case 'n':
			case 'N':
				exsist_test.close();
				std::cout<<"koniec programu\n";
				return -1;
				break;
			default:
				std::cout<<"hmm?, co miales/as na mysli?\nsprobuj jeszcze raz,\ny -> chce wyczyscic istniejacy plik base64\nn -> chce zakonczyc prace programu\n";
				goto jeszcze_raz;
		}
	}
	else { 
		pFileOut.open(newFile.c_str(), std::ios::binary); //create new file
		std::cout<<"stworzono plik "<<newFile<<std::endl;
	}
	
	
	if(!pFileOut.good() && !pFileOut.is_open()) { std::cout<<"Jest problem z plikiem wyjsciowym\n("<<newFile<<")\n"; return -1; }
	else std::cout<<"otwarto "<<newFile<<std::endl;
    //-------------------START------------------------------
	
	
	
	
	
	if (type=='e')
	{
		// For encode --------------------
		char buffer [3072] = {}; //1024 * 3
		std::string ss="";
		while(!pFileIn.eof()) {
			pFileIn.read(buffer,3072);
			ss += base64_encode(reinterpret_cast<unsigned char const*>(buffer), pFileIn.gcount());
			pFileOut<<ss;
			ss="";
		}
	}
	else
	{
		// For decode --------------------
		char buffer [3072] = {}; //1024 * 3
		std::string ss="";
		while(!pFileIn.eof()) {
			pFileIn.read(buffer,3072);
			for(int i=0;i < pFileIn.gcount(); ++i) ss += buffer[i];
			ss = base64_decode(ss);
			pFileOut<<ss;
			ss="";
		}
	}
	
	
	
	
	// ----------------------FINISH--------------------------
	pFileOut.seekp(0,std::ios::end);
	int size = pFileOut.tellp();
	pFileOut.close();
	std::cout<<"zamknieto "<<newFile<<std::endl;
	pFileIn.close();
	std::cout<<"zamknieto "<<argv[1]<<std::endl;
	
	if (!size) { 
		if( remove( newFile.c_str() ) == 0 )
			 printf( "Usunieto pusty plik base64" );
		else
			 printf( "Swietna robota !" );
	}
	return 0;
}

bool is_base64(std::string filename)
{
	int dot_pos = filename.find('.');
	char base[] = ".base64";
	if(!(filename.size()-dot_pos == 7) || !isalnum(filename[0])) {
		return false; 
	}
	int i = 0;
	while(i<7) {
		if ((char)filename[dot_pos+i] != (char)base[i++] ) return false;
	}
	return true;
}















// koniec pliku main.cpp

