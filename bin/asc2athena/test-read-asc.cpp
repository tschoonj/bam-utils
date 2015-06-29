#include "bam_file_asc.h"

int main(int argc, char *argv[]) {
	
	if (argc != 2) {
		std::cerr << "USAGE: " << argv[0] << " filename" << std::endl;
		return 1;
	}

	BAM::ASC data(argv[1]);

	return 0;
}
