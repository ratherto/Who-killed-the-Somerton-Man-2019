import httplib
import json
import time

#the this is the data mining program that will read the rsid in "clinical_rsid.txt" and get the corresponding genetic diseases from dbSNP
#the received clinical effects will be recorded in "clinical_effects.txt" and corresponding alleles in "alleles.txt"
#columns for "clinical_effects.txt": #rsid	#seq id	#update time	#ClinVar Accession	#desease name
#columns for "alleles.txt": #rsid	#seq id	#first base	#second base
#"log.txt" records the operations for failed connecting or successfully record a clinical effect. columns: #rsid	#type	#detail  ; where in column type, 0 represent successfully record effects, 2 & 3 for connection error
#note that u might have rerun the program for the SNPs with type 2&3 error recorded in log.txt. This would require u to make a new clinical_rsid.txt with the rsid u need.
#dbsnp API: api.ncbi.nlm.nih.gov/variation/v0/refsnp/rsid         where rsid have to be change to a specific rsid and the website will send back data in json format
#sample api: api.ncbi.nlm.nih.gov/variation/v0/refsnp/rs5257 

def search_rsid( rsid, outputfile, logfile):
	httpClient = None
	line=""
	rs_id=""
	log=""
	try:
		host='api.ncbi.nlm.nih.gov'
		source='/variation/v0/refsnp/'+rsid
		httpClient = httplib.HTTPConnection(host, 80)
		httpClient.request('GET', source)

		response = httpClient.getresponse()
		if response.status==200:
			resp_body=response.read()
			json_obj=json.loads(resp_body)

			alleles=0#variable use to record how many alleles are recorded
			clinical=0#variable use to record how many clinical effects are recorded

			#get details of clinical effects
			for i in json_obj["primary_snapshot_data"]["allele_annotations"]:
				if i["clinical"]: 
					seq_id=i["assembly_annotation"][0]["seq_id"]
					for j in i["clinical"]:
						line="rs"+rsid+"\t"+seq_id+"\t"+j["update_date"]+"\t"+j["accession_version"]
						for n in j["disease_names"]:
							line=line+"\t"+n
						line=line+"\n"
						outputfile.write(line)
						clinical=clinical+1#One clinical effect recorded

			#get variants on base pairs
			for i in json_obj["primary_snapshot_data"]["placements_with_allele"]:
				if i["alleles"]:
					line="rs"+rsid+"\t"+i["seq_id"]
					for j in i["alleles"]:
						line=line+"\t"+"["+j["allele"]["spdi"]["deleted_sequence"]
						line=line+">"+j["allele"]["spdi"]["inserted_sequence"]+"]"
					line=line+"\n"
					outputfile2.write(line)
					alleles=alleles+1#One allele recorded


			log="rs"+rsid+"\t0\trecorded details:\n\t"+str(alleles)+" alleles recorded\n\t"+str(clinical)+" clinical effects recorded\n"
			print (log)
			logfile.write(log)
			#print json.dumps(json_obj["primary_snapshot_data"]["allele_annotations"]["clinical"], indent=4, sort_keys=True)
		else:
			log="rs"+rsid+"\t2\tfail to connect to website: " + response.status + " " + response.reason+"\n"
			print log
			logfile.write(log)

	except Exception, e:
		log="rs"+rsid+"\t3\tfail to set up httpClient: " + str(e) + "\n"
		print log
		logfile.write(log)
	finally:
	    if httpClient:
	        httpClient.close()


# main
inputfilename="clinical_rsid.txt"	
outputfilename="clinical_effects.txt"
outputfilename2="alleles.txt"
logfilename="log.txt"

inputfile = open(inputfilename, "r")



for i in inputfile:
	i=i.replace('\n', '')
	i=i.replace('rs', '')
	outputfile = open(outputfilename, "a")
	outputfile2 = open(outputfilename2, "a")
	logfile = open(logfilename, "a")
	time.sleep(0.5)
	search_rsid( i, outputfile, logfile)
	logfile.close()
	outputfile.close()
	outputfile2.close()


inputfile.close()