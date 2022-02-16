#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "fuzzylogic.h"

using std::max;
using std::min;

// By: Bassel Yasser Al-Ashram (20176042)
int main(int argc, char *argv[])
{
	int variables;
	float project_fund;
	float experience_level;

	// Opening file and parsing input

	std::ifstream input("input.txt");
	input >> variables;
	while(variables--)
	{
		input.ignore();
		std::string line;
		std::getline(input, line);
		if(line == "Project Fund:")
		{
			input >> project_fund;
		}
		else if(line == "Experience Level:")
		{
			input >> experience_level;
		}
	}
	
	// Calculating membership values

	fl::pf_very_low = fl::getMembership(project_fund, fl::project_funding[0], 4);
	fl::pf_low = fl::getMembership(project_fund, fl::project_funding[1], 4);
	fl::pf_medium = fl::getMembership(project_fund, fl::project_funding[2], 4);
	fl::pf_high = fl::getMembership(project_fund, fl::project_funding[3], 4);

	fl::tel_beginner = fl::getMembership(experience_level, fl::team_experience_level[0], 3);
	fl::tel_intermediate = fl::getMembership(experience_level, fl::team_experience_level[1], 3);
	fl::tel_expert = fl::getMembership(experience_level, fl::team_experience_level[2], 3);

	// Applying inference rules

	fl::risk_low = max(fl::pf_high, fl::tel_expert);
	fl::risk_normal = min(fl::pf_medium, max(fl::tel_intermediate, fl::tel_beginner));
	fl::risk_high = max(fl::pf_very_low, min(fl::pf_low, fl::tel_beginner));

	// Weighted average method of defuzzification

	// Calculating centroids

	fl::c_low = fl::getCentroid(fl::risk[0], 3);
	fl::c_normal = fl::getCentroid(fl::risk[1], 3);
	fl::c_high = fl::getCentroid(fl::risk[2], 3);

	float result = (fl::risk_low * fl::c_low) + (fl::risk_normal * fl::c_normal) + (fl::risk_high * fl::c_high);
	result /= (fl::risk_low + fl::risk_normal +fl::risk_high);

	// Output and calculating result's membership

	std::cout << "Predicted Value (Risk) = " << result << std::endl;
	std::cout << "Risk will be ";

	fl::risk_low = fl::getMembership(result, fl::risk[0], 3);
	fl::risk_normal = fl::getMembership(result, fl::risk[1], 3);
	fl::risk_high = fl::getMembership(result, fl::risk[2], 3);

	float mx = max(fl::risk_low, max(fl::risk_normal, fl::risk_high));

	if(fl::compareFloatsEqual(mx, fl::risk_low) && !fl::compareFloatsEqual(fl::risk_low, fl::risk_normal))
	{
		std::cout << "Low";
	}
	
	else if(fl::compareFloatsEqual(mx, fl::risk_normal) && !fl::compareFloatsEqual(fl::risk_normal, fl::risk_high))
	{
		std::cout << "Normal";
	}

	else
	{
		std::cout << "High";
	}
	
	return 0;
}