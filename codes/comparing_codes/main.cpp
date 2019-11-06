#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <ostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;
using namespace std;

/*
this program allow the user to compare common SNPs among 3 files A, B, S
firstly, it will count how many common SNPs are there between A and S
then count common SNPs between B and S
then is count common SNPs among all 3 files
The program will ask to select a input file as A and B
If u like to change file S, then u have to modify the code, by changing the variable fileS (including suffix) eg. change to a.txt then fileS="a.txt"
the input files as A and B should be place in folder /input_files
and file S should be placed in folder /file_S

the counting result will be displayed and be recorded in log/log.txt

to compile the codes: g++ -lstdc++fs -std=c++17 -o Counting main.cpp seperation.cpp comparison.cpp -lstdc++fs
then execute codes: ./counting

Note that if u are using MSYS in windows to compile this code, it might not work due to some bug with MSYS itself
*/

extern vector<string>** speration(string filename);	//seperate a DNA file, returning 2 vectors, first is rsid vector, second is corresponding alleles
extern vector<string>** comparison(vector<string> rsid1,vector<string> alleles1, vector<string> rsid2, vector<string> alleles2); //returning 2 vectors: first is common rsid vector (note that the last element would be the number of how many common SNPs between 2 DNA files, this element should be pop out), and second is corresponding alleles.
																																 //when comparing file S with other files, the first rsid and alleles vectors should belong to other files'(file A or B); second alleles and rsid vector should belong to file S 
string inputfile(string AB){
	string path = "input_files/";
	vector<string> input_files;
	int index=1;

	cout<<"Please select a file as file "<<AB<<": "<<endl;
	for (const auto & entry : fs::directory_iterator(path)){
		fs::path p = entry.path();
		input_files.push_back(p.string());
    	cout << index <<". " << (p.string()).substr(12) << endl;
    	index++;
    }
	cout<< "r. Refresh menu"<<endl;


    string file;
    int file_int;

    while (true){
    	cout<<endl<<"Please enter an index: ";
    	cin>>file;
    	istringstream ( file ) >> file_int;

    	if (file_int > 0 && file_int < index){
    		file=input_files[file_int-1];
    		cout<<"File "<<AB<<" selected: "<<file.substr(12)<<endl;
    		break;
    	}
    	else if(file=="r"){

    		input_files.clear();
			int index=1;

    		cout<<endl<<"Please select a file as file "<<AB<<": "<<endl;
			for (const auto & entry : fs::directory_iterator(path)){
				fs::path p = entry.path();
				input_files.push_back(p.string());
		    	cout << index <<". " << (p.string()).substr(12) << endl;
		    	index++;
		    }
			cout<< "r. Refresh menu"<<endl;
    	}
    	else {
    		cout<<"Invalid index, please enter an available index"<<endl;
    	}
    }

    return file;
}

int main(){
	vector<string>** fileA_sep, **fileB_sep, **fileS_sep;
	vector<string>** comparison_AS, **comparison_BS, **comparison_AB;


	//cout<<"Start comparing files"<<endl;


	/**************selecting file A**************/
	string fileA=inputfile("A");
    /********************************************/


	/**************selecting file B**************/
	string fileB=inputfile("B");
    /********************************************/


	/**file S should be stored in path: /file_S */
	string fileS="samtools_sm_23andme_dp5_edit_with_header.txt";	//u might like to change it to other files, plz include suffix eg. .txt
	fileS="file_S/"+fileS;
    /********************************************/

	/***********list file A, B and S*************/
	cout<<endl<<"*****************************************************************************"<<endl;
	cout<<"File A: "<<fileA.substr(12)<<endl;
	cout<<"File B: "<<fileB.substr(12)<<endl;
	cout<<"File S: "<<fileS.substr(7)<<endl;
	cout<<"*****************************************************************************"<<endl<<endl;

	/********************************************/

    /***************processing files*************/
	fileA_sep=speration(fileA);
	vector<string> *rsidA=new vector<string>;
	vector<string> *allelesA=new vector<string>;
	for (int i=0;i<fileA_sep[0]->size();i++){
		rsidA->push_back((*fileA_sep[0])[i]);
		allelesA->push_back((*fileA_sep[1])[i]);
	}

	fileB_sep=speration(fileB);
	vector<string> *rsidB=new vector<string>;
	vector<string> *allelesB=new vector<string>;
	for (int i=0;i<fileB_sep[0]->size();i++){
		rsidB->push_back((*fileB_sep[0])[i]);
		allelesB->push_back((*fileB_sep[1])[i]);
	}

	fileS_sep=speration(fileS);
	vector<string> *rsidS=new vector<string>;
	vector<string> *allelesS=new vector<string>;
	for (int i=0;i<fileS_sep[0]->size();i++){
		rsidS->push_back((*fileS_sep[0])[i]);
		allelesS->push_back((*fileS_sep[1])[i]);
	}
	/********************************************/

	/*************comparing A and S**************/
	cout<<endl<<"Counting common SNPs between file A and S......"<<endl;
	comparison_AS=comparison(*rsidA,*allelesA,*rsidS,*allelesS);
	vector<string> *rsidAS=new vector<string>;
	vector<string> *allelesAS=new vector<string>;
	int commonAS, length;
	length=(comparison_AS[0]->size())-1;
	istringstream ( (*comparison_AS[0])[length] ) >> commonAS;
	comparison_AS[0]->erase(comparison_AS[0]->begin() + length);	

	for (int i=0;i<comparison_AS[0]->size();i++){
		rsidAS->push_back((*comparison_AS[0])[i]);
		allelesAS->push_back((*comparison_AS[1])[i]);
	}

	cout<<"Number of common SNPs between file A and file S: "<<commonAS<<endl;
	/********************************************/

	/*************comparing B and S**************/
	cout<<endl<<"Counting common SNPs between file B and S......"<<endl;
	comparison_BS=comparison(*rsidB,*allelesB,*rsidS,*allelesS);
	vector<string> *rsidBS=new vector<string>;
	vector<string> *allelesBS=new vector<string>;
	int commonBS;
	length=(comparison_BS[0]->size())-1;
	istringstream ( (*comparison_BS[0])[length] ) >> commonBS;
	comparison_BS[0]->erase(comparison_BS[0]->begin() + length);	

	for (int i=0;i<comparison_BS[0]->size();i++){
		rsidBS->push_back((*comparison_BS[0])[i]);
		allelesBS->push_back((*comparison_BS[1])[i]);
	}

	cout<<"Number of common SNPs between file B and file S: "<<commonBS<<endl;
	/********************************************/

	/****comparing B and A only includes common snps with S****/
	cout<<endl<<"Counting common SNPs between file A and file B with same degradation level as file S......"<<endl;
	comparison_AB=comparison(*rsidAS,*allelesAS,*rsidBS,*allelesBS);
	vector<string> *rsidAB=new vector<string>;
	vector<string> *allelesAB=new vector<string>;
	int commonAB;
	length=(comparison_AB[0]->size())-1;
	istringstream ( (*comparison_AB[0])[length] ) >> commonAB;
	comparison_AB[0]->erase(comparison_AB[0]->begin() + length);	

	for (int i=0;i<comparison_AB[0]->size();i++){
		rsidAB->push_back((*comparison_AB[0])[i]);
		allelesAB->push_back((*comparison_AB[1])[i]);
	}

	cout<<"Number of common SNPs  between file A and file B with same degradation level as file S: "<<commonAB<<endl;
	/**********************************************************/

	/************Summarise the result************/
	cout<<endl<<"*****************************************************************************"<<endl;
	cout<<"File A: "<<fileA.substr(12)<<endl;
	cout<<"File B: "<<fileB.substr(12)<<endl;
	cout<<"File S: "<<fileS.substr(7)<<endl;

	cout<<"Number of common SNPs between file A and file S: "<<commonAS<<endl;
	cout<<"Number of common SNPs between file B and file S: "<<commonBS<<endl;
	cout<<"Number of common SNPs  between file A and file B with same degradation level as file S: "<<commonAB<<endl;
	cout<<"*****************************************************************************"<<endl<<endl;
	ofstream outfile;
	outfile.open("log/log.txt", ios_base::app);

	//record time
	time_t now = time(0);
	char* current_time = ctime(&now);

	//record the results in log
	outfile << current_time << endl;
	outfile	<< "\tFile A: " << fileA.substr(12) << endl;
	outfile	<< "\tFile B: " << fileB.substr(12) << endl;
	outfile	<< "\tFile S: " << fileS.substr(7) << endl;
	outfile<<"\tNumber of common SNPs between file A and file S: "<<commonAS<<endl;
	outfile<<"\tNumber of common SNPs between file B and file S: "<<commonBS<<endl;
	outfile<<"\tNumber of common SNPs  between file A and file B with same degradation level as file S: "<<commonAB<<endl<<endl;

	outfile.close();
	/********************************************/

}