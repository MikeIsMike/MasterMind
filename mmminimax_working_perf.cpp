#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <set>
#include <map>

/// you can add other headers as needed
/// but only headers from the standard library
/// and not the algorithm header

/// do not use using namespace std

/// functions for random number generation, do not alter the declarations
void set_random_seed();
int randn(int n);

/// you can define and use additional functions and structs,
/// add here the declarations for any other functions 
/// and/or structs you wish to define and use 
/// (the implementation for functions that don't belong to a struct
/// should be added after the main)



/// this is the struct definition for the code maker 
/// do not alter the name
struct mm_code_maker{
    
    /// this member function sets the values for the member data
    /// representing the length of the code
    /// and the number of symbols (the symbols will be 0 to i_num - 1)
    /// (this should be a constructor in proper OOP but ok)
    /// do not alter this function
    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;
    }
    
    /// this member function generates a random sequence based 
    /// on the length and num parameters stored as member data
    /// do not alter this function
    void generate_sequence(){
        for(int i = 0; i < length; i++){
            sequence.push_back(randn(num));
        }
    }
    
    /// do not alter name and parameter list for this function
	void give_feedback(const std::vector<int>& attempt, int& black_hits, int& white_hits) {
		black_hits = 0;
		white_hits = 0;
		std::vector<int> tmp = sequence;
		std::vector<int> copy_attempt = attempt;
		for (int i = 0; i < attempt.size(); i++) {
			if (attempt[i] == tmp[i]) {
				black_hits++;
			}
		}
		for (int i = 0; i < attempt.size(); i++) {
			for (int j = 0; j < attempt.size(); j++) {
				if (copy_attempt[i] == tmp[j] && copy_attempt[j] != tmp[j] && copy_attempt[i] != tmp[i] && copy_attempt[i] != -1) {
					tmp[j] = -1;
					copy_attempt[i] = -1;
					white_hits++;
				}
			}
		}
	}

    
    /// member data holding the sequence generated by generate_sequence
    /// do not alter this
    std::vector<int> sequence;
    
    /// member data holding the values for length of code and number of symbols
    /// do not alter these
    int length;
    int num;
    
    /// do not add any other member data, 
    /// in particular note that any variables needed for function give_feedback
    /// need to be declared within give_feedback 
    /// (they are not member data of the struct)
    
    /// you may add other member functions if needed
	void change_sequence(std::vector<int>& i_sequence){
		sequence = i_sequence;
	}


};


/// this is the struct definition for the solver, do not alter the name
struct mm_solver{
    
    /// this member function sets the values for the member data
    /// representing the length of the code
    /// and the number of symbols (the symbols will be 0 to i_num - 1)
    /// (this should be a constructor in proper OOP but ok)
    /// do not alter the function interface (name, parameter list, void return)
    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;
		if ((length < 4) || ((length == 4) && (num <= 50)) || ((length == 5) && (num <= 23)) || ((length == 6) && (num <= 14)) || ((length == 7) && (num <= 9))) {
			generate_set(i_length, i_num);
			initialise_potentialSet();
			generate_all_hits(i_length);
		}
		else if ((length == 8) || (length == 9) || (length == 10) || ((length == 4) && (num>50)) || ((length == 5) && (num>23)) || ((length == 6) && (num>14)) || ((length == 7) && (num>9))) {
			done = false;
		}
		else {
			triedEveryNum = false;
			numIndex = 0;
		}
		possibleIndex = 0;
		presentNum = 0;
		
        
        /// you can include additional implementation lines here if needed
        
    }
    
    /// this member function creates an attempt to find the right code
    /// (see the other examples provided for clarifications)
    /// do not alter the function interface (name, parameter list, void return)
	void create_attempt(std::vector<int>& attempt) {
		/// write your implementation here
		if ((length < 4) || ((length == 4) && (num <= 50)) || ((length == 5) && (num <= 23)) || ((length == 6) && (num <= 14)) || ((length == 7) && (num <= 9))) {
			if (pow(num, length) > 1300) {
				create_attempt_rand(attempt);
			}
			else {
				create_attempt_minimax(attempt);
			}
			
		}
		else if ((length == 8) || (length == 9) || (length == 10) || ((length == 4) && (num>50)) || ((length == 5) && (num>23)) || ((length == 6) && (num>14)) || ((length == 7) && (num>9))) {
			create_attempt_midSet(attempt);
		}
		else {
			create_attempt_bigSet(attempt);
		}
		
	}
		
    
    /// this member function acquires the feedback about a certain attempt
    /// (see the other examples provided for clarifications)
    /// do not alter the function interface (name, parameter list, void return)
    void learn(std::vector<int>& attempt, int black_hits, int white_hits){
        /// write your implementation here

		if ((length < 4) || ((length == 4) && (num <= 50)) || ((length == 5) && (num <= 23)) || ((length == 6) && (num <= 14)) || ((length == 7) && (num <= 9))) {
			learn_smallSet(attempt, black_hits, white_hits);
		}
		else if((length==8)||(length==9)||(length==10)||((length==4)&&(num>50))||((length==5)&&(num>23))||((length==6)&&(num>14))||((length==7)&&(num>9))){
			learn_midSet(attempt, black_hits, white_hits);
		}
		else {
			learn_bigSet(attempt, black_hits, white_hits);
		}
    }
    
    int length;
    int num;
	
	
    
    /// you may add other member functions and member data as needed
    /// (keep in mind the distinction between member function variables
    /// and member data of the struct)
	std::vector<std::vector<int> > fullSet;
	std::vector<std::vector<int> > potentialSet;
	std::vector< std::vector<int> > hitSet;
	std::set<std::vector<int> > triedSet;
	bool triedEveryNum;
	bool done;
	std::vector<int> prevAttempt;
	std::vector<int> possibleNums;
	int numIndex;
	int possibleIndex;
	int prev_black_hits;
	int presentNum;


    
    void generate_set(int length, int num){
    	int element1 = 0;
		std::vector<int> elementarr;
		for (int i = 0; i < length; i++) {
			elementarr.push_back(0);
		}
    	while(element1<pow(num, length)){
			std::vector<int> tmp;
			tmp = elementarr;
    		int element2 = element1;
    		for(int i=0; i<length; i++){
    			int digit = element2%num;
    			element2=element2/num;
    			tmp[length-i-1]=digit;
    		}
    		fullSet.push_back(tmp);
    		element1++;
    	}
    }


	void initialise_potentialSet() {
		potentialSet = fullSet;
	}


	void generate_all_hits(int length) {
		
		for (int i = 0; i <= length; i++) {
			for (int j = 0; j <= length; j++) {
				if ((i + j <= length)&&!((j==1)&&(i==length-1))) {
					std::vector<int> hitCondition;
					hitCondition.push_back(i); //push_back black hit first
					hitCondition.push_back(j);
					hitSet.push_back(hitCondition);
				}
			}
		}
	}
	

	void initialise_hitMap(std::map<std::vector<int>, int>& hitMap){
		for(int i=0; i<hitSet.size(); i++){
					hitMap.insert(std::pair<std::vector<int>, int>(hitSet[i], 0));
				}
	}


	void create_attempt_rand(std::vector<int>& attempt) {
		attempt = potentialSet[randn(potentialSet.size())];
		/*bool ready = false;
		while (!ready) {

			for (int i = 0; i < length; i++) {
				attempt.push_back(randn(num));
			}

			bool found = false;
			for (int i = 0; i < potentialSet.size() && !found; i++) {
				if (attempt == potentialSet[i]) {
					found = true;
				}
			}

			if (!found) {
				/// the following printing can be used for testing purposes in order
				/// to see how much of an improvement this version is on the previous one
				/// std::cout << "this was already attempted, resetting" << std::endl;
				attempt.clear();
			}
			else {
				ready = true;
			}
		}*/
	}


	void create_attempt_minimax(std::vector<int>& attempt) {
		if (potentialSet.size() <= 2) {
			attempt = potentialSet[0];
		}
		else if (potentialSet.size() == fullSet.size()) {
			for (int i = 0; i < length / 2; i++) {
				attempt.push_back(0);
			}
			for (int i = length / 2; i < length; i++) {
				attempt.push_back(1);
			}

		}
		else {
			mm_code_maker tmpMaker;
			int min = fullSet.size();
			for (int i = 0; i < fullSet.size(); i++) {
				std::map<std::vector<int>, int> hitMap;
				initialise_hitMap(hitMap);
				if (!triedSet.count(fullSet[i])) {
					int maxSetSize = 0;
					for (int j = 0; j < potentialSet.size(); j++) {
						int black_hits = 0, white_hits = 0;
						tmpMaker.change_sequence(potentialSet[j]);
						tmpMaker.give_feedback(fullSet[i], black_hits, white_hits);
						std::vector<int> tmpHit;
						tmpHit.push_back(black_hits);
						tmpHit.push_back(white_hits);
						hitMap[tmpHit] += 1;

					}
					std::map<std::vector<int>, int>::iterator it = hitMap.begin();
					while (it != hitMap.end())
					{
						if (maxSetSize < it->second) {
							maxSetSize = it->second;
						}
						it++;
					}
					if (maxSetSize < min) {
						min = maxSetSize;
						attempt = fullSet[i];
					}
				}

			}
			triedSet.insert(attempt);
		}
	}


	void initialise_prevAttem(int
			i_length){
		for(int i=0; i<i_length; i++){
			prevAttempt.push_back(0);
		}

	}


	void create_attempt_bigSet(std::vector<int>& attempt) {
		if (!triedEveryNum) {
			for (int j = 0; j<length; j++) {
				attempt.push_back(presentNum);
			}
		}
		else {
			attempt = prevAttempt;
			attempt[numIndex] = possibleNums[possibleIndex];

		}
	}


	void learn_bigSet(std::vector<int>& attempt, int black_hits, int white_hits){
		if (!triedEveryNum) {
			if (attempt[0] == 0) {
				prevAttempt = attempt;
				prev_black_hits = black_hits;
			}
			for (int i = 0; i<black_hits; i++) {
				possibleNums.push_back(presentNum);
			}
			presentNum++;
			if (presentNum >= num) {
				triedEveryNum = true;
			}
		}
		else {
			if (prev_black_hits > black_hits ) {
				possibleIndex--;
				prevAttempt[numIndex] = possibleNums[possibleIndex];
				numIndex++;
				possibleNums.erase(possibleNums.begin() + possibleIndex);
				possibleIndex = 0;
			}
			else if (prev_black_hits < black_hits) {
				numIndex++;
				possibleNums.erase(possibleNums.begin() + possibleIndex);
				possibleIndex = 0;
				prevAttempt = attempt;
				prev_black_hits = black_hits;
			}
			else {
				possibleIndex++;
				prev_black_hits = black_hits;
			}
		}
	}


	void learn_smallSet(std::vector<int>& attempt, int black_hits, int white_hits) {
		int i = 0;
		mm_code_maker tmpMaker;
		std::vector<std::vector<int> > potentialSetAux; //auxiliary potentialSet
		int aux_black_hits = 0;
		int aux_white_hits = 0;
		tmpMaker.change_sequence(attempt);
		while (i < potentialSet.size()) {
			tmpMaker.give_feedback(potentialSet[i], aux_black_hits, aux_white_hits);
			if ((aux_black_hits == black_hits) && (aux_white_hits == white_hits)) {
				potentialSetAux.push_back(potentialSet[i]);
			}
			i++;

		}
		potentialSet = potentialSetAux;
	}


	void create_attempt_midSet(std::vector<int>& attempt) {
		if (presentNum<num) {
			for (int j = 0; j < length; j++) {
				attempt.push_back(presentNum);
			}
		}
		else if(done){
			create_attempt_rand(attempt);
		}
		else if(potentialSet.size()==0){
			for(int i=0; i<length; i++){
				attempt.push_back(0);
			}
			potentialSet.push_back(attempt);
		}
	}

	void learn_midSet(std::vector<int>& attempt, int black_hits, int white_hits) {
		if (presentNum<num) {
			if (attempt[0] == 0) {
				prevAttempt = attempt;
				prev_black_hits = black_hits;
			}
			for (int i = 0; i<black_hits; i++) {
				possibleNums.push_back(presentNum);
			}
			presentNum++;
		}
		else {
			if (!done) {
				generate_permutation(length, possibleNums);
				done = true;
				potentialSet = fullSet;
			}
			learn_smallSet(attempt, black_hits, white_hits);
		}
	}

	void generate_permutation(int i_length, std::vector<int>& possibleNums) {
		int tmp[i_length];
		for (int i = 0; i < i_length; i++) {
			tmp[i] = 0;
		}

		fullSet.push_back(possibleNums);

		int i = 0;
		while (i < i_length) {
			if (tmp[i] < i) {
				if (i % 2 == 0) {
					int aux = possibleNums[0];
					possibleNums[0] = possibleNums[i];
					possibleNums[i] = aux;
				}
				else {
					int aux = possibleNums[tmp[i]];
					possibleNums[tmp[i]] = possibleNums[i];
					possibleNums[i] = aux;
				}
				fullSet.push_back(possibleNums);
				tmp[i]++;
				i = 0;
			}
			else {
				tmp[i] = 0;
				i++;
			}
		}
			
		
	}




};

/// before the submission you need to remove the main
/// (either delete it or comment it out)
/// otherwise it will intefere with the automated testing

/*
int main(){
    /// write the code for the main here in order to test your functions
	set_random_seed();

	int length, num;
	//std::vector< std::vector<int> > fullset;
    std::cout << "enter length of sequence and number of possible values:" << std::endl;
    std::cin >> length >> num;



    
	double sum;
	double totalduration;
	
    /// just some number to limit the number of attempts
	for (int i = 0; i < 100; i++) {
		std::clock_t start;
		double duration;
		start = std::clock();

		mm_solver solver;
		/// we declare an object of type mm_solver
		solver.init(length, num);
		/// we initialise the values for length and num

		mm_code_maker maker;
		/// we declare an object of type mm_code_maker
		maker.init(length, num);
		/// we initialise the values for length and num
		
		//maker.change_sequence(solver.fullSet[i]);
		maker.generate_sequence();
		/// we generate a random sequence
		/// we declare an object of type mm_solver
		int black_hits=0, white_hits=0;
		int number = 0;
		std::vector<int> att;
		while (black_hits<length) {
			std::vector<int> attem;
			solver.create_attempt(attem);
			maker.give_feedback(attem, black_hits, white_hits);

			solver.learn(attem, black_hits, white_hits);

			number++;
		}
		std::cout << number << std::endl;
		sum += number;
		

		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		totalduration += duration;
		std::cout << "printf: " << duration << std::endl;
	}
	std::cout << sum / 100 << std::endl;
	std::cout << totalduration / 100 << std::endl;
	

    return 0;
}
*/

/// not a great implementation for set_random_seed and for randn;
/// if you are trying to get better results you may want to change 
/// the implementation using C++11 features, see for instance
/// https://isocpp.org/files/papers/n3551.pdf
/// but don't change the interface/declaration of the functions

void set_random_seed(){
    std::srand(std::time(0));
}

int randn(int n){
    return std::rand() % n;
}

/// add here the implementation for any other functions you wish to define and use
