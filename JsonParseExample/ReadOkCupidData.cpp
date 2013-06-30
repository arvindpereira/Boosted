/*
 * ReadOkCupidData.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: arvind
 */

#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <json_spirit.h>

using namespace std;
using namespace json_spirit;

struct Answer {
	int questionId;
	int answer;
	vector <int>acceptableAnswers;
	int importance;

	bool operator==(const Answer &a2) {
		if (a2.acceptableAnswers.size()!= this->acceptableAnswers.size()) return false;
		else {
			for (int i=0; i<=a2.acceptableAnswers.size(); i++ ) {
				if ( this->acceptableAnswers[i]!=a2.acceptableAnswers[i] ) {
					return false;
				}
			}
			return (a2.questionId==this->questionId ) &&
					(a2.answer == this->answer);
		}
	}

	/** Test if the answer being passed matches my answer
	 *
	 * */
	bool isMatch( const Answer &a2 ) {
		if( this->questionId==a2.questionId ) {
			for( int i=0; i<a2.acceptableAnswers.size(); i++) {
				if(this->answer == a2.acceptableAnswers[i]) {
					return true;
				}
			}
			return false;
		}
		else throw invalid_argument("Question Ids don't match");
	}

	friend ostream& operator<<( ostream & os, const Answer &a ) {
		os<<"questionId:"<<a.questionId<<", answer:"<<a.answer<<", importance:"<<a.importance<<", acceptable answers: [ ";
		for( int i=0;i< a.acceptableAnswers.size(); i++ )
			os<< a.acceptableAnswers[i]<<", ";
		os<<"]\n";
		return os;
	}
};


struct Profile {
	int profileId;
	vector<Answer> ansArray;
	friend ostream & operator<<( ostream &os, const Profile prof ) {
		os<<"profileId :"<<prof.profileId<<", answers : [";
		for( int i=0;i<prof.ansArray.size(); i++ ) {
			os<<prof.ansArray[i]<<", ";
		}
		os<<" ]";
		return os;
	}
};


vector<int> get_acceptable_answers( const Array& acceptableAnswers ) {
	vector<int> acceptAnswers;
	for ( int i=0; i<acceptableAnswers.size(); i++ ) {
		acceptAnswers.push_back( acceptableAnswers[i].get_int() );
	}
	return acceptAnswers;
}

vector<Answer> get_answers( const Array& answers ) {
	vector<Answer> theAnswers;

	for (int j=0; j < answers.size(); j++ ) {
		const Object &eachAnswer = answers[j].get_obj();
		Answer anAns;

		for ( Object::size_type i=0; i!= eachAnswer.size(); i++ ) {
			const Pair& ansItems = eachAnswer[i];

			if( ansItems.name_ == "questionId" ) {
				anAns.questionId = ansItems.value_.get_int();
			}
			else if( ansItems.name_ == "answer" ) {
				anAns.answer = ansItems.value_.get_int();
			}
			else if( ansItems.name_ == "acceptableAnswers" ) {
				anAns.acceptableAnswers = get_acceptable_answers( ansItems.value_.get_array() );
			}
			else if( ansItems.name_ == "importance" ) {
				anAns.importance = ansItems.value_.get_int();
			}
		}
		theAnswers.push_back( anAns );
	}

	return theAnswers;
}

Profile read_profile( const Object& aProfile ) {
	Profile newProf;
	for( Object::size_type i=0; i!=aProfile.size(); i++ ) {
		const Pair& profItems = aProfile[i];

		if( profItems.name_ =="id" ) {
			newProf.profileId = profItems.value_.get_int();
		}
		else if( profItems.name_ =="answers" ) {
			const Value& answers = profItems.value_;
			const Array& allAnswers = answers.get_array();
			newProf.ansArray = get_answers( allAnswers );
		}
	}
	return newProf;
}


vector<Profile> read_profiles ( string file_name ) {

	ifstream is(file_name.c_str());
	Value value;
	try {
		read ( is, value );
	}
	catch ( json_spirit::Error_position &ep ) {
		cerr<<"Error on "<<ep.line_<<", at Column "<<ep.column_<<", :"<<ep.reason_<<endl;
		throw;
	}

	cout<<"Value Type = "<<int(value.type())<<endl;
	const Object& profile = value.get_obj();
	const Pair& pair = profile[0];
	const string &name = pair.name_;
	const Value& profiles = pair.value_;

	vector<Profile> allProfiles;

	if( name== "profiles" ) {
		const Array& profArray= profiles.get_array();

		for( int i=0; i<profArray.size(); i++ ) {
			const Object& profileObj = profArray[i].get_obj();
			allProfiles.push_back( read_profile( profileObj ) );
			cout<< allProfiles[i];
		}
	}

	return allProfiles;
}


int main() {
	string file_name = string("okcupid-input.json");

	vector<Profile> allProfiles = read_profiles( file_name );

	return 0;

}
