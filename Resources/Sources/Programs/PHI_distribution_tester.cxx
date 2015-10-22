#include "./../Includes/Timer/Timer.hh"
#include "./../Includes/Input_output_interface/Input_output_interface.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"

#include <iostream>
#include <string>
#include <sstream>

# define M_PI 3.14159265358979323846

int main( int argc, char** argv )
{

	/****************************************************************************************************
	 *                                      Setting up the project                                      *
	 ****************************************************************************************************/

	system( "clear" );
	std :: cout << "Process: " << argv[0] << " is being executed..." << std :: endl;
	//new TApplication( "Application", &argc, argv );
	//new TBrowser();

	/****************************************************************************************************
	 *                                 Instantiations and initalizations                                *
	 ****************************************************************************************************/
	
	float PHI_parameter_min            = 0.0;
	float PHI_parameter_max            = M_PI;
	float PHI_parameter_step           = 0.01;

	Timer* timer = new Timer();
	Input_output_interface* input_output_interface = new Input_output_interface();

	/****************************************************************************************************
	 *                                          Configuration                                           *
	 ****************************************************************************************************/

	timer -> restart( "Process: configuring parameters..." );
	input_output_interface -> set_output_file( "Results/PHI_distribution.root", "RECREATE" );
	timer -> print_seconds( "Process: Took about ", " second(s)" );
	
	/****************************************************************************************************
	 *                                       Performing operations                                      *
	 ****************************************************************************************************/

	TF1* hit_distribution_function = new TF1( "hit_distribution_function", "1.0+([0]*cos([1] + x))", PHI_parameter_min, PHI_parameter_max );
	hit_distribution_function -> SetParameter( 0, 0.2 );
	hit_distribution_function -> SetParameter( 1, -0.5 * M_PI );
	//hit_distribution_function -> SetParameter( 2, 1.0 );
	int hit_distribution_numbins = (PHI_parameter_max - PHI_parameter_min) / PHI_parameter_step + 1;
	TH1F* hit_distribution = new TH1F( "hit_distribution", "hit_distribution", hit_distribution_numbins, PHI_parameter_min, PHI_parameter_max );
	for( int current_particle_index = 0; current_particle_index < 10000000; ++current_particle_index )
	{
		float position = hit_distribution_function -> GetRandom();
		hit_distribution -> Fill( position );
	}
	input_output_interface -> save_histogram( hit_distribution );

	/****************************************************************************************************
	 *                                            Terminating                                           *
	 ****************************************************************************************************/

	std :: cout << "Process: " << argv[0] << " terminated succesfully." << std :: endl;
	return 0; 

}
