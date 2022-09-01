#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include "parallel_tracks.h"

using std::string; using std::ifstream; using std::istringstream; 
using std::cin; using std::cout; using std::endl; using std::string; 
using std::getline; using std::invalid_argument; using std::domain_error;

bool is_valid_country(string country){
	// Checks if country is 3 chars long
	if(country.size()!=3){
		return false;
	}
	// Checks if each char in country is a capital letter
	for(int i = 0; i < 3; i++){
		if(!isupper(country.at(i))){
			return false;
		}
	}
	return true;
}

bool is_valid_name(string name){
	// Checks if name is longer than 1 char
	if(trim(name).size() <= 1){
		return false;
	}
	// Checks if each char in name is alphabetical or a space
	for(size_t i = 0; i < name.size(); i++){
		if(!isalpha(name.at(i)) && !isspace(name.at(i))){
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------
// Name: get_runner_data
// PreCondition:  the prepped parallel arrays , and a legit filename is pass
// PostCondition: all arrays contain data from the text file given
//---------------------------------------------------------
void get_runner_data( const std::string& file, double timeArray[], std::string countryArray[], 
		unsigned int numberArray[], std::string lastnameArray[]) 
{
	ifstream inFS;
	string nextLine;
	istringstream inSS;
	int i = 0;

	// Checks if file is valid
	inFS.open(file);
	if(!inFS.is_open()){
		throw std::invalid_argument("Cannot open file");
	}
	// Fills the arrays with data from the file
	while(!inFS.eof() && i < 9){

		// Gets line from file
		getline(inFS,nextLine);
		// Checks if line is empty and throws domain_error if so
		if(trim(nextLine)==""){
			throw std::domain_error("File missing data");
		}

		// Splits line into elements and puts them into the arrays
		inSS.clear();
		inSS.str(nextLine);
		inSS >> timeArray[i]; 
		// Checks if time is valid
		if(inSS.fail() || timeArray[i]<0){
			throw std::domain_error("File contains invalid data (time)");
		}
		inSS >> countryArray[i];
		// Checks if country is valid
		if(inSS.fail() || !is_valid_country(countryArray[i])){
			throw std::domain_error("File contains invalid data (country)");
		}
		inSS >> numberArray[i];
		//Checks if number is valid
		if(inSS.fail() || numberArray[i] >= 100){
			throw std::domain_error("File contains invalid data (number)");
		}
		inSS >> lastnameArray[i];
		// Checks if last name is valid
		if(inSS.fail() || !is_valid_name(lastnameArray[i])){
			throw std::domain_error("File contains invalid data (name)");
		}
		i++;	
	}
	inFS.close();
}

//-------------------------------------------------------
// Name: prep_double_array
// PreCondition:  a double arrays is passed in
// PostCondition: data in the array is 'zeroed' out
//---------------------------------------------------------
void prep_double_array(double ary[])
// making sure all values within the array are set to 0.0;
{
  	for(unsigned int i = 0; i < SIZE; i++){
	  	ary[i] = 0.0;
  	}
}

//-------------------------------------------------------
// Name: prep_double_array
// PreCondition:  an unsigned int arrays is passed in
// PostCondition: data in the array is 'zeroed' out
//---------------------------------------------------------
void prep_unsigned_int_array(unsigned int ary[])
// making sure all values within the array are set to 0;
{
	for(unsigned int i = 0; i < SIZE; i++){
	  ary[i] = 0;
  	}
}

//-------------------------------------------------------
// Name: prep_string_array
// PreCondition:  a string arrays is pass in
// PostCondition: data in the array is "N/A" out to determine if
// a value has changed
//---------------------------------------------------------
void prep_string_array(std::string ary[])
// making sure all values within the array are set to "N/A";
{
	for(unsigned int i = 0; i < SIZE; i++){
	  ary[i] = "N/A";
    }
}

//-------------------------------------------------------
// Name: get_ranking
// PreCondition:  just the time array is passed in, and has valid data
// PostCondition: after a very inefficient nested loop to determine the placements 
// and places the ranks in a new array. That new array is returned
//---------------------------------------------------------
void get_ranking(const double timeArray[], unsigned int rankArray[])
{
	int numFaster = 0;
	for(unsigned int i = 0; i < SIZE; i++){
		numFaster = 0;
		for(unsigned int j = 0; j < SIZE; j++){
			if(timeArray[i] > timeArray[j]){
				numFaster++;
			}
		}	
		rankArray[i] = numFaster + 1;
	}
}


//-------------------------------------------------------
// Name: print_results
// PreCondition:  almost all parallel arrays are passed in and have valid data
// PostCondition: after a very inefficient nested loop to determine the ranks
// it then displays then along with a delta in time from the start
//---------------------------------------------------------
void print_results(const double timeArray[], const std::string countryArray[],
		const std::string lastnameArray[], const unsigned int rankArray[])
{

	std::cout << "Final results!!";
	std::cout << std::setprecision(2) << std::showpoint << std::fixed << std::endl;
	double best_time = 0.0;
		
	// print the results, based on rank, but measure the time difference_type
	for(unsigned int j = 1; j <= SIZE; j++)
	{
		
		// go thru each array, find who places in "i" spot
		for(unsigned int i = 0; i < SIZE; i++)
		{
			if(rankArray[i] == 1) // has to be a better way, but need the starting time
			{
				best_time = timeArray[i];
			}
			
			
			if(rankArray[i] == j) // then display this person's data
			{
				// this needs precision display
				std::cout << "[" << j << "]  " << timeArray[i] << " " << std::setw(15) << std::left << lastnameArray[i] << "\t" << "(" << countryArray[i] << ")  +" << (timeArray[i] - best_time) << std::endl; 
			}
			
		}
	}	
}

std::string trim(std::string ret) {
	// remove whitespace from the beginning
	while (!ret.empty() && isspace(ret.at(0))) {
			ret.erase(0, 1);
		}

	// remove whitespace from the end
	//  Note: last index is (.size() - 1) due to 0 based indexing
	while (!ret.empty() && isspace(ret.at(ret.size()-1))) {
		ret.erase(ret.size()-1, 1);
	}
	
	return ret;
}