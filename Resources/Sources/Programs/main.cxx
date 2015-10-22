#include "./../Includes/Timer/Timer.hh"
#include "./../Includes/Elliptic_flow_simulation_v1/Elliptic_flow_simulation.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
//#include "TSystem.h"
#include "TApplication.h"

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
	// new TApplication( "Application", &argc, argv );
	// new TBrowser();

	/****************************************************************************************************
	 *                                 Instantiations and initalizations                                *
	 ****************************************************************************************************/
	
	int   generations_per_parameterset = 100;
	int   particle_number_min          = 1;
	int   particle_number_max          = 50;
	int   particle_number_step         = 1;
	float v_2_parameter_min            = 0.00;
	float v_2_parameter_max            = 0.5;
	float v_2_parameter_step           = 0.025;
	float PHI_parameter_min            = 0.0;
	float PHI_parameter_max            = M_PI;
	float PHI_parameter_step           = 0.2; // Min: 0.1

	Timer* timer = new Timer();
	Elliptic_flow_simulation* simulation = new Elliptic_flow_simulation();

	/****************************************************************************************************
	 *                                          Configuration                                           *
	 ****************************************************************************************************/

	timer -> restart( "Process: configuring parameters..." );
	simulation -> set_output_file( "Results/elliptic_flow_1.root", "RECREATE" );
	simulation -> set_number_of_generations( generations_per_parameterset );
	simulation -> set_parameter_range_num_particles( particle_number_min, particle_number_max, particle_number_step );
	simulation -> set_parameter_range_v2( v_2_parameter_min, v_2_parameter_max, v_2_parameter_step );
	simulation -> set_parameter_range_PHI( PHI_parameter_min, PHI_parameter_max, PHI_parameter_step );
	simulation -> init_elliptic_flow_histogram();
	timer -> print_seconds( "Process: Took about ", " second(s)." );
	
	/****************************************************************************************************
	 *                                       Performing operations                                      *
	 ****************************************************************************************************/

	timer -> restart( "Process: Running simulation..." );
	simulation -> start_simulation();
	timer -> print_seconds( "Process: Took about ", " second(s)." );

	/****************************************************************************************************
	 *                                            Terminating                                           *
	 ****************************************************************************************************/

	simulation -> close_output_file();
	//delete simulation;
	std :: cout << "Process: " << argv[0] << " terminated succesfully." << std :: endl;
	return 0; 

}
