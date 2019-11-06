#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <ostream>
#include <sstream>
#include <vector>

using namespace std;

/*
By exectue the code, the terminal will ask the user to input the file name(not including suffix eg.for a.txt enter a)
Then enter the level, where level is how much percentage of SNPs u like to kept. 
Note that this program will remove first n snps for every 10 snps, therefore u can only enter 0.1, 0.2, 0.3, ..., 0.9 for level
Note that the file has to be in the format of 23andMe format
Note that the header row has to be modified as "#rsid	chromosome	position	genotype" where normally the header row is "# rsid	chromosome	position	genotype"
*/

static string base_pairs[4]={"A","C","T","G"};//defining the DNA Nucleobase

/*a function to randomly generate a base pair*/
string random_base_pair(){
	string pair=base_pairs[rand()%4]+base_pairs[rand()%4];
	return pair;
}

vector<string> rsid_list(){
	string inputfile="rsid_list.txt";
	vector<string> rs_list;

	/* open a file in read mode. */
	ifstream infile; 
	infile.open(inputfile); 

	if (!infile.good()){
		cout<<"rsid file not exist, please check the inputfile name:\""+inputfile+"\"";
		return rs_list;
	}
	else{
		string data;
		while (true){

			infile >> data; 
			rs_list.push_back(data);
			if (infile.eof()) break;
		}

		/*print out all id*/
		/*for (int i=0;i<rs_list.size();i++){
			cout<<rs_list[i]<<", ";
		}
		cout<<endl;*/
		infile.close();
		return rs_list;
	}

}


int main(){
	srand((unsigned)time(NULL)); //initialize random seed.

	string inputfile="genome_Zihe_Wang_v5_Full_20190501165205";  //warning: only need prefix name
	cout<<"Enter a file name: ";
	cin>>inputfile;

	float level;
	cout<<"Enter degradation level: ";
	cin>>level;

	stringstream ss;
	ss << level;
	string l_str = ss.str();

	level=10*level;

	string suffix="txt";
	string suffix2="csv";
	string outputfile="Degradation_"+l_str+"_"+inputfile+"."+suffix;
	inputfile=inputfile +"."+suffix;
	string data;
	int min=3500;


	/*********************create a vector that record all rsID with exist base pairs in SM file**********************************/
	//vector<string> rsIDs=rsid_list();
	/****************************************************************************************************************************/

	/* open a file in read mode. */
	ifstream infile; 
	infile.open(inputfile); 

	if (!infile.good()){
		cout<<"file not exist, please check the inputfile name:\""+inputfile+"\"";
	}
	else{
		/*open csv files in write mode.*/
		/*a txt file use to record the new result*/
		ofstream outfile;
		outfile.open(outputfile);

		/*a csv file use to record the new result*/

		string line="";//"line" is for recording each line of the input file;for txt output file;

		/*start reading file*/
		cout<<"start reading file"<<endl;

		/*record the header*/
		while (data!="#rsid" && !infile.eof() ){
			infile >> data; 
		}
		line=line+data+"\t";
		infile >> data; 
		line=line+data+"\t";
		infile >> data; 
		line=line+data+"\t";
		infile >> data; 
		line=line+data+"\t";
		outfile << line<< endl;
		line="";

		/*start to replace the empty base pair with a random pair*/
			bool check=false;
			float j=0;
			int i=0;
			int chrom=0;
			int chrom_pre=0;
			//start degarding
			while (true){
				infile>>data;
				if(i==0){
					i++;
					line=line+data+"\t";
				}
				else if (i==3){	//genotype
					//cout<<data<<endl;
					i=0;
					if (data!="--"){
						j++;
					}

					if (data!="--" && j>level){
						data="--";
					}


					line=line+data;

					outfile << line << endl;//write the line to the csv output file if it has a base pair
					line="";
					check=false;

					if (j==10){
						j=0;
					}
				}
				else if(i==1){	//chrom
					line=line+data+"\t";
					i++;
					istringstream iss (data);
					iss >> chrom;
					if (chrom!=chrom_pre){
						j=0;
					}
					chrom_pre=chrom;
				}
				else{
					line=line+data+"\t";
					i++;
					//cout<<data<<"\t";
				}
				if (infile.eof()) break;
			}
			/*close the file*/
			infile.close();

			outfile.close();
			cout<<"end"<<endl;

	}

	return 0;
}
