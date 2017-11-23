#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include "grill.h"

Grill::Grill(){

}

// Switches the kebabs at pos1 and pos2
// Returns true if positions are valid and swap can be performed
// Returns false if positions are invalid
bool Grill::swap(unsigned int pos1, unsigned int pos2){ 
	
	//pos1 and pos2 are 1-indexed positions, if not in the range, return false
	if(pos1 <= 0 || pos2 <= 0 || pos1 > rack.size() || pos2 > rack.size()) {

		return false;
	}

	int max_pos = 0;
	int min_pos = 0;

	//if swap the same kebab
	if (pos1 == pos2) {
		
		return true;
	}


	if( pos1 < pos2 ) {
		
		min_pos = pos1 - 1;
		max_pos = pos2 - 1;

	} else {
		
		min_pos = pos2 - 1;
		max_pos = pos1 - 1;
	}
	
	//pop two kebabs out from the rack linkedlist, must pop the one at back first then the one at front
	Kebab* kebab1 = rack.removeAt(max_pos);
	Kebab* kebab2 = rack.removeAt(min_pos);

	//insert them back, the one at front first then the one at back
	return rack.insertAt(kebab1, min_pos) && rack.insertAt(kebab2, max_pos);
}

// remove and return the kebab at a 1-indexed
// position on the grill
// return NULL if pos invalid
Kebab* Grill::request(unsigned int pos){

    return rack.removeAt(pos - 1);
}

// remove and return the leftmost kebab matching
// at least the ingredients of r, in any order
// return NULL if no match found
Kebab* Grill::request(Kebab& r){
    
    for(unsigned int i = 0; i < rack.size(); i++ ) {

		if(rack.elementAt(i)->hasSameIngredients(r)){
			return rack.removeAt(i);
		}
	
	}

    return NULL;
}

void Grill::addKebab(Kebab& n){
//add to the rightmost position which is the back of the linkedlist rack
	rack.insertBack(&n);
}

// arrange in decreasing order of # of meat items
void Grill::sortByMeat(){
	// this is an insertion sort algorithm. I have adapted the example found at:
	// http://www.geeksforgeeks.org/insertion-sort/ 

	int j;
	Kebab *key;

	for( int i = 1; i < (int) rack.size(); i++ ) {
		key = rack.elementAt( (unsigned int) i );
		j = i - 1;

		// move kebabs in rack of positions 0 to i-1 that have more meat than key
		// one position ahead of where they currently are
		while( (j >= 0) && (rack.elementAt( (unsigned int) j )->numMeats() < key->numMeats() ) ) {
			
			// put kebab at j into j+1
			rack.removeAt( (unsigned int)(j+1) );
			rack.insertAt( rack.elementAt( (unsigned int)j ), (unsigned int)(j+1) );
			j--;
		}
		// put key kebab into the rack at position j+1
		rack.removeAt( (unsigned int)(j+1) );
		rack.insertAt( key, (unsigned int)(j+1) );
	}
	
}

// returns a vector containing the string representation of all
//   kebabs on the grill
std::vector<std::string> Grill::showKebabs() const{

	std::vector<std::string> kebabs;

	for(unsigned int i = 0; i < rack.size(); i++ ) {

		std::string kebab = rack.elementAt(i)->listIngredients();

		kebabs.push_back(kebab);
	}
	
	return kebabs;
    
}
