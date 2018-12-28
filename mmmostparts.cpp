#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <set>

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
    /*void give_feedback(const std::vector<int>& attempt, int& black_hits, int& white_hits){
    	std::vector<int> sequence_copy = sequence;
    	std::vector<int> attempt_copy = attempt;
    	int i1 = 0;
		black_hits = 0;
		white_hits = 0;
    	while(i1<attempt_copy.size()){ //find black hits
    		if(sequence_copy[i1]==attempt_copy[i1]){
    			black_hits++;
    			attempt_copy.erase(attempt_copy.begin()+i1);
    			sequence_copy.erase(sequence_copy.begin()+i1);
    		}
    		else{
    			i1++;
    		}
    	}

    	int i=0, j=0;
    	bool found = false;
    	while(i<attempt_copy.size()){
    		while((j<attempt_copy.size())&&(!found)){
    			if(attempt_copy[i]==sequence_copy[j]){
    				white_hits++;
    				attempt_copy.erase(attempt_copy.begin()+i);
    				sequence_copy.erase(sequence_copy.begin()+j);
    				found = true;
    			}
    			else{
    				j++;
    			}
    		}
    		if(found){
    			found=false;
    			j=0;
    		}
    		else{
    			i++;
    			j=0;
    		}

    	}
        /// write here your implementation for this function 
        /// which takes in input an attempt
        /// and provides feedback in terms of black hits 
        /// and white hits (see linked paper)
    }*/
    
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
	void change_sequence(std::vector<int> i_sequence){
		sequence = i_sequence;
	}

	std::vector<int> get_sequence() {
		return sequence;
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
		generate_set(i_length, i_num);
		create_maker(i_length, i_num);
		initialise_potentialSet();
		
        
        /// you can include additional implementation lines here if needed
        
    }
    
    /// this member function creates an attempt to find the right code
    /// (see the other examples provided for clarifications)
    /// do not alter the function interface (name, parameter list, void return)
	void create_attempt(std::vector<int>& attempt) {
		/// write your implementation here
		if (potentialSet.size() == 1) {
			attempt = potentialSet[0];
		}
		else if (potentialSet.size() == fullSet.size()) {
			for (int i = 0; i < length / 2; i++) {
				attempt.push_back(0);
			}
			for (int i = length / 2; i < length*3/4; i++) {
				attempt.push_back(1);
			}
			for(int i=length*3/4; i<length; i++){
                            attempt.push_back(2);
                        }
		}
		else {
			mm_code_maker tmpMaker;
			std::vector<int> scores;
			for (int i = 0; i < fullSet.size(); i++) {
				std::vector<int> tmpAttempt;
				tmpAttempt = fullSet[i];
				std::set<std::vector<int> > partitions;

				for (int j = 0; j < potentialSet.size(); j++) {
					tmpMaker.change_sequence(potentialSet[j]);
					partitions.insert(scoring_each(tmpAttempt, tmpMaker));
				}
				scores.push_back(partitions.size());
			}
			attempt = fullSet[find_max_index(scores)];
		}
	}
		
    
    /// this member function acquires the feedback about a certain attempt
    /// (see the other examples provided for clarifications)
    /// do not alter the function interface (name, parameter list, void return)
    void learn(std::vector<int>& attempt, int black_hits, int white_hits){
        /// write your implementation here
		
		int i=0;
		mm_code_maker tmpMaker;
		std::vector<std::vector<int> > potentialSetAux; //auxiliary potentialSet

		while (i < potentialSet.size()) {
			int aux_black_hits = 0;
			int aux_white_hits = 0;
			tmpMaker.change_sequence(potentialSet[i]);
			tmpMaker.give_feedback(attempt, aux_black_hits, aux_white_hits);
			if ((aux_black_hits == black_hits) && (aux_white_hits == white_hits)) {
				potentialSetAux.push_back(potentialSet[i]);
			}
			i++;
			
		}
		potentialSet = potentialSetAux;
    }
    
    int length;
    int num;
	
	
    
    /// you may add other member functions and member data as needed
    /// (keep in mind the distinction between member function variables
    /// and member data of the struct)
	std::vector< std::vector<int> > fullSet;
	std::vector< std::vector<int> > potentialSet;
	std::vector<int> sequence;
	mm_code_maker maker;
    
    void generate_set(int length, int num){
    	int size = pow(num,length);
    	int element1 = 0;
    	int elementarr[length];
    	while(element1<size){
    		int element2 = element1;
    		for(int i=0; i<length; i++){
    			int digit = element2%num;
    			element2=element2/num;
    			elementarr[length-i-1]=digit;
    		}
    		std::vector<int> elementvec(elementarr, elementarr+length);
    		fullSet.push_back(elementvec);
    		element1++;
    	}
    }



	void create_maker(int i_length, int i_num) {
		maker.init(i_length, i_num);
		maker.generate_sequence();
		sequence = maker.get_sequence();
	}

	void initialise_potentialSet() {
		potentialSet = fullSet;
	}

	bool over(std::vector<int> attempt) {
		if (sequence == attempt) {
			return true;
		}
		return false;
	}

	std::vector<int> scoring_each(std::vector<int> attempt, mm_code_maker tmpMaker) {
		int black_hits = 0, white_hits = 0;
		tmpMaker.give_feedback(attempt, black_hits, white_hits);
		std::vector<int> score;
		score.push_back(black_hits);
		score.push_back(white_hits);
		return score;
	}

	int find_max_index(std::vector<int> scores) {
		int max = 0;
		int index = 0;
		for (int i = 0; i < scores.size(); i++) {
			if (scores[i] > max) {
				max = scores[i];
				index = i;
			}
		}
		return index;
	}
	
	






};

/// before the submission you need to remove the main
/// (either delete it or comment it out)
/// otherwise it will intefere with the automated testing

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
	for (int i = 0; i < 1296; i++) {
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
		
		maker.change_sequence(solver.fullSet[i]);
		//maker.generate_sequence();
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
	std::cout << sum / 1296 << std::endl;
	std::cout << totalduration / 1296 << std::endl;
	

    return 0;
}

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