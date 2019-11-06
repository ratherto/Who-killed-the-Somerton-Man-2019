#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <ostream>
#include <sstream>
#include <vector>

using namespace std;

//count how many common SNPs between 2 DNA files. Return the common SNPs' rsids and alleles. The last element of rsid vector would store the number of common SNPs
//when comparing file S with other files, the first rsid and alleles vectors should belong to other files'(file A or B); second alleles and rsid vector should belong to file S 
vector<string>** comparison(vector<string> rsid1,vector<string> alleles1, vector<string> rsid2, vector<string> alleles2){

	vector<string> *rsid_list = new vector<string>;
	vector<string> *genotype_list = new vector<string>;
	vector<string>** results =new vector<string>* [2];

	int result=0; //counting how many common SNPs
	for (int n=0;n<rsid1.size();n++){
		for (int m=0;m<rsid2.size();m++){
			if (rsid1[n]==rsid2[m]){// check any snps have common rsid
				if (alleles1[n]==alleles2[m]){
					result++;	//if same allleles then 1 common SNPs found
				}
				//store the common rsid and alleles in the vectors
				rsid_list->push_back(rsid1[n]);
				genotype_list->push_back(alleles1[n]);

				//increase searching efficiency
				rsid2.erase(rsid2.begin() + m);		//a common SNP found means this SNP is no need for the rest of search, remove it
				alleles2.erase(alleles2.begin() + m);	//same as above
				break; //snps with common rsid found, can stop from searching with rest snps in file2;
			}
		}
	}

	//convert the number of common SNPs to string and store in the tail of rsid vector
	stringstream ss;
	ss << result;
	string count = ss.str();
	rsid_list->push_back(count);

	results[0]=rsid_list;
	results[1]=genotype_list;

	return results;
}