#ifndef BAM_FILE_ASC_H
#define BAM_FILE_ASC_H

#include <string>
#include <iostream>
#include <vector>
#include <exception>

namespace BAM {
	class Exception: public std::exception {
	private:
		std::string Message;  
	public:
		// constructors
		//explicit Exception(const char *ch) : Message(ch) {}
		explicit Exception(const std::string &s) : Message(s) {}
		virtual ~Exception() throw() {};
		// throw method
		virtual const char* what() const throw() {
			return Message.c_str();
		}
	};

	class ASCSingle {
	private:
		double Energy;
		double Ioni_1;
		double Ioni_2;
		double Ioni_3;
		double BESSY;
		double Encoder;
		double Energy_Encoder;
		ASCSingle(std::string line);
		friend class ASC;

	};

	class ASC {
	private:
		std::vector<class ASCSingle> singles;
	public:
		ASC(std::string filename);	
	};


}



#endif