#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <ostream>
#include <sstream>

using namespace std;

/*
this program is to replace the empty snps in input file with homozygous pairs CC
To change the input file name refer to variable inputfile
Also have to change array exist[22] to the number of non-empty snps in each chromosone
To change the total amount of non empty snps in the generated file refer to variable min. eg if u like 5000 SNPs in each chromosone min=5000
*/


static string base_pairs[4]={"A","C","T","G"};//defining the DNA Nucleobase

/*a function to randomly generate a base pair*/
string random_base_pair(){
	string pair=base_pairs[rand()%4]+base_pairs[rand()%4];
	return pair;
}


int main(){
	srand((unsigned)time(NULL)); //initialize random seed.


	string inputfile="samtools_sm_23andme_dp5_edit_with_header";  //warning: only need prefix name
	string suffix="txt";
	string suffix2="csv";
	string outputfile="CC_"+inputfile+"."+suffix;
	inputfile=inputfile +"."+suffix;
	string data;
	int exist[22]={1014,978,658,621,661,880,655,601,519,705,705,596,393,441,440,558,519,372,514,375,245,303};
	string pairs[22]={"CT","CC","TT","GA","CC","GG","CT","TT","AG","TT","TG","GA","TT","AT","TC","CC","GA","CA","GA","GG","TC","CC"};
	int min=3500;


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
			int j=0;
			int i=0;
			int chrom=0;
			int chrom_check=0;
			while (true){
				infile>>data;
				if (i==3){
					//cout<<data<<endl;
					i=0;
					if (data=="--" && j<min-exist[chrom-1]){
						data="CC";//replace the empty pair with random one
						j++;
					}
					line=line+data;
					
					//cout<<line<<endl;

					outfile << line << endl;//write the line to the csv output file if it has a base pair
					line="";
				}
				else if(i==1){
					line=line+data+"\t";
					if(data!="mt" && data!="X"){
						istringstream iss (data);
						iss >> chrom;
						if (chrom_check==0){
							chrom_check=chrom;
						}
						else if(chrom_check!=chrom){
							//cout<<chrom_check<<" "<<j<<endl;
							j=0;
							chrom_check=chrom;
						}
					}
					else{
						istringstream iss (data);
						iss >> chrom;
						j=20000;
					}
					//cout<<chrom<<endl;
					i++;

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
