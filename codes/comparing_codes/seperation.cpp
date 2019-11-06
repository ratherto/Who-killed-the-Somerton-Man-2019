#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <ostream>
#include <sstream>
#include <vector>

using namespace std;


//get the rsids and corresponding genotypes of the file
vector<string>** speration(string filename){
	vector<string> *rsid_list = new vector<string>;
	vector<string> *genotype_list = new vector<string>;
	vector<string>** result =new vector<string>* [2];


	//start sperate the file;
	cout<<"Processing the file: "<<filename<<endl;

	/* open a file in read mode. */
	ifstream infile; 
	infile.open(filename); 

	if (!infile.good()){
		cout<<"file not exist, please check the inputfile name:\""+filename+"\"";
	}

	string data;
	string data_pre;
	//skip useless lines
	while ( (data_pre!="position" || data!="genotype") && !infile.eof() ){
			data_pre = data;
			infile >> data; 
	}

	//starting sperate rsid and genotypes
	int i=0;
	string rsid="";
	int chrom=0;
	int j=0; //use to count how many available SNPs in the file
	bool a=true;
	while (!infile.eof()){
		infile>>data;

		if(i==0){	//rsid
			i++;
			rsid=data;
		}
		else if (i==3){	//genotype
			i=0;
			if (rsid[0]!='i' && data!="--" && chrom!= 0){
				rsid_list->push_back(rsid);
				genotype_list->push_back(data);
				j++;
			}
		}
		else if (i==1){	//chrom
			i++;
			istringstream ( data ) >> chrom;	
			//cout<<chrom<<endl;
		}
		else{
			i++;
		}
		//if (infile.eof()) break;
	}

	result[0]=rsid_list;
	result[1]=genotype_list;

	cout<<j<<" non-empty SNPs"<<endl<<"Done"<<endl;
	infile.close();
	return result;

}


