#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

/*
this program allow the user to count how manying SNPs are in each chromosome and how many SNPs are not empty
Note that the file has to be in the format of 23andMe format
Note that the header row has to be modified as "#rsid	chromosome	position	genotype" where normally the header row is "# rsid	chromosome	position	genotype"
*/

int main(){
	/*setting input and output file name;*/
	string inputfile="samtools_sm_23andme_dp5_edit_with_header";  //warning: only need prefix name
	cout<<"Enter file name: ";
	cin>>inputfile;
	string suffix="txt";
	string suffix2="csv";
	string outputfile="ChromCount_"+inputfile+"."+suffix;

	inputfile=inputfile +"."+suffix;
	string data;

	/* open a file in read mode. */
	ifstream infile; 
	infile.open(inputfile); 

	/*check existence of the input file*/
	if (!infile.good()){
		cout<<"file not exist, please check the inputfile name:\""+inputfile+"\"";
	}
	else{
			/*open csv files in write mode.*/
			/*a file use to record the exist pairs for each chromosome*/
			ofstream outfile;
		   	outfile.open(outputfile);

		   	string line="";//"line" is for recording each line of the input file;
		   	string line2="";//for txt output file

			/*start reading file*/
			cout<<"start reading file"<<endl;

			/*record the header*/
			while (data!="#rsid" && !infile.eof() ){
				infile >> data; 
			}
			line=line+data+",";
			infile >> data; 
			line=line+data+",";
			infile >> data;
			line=line+data+","; 

			infile >> data;
			line=line+data;


			line="";

			/*set counters: total amount; empty pairs; exist pairs;*/
			int chromCount[24]={0};
			int chromExist[24]={0};


			/*start reading each DNA pair and writing the exist pairs to output file*/
			int i=0;
			int chrom=0;
			while (true){
				infile>>data;
				if (i==3){
					line=line+data;
					//cout<<data<<endl;
					i=0;
					chromCount[chrom-1]++;
					if (data!="--"){
						chromExist[chrom-1]++;
					}
					line="";
				}
				else if(i==1){
					line=line+data+",";
					if(data=="mt"){
						chrom=23;
					}
					else if(data=="X"){
						chrom=24;
					}
					else{
						istringstream iss (data);
						iss >> chrom;
					}
					//cout<<chrom<<endl;
					i++;

				}
				else{
					line=line+data+",";
					i++;
					//cout<<data<<"\t";
				}
				if (infile.eof()) break;
			}
			/*close the file*/
			infile.close();

			/*display and record the counting result*/
			int a=0;
			int b=0;

			for (int j=0;j<24;j++){

				if (j==22){
					cout<<"Chrom"<<" mt"<<": "<<chromCount[j]<<endl;
					cout<<"Exist: "<<chromExist[j]<<endl;
					outfile <<"Total amount of chromosome"<<" mt"<<": "<< chromCount[j]<< endl;
					outfile <<"Exist amount of chromosome"<<" mt"<<": "<< chromExist[j]<< endl;
				}
				else if(j==23){
					cout<<"Chrom"<<" X"<<": "<<chromCount[j]<<endl;
					cout<<"Exist: "<<chromExist[j]<<endl;
					outfile <<"Total amount of chromosome"<<" X"<<": "<< chromCount[j]<< endl;
					outfile <<"Exist amount of chromosome"<<" X"<<": "<< chromExist[j]<< endl;
				}
				else{
					cout<<"Chrom"<<j+1<<": "<<chromCount[j]<<endl;
					cout<<"Exist: "<<chromExist[j]<<endl;
					outfile <<"Total amount of chromosome"<<j+1<<": "<< chromCount[j]<< endl;
					outfile <<"Exist amount of chromosome"<<j+1<<": "<< chromExist[j]<< endl;
				}
				a=a+chromCount[j];
				b=b+chromExist[j];
				cout<<a<<" "<<b<<endl;
			}
			outfile.close();
			cout<<"end"<<endl;

	}
	
	return 0;
}
