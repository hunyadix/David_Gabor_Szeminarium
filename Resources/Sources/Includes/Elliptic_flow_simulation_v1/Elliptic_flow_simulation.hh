#ifndef ELLIPTIC_FLOW_SIMULATION_H
#define ELLIPTIC_FLOW_SIMULATION_H

#include "./../Input_output_interface/Input_output_interface.hh"
#include "./../Event_simulator/Event_simulator.hh"

#include "TF1.h"
#include "TCanvas.h"

#include <cmath>
#include <iomanip>

class Elliptic_flow_simulation : public Input_output_interface
{
	private:
		int   generations_per_parameterset = 0;
		int   particle_number_min          = 0;
		int   particle_number_max          = 0;
		int   particle_number_step         = 0;
		float v_2_parameter_min            = 0.0;
		float v_2_parameter_max            = 0.0;
		float v_2_parameter_step           = 0.0;
		float PHI_parameter_min            = 0.0;
		float PHI_parameter_max            = 0.0;
		float PHI_parameter_step           = 0.0;

		TH2F* unscaled_elliptic_flow_histogram = nullptr;
		TH2F* scaled_elliptic_flow_histogram   = nullptr;
		Event_simulator* event_simulator       = nullptr;
		void   check_elliptic_flow_histogram();
		void   configure_event_simulator();
		double get_flow_histogram_scale_factor();

	public:
		Elliptic_flow_simulation();
		~Elliptic_flow_simulation();
		void set_number_of_generations( int num_generations_p );
		void set_parameter_range_num_particles( int   min_p, int   max_p, int   step_p );
		void set_parameter_range_v2(            float min_p, float max_p, float step_p );
		void set_parameter_range_PHI(           float min_p, float max_p, float step_p );
		void init_elliptic_flow_histogram();
		void start_simulation();
		void save_elliptic_flow_histograms();
};

Elliptic_flow_simulation :: Elliptic_flow_simulation()
{
	this -> event_simulator = new Event_simulator();
}

Elliptic_flow_simulation :: ~Elliptic_flow_simulation()
{
	delete this -> event_simulator;
}

void Elliptic_flow_simulation :: check_elliptic_flow_histogram()
{
	if( !(this -> unscaled_elliptic_flow_histogram) )
	{
		std :: cerr << "Error: unscaled elliptic-flow histogram uninitialized." << std :: endl;
		exit( -1 );
	}
}

void Elliptic_flow_simulation :: configure_event_simulator()
{
	TF1* hit_distribution_function = new TF1( "hit_distribution_function", "(1+[0]*cos([1] + x))", this -> PHI_parameter_min, this -> PHI_parameter_max );
	TF1* estimating_function       = new TF1( "hit_estimating_function",   "(1+[0]*cos([1] + x))", this -> PHI_parameter_min, this -> PHI_parameter_max );
	int hit_distribution_numbins = (this -> PHI_parameter_max - this -> PHI_parameter_min) / this -> PHI_parameter_step + 1;
	this -> event_simulator -> set_distribution( hit_distribution_function );
	this -> event_simulator -> set_estimating_function( estimating_function );
	this -> event_simulator -> set_bin_settings( hit_distribution_numbins, this -> PHI_parameter_min, this -> PHI_parameter_max );
	this -> event_simulator -> get_distribution() -> SetParameter( 1, -0.5 * M_PI );
	this -> event_simulator -> get_estimating_function() -> SetParameter( 1, -0.5 * M_PI );
}

void Elliptic_flow_simulation :: set_number_of_generations( int num_generations_p )
{
	this -> generations_per_parameterset = num_generations_p;
}

void Elliptic_flow_simulation :: set_parameter_range_num_particles( int min_p, int max_p, int step_p )
{
	this -> particle_number_min  = min_p;
	this -> particle_number_max  = max_p;
	this -> particle_number_step = step_p;
}

void Elliptic_flow_simulation :: set_parameter_range_v2( float min_p, float max_p, float step_p )
{
	this -> v_2_parameter_min  = min_p;
	this -> v_2_parameter_max  = max_p;
	this -> v_2_parameter_step = step_p;
}

void Elliptic_flow_simulation :: set_parameter_range_PHI( float min_p, float max_p, float step_p )
{
	this -> PHI_parameter_min  = min_p;
	this -> PHI_parameter_max  = max_p;
	this -> PHI_parameter_step = step_p;
}

void Elliptic_flow_simulation :: init_elliptic_flow_histogram()
{
	int elliptic_flow_histo_numbins_x = (v_2_parameter_max   - v_2_parameter_min)   / v_2_parameter_step + 1;
	int elliptic_flow_histo_numbins_y = (particle_number_max - particle_number_min) / particle_number_step + 1;
	this -> unscaled_elliptic_flow_histogram = new TH2F( "Unscaled_elliptic_flow", "Elliptic flow without scaling;v_2;n_particles",
	                                                     elliptic_flow_histo_numbins_x, v_2_parameter_min, v_2_parameter_max,
	                                                     elliptic_flow_histo_numbins_y, particle_number_min, particle_number_max );
	this -> unscaled_elliptic_flow_histogram -> SetStats( kFALSE );
	this -> scaled_elliptic_flow_histogram = new TH2F( "", "", 1, 0.0, 0.0, 1, 0.0, 0.0 );
}

double Elliptic_flow_simulation :: get_flow_histogram_scale_factor()
{
	double elliptic_flow_histogram_scale_factor = 0;
	for( float PHI = this -> PHI_parameter_min; PHI <= this -> PHI_parameter_max; PHI += this -> PHI_parameter_step )
	{
		for( int scenario_times_generated = 0; scenario_times_generated < this -> generations_per_parameterset; scenario_times_generated++ )
		{
			++elliptic_flow_histogram_scale_factor;
		}
	}
	elliptic_flow_histogram_scale_factor = 1.0 / elliptic_flow_histogram_scale_factor;
	return elliptic_flow_histogram_scale_factor;
}

void Elliptic_flow_simulation :: start_simulation()
{
	this -> check_elliptic_flow_histogram();
	this -> configure_event_simulator();
	this -> event_simulator -> get_distribution() -> SetParameter( 0, 0.2 );
	this -> save_histogram( this -> event_simulator -> get_distribution_sample( 100000 ) );
	for( float v_2 = this -> v_2_parameter_min; v_2 <= this -> v_2_parameter_max; v_2 += this -> v_2_parameter_step )
	{
		this -> event_simulator -> get_distribution()        -> SetParameter( 0, v_2 );
		this -> event_simulator -> get_estimating_function() -> FixParameter( 0, v_2 );
		float debug_percent = (v_2 - v_2_parameter_min) / (v_2_parameter_max - v_2_parameter_min) * 100;
		std :: cout << "Debug: Running process. Progression: " << std :: setprecision(2) << debug_percent << "%." << std :: endl;
		for( int n_particles = this -> particle_number_min; n_particles <= this -> particle_number_max; n_particles += this -> particle_number_step )
		{
			float estimated_mean_difference = this -> event_simulator -> simulate_n_events_fit_method( this -> generations_per_parameterset, n_particles );
			this -> unscaled_elliptic_flow_histogram -> Fill( v_2, n_particles, estimated_mean_difference );
		}
	}
	std :: cout << "Debug: Running process. Progression: 100%." << std :: endl;
	this -> unscaled_elliptic_flow_histogram -> Copy( *scaled_elliptic_flow_histogram );
	this -> scaled_elliptic_flow_histogram -> Scale( this -> get_flow_histogram_scale_factor() );
	scaled_elliptic_flow_histogram -> SetName( "Scaled_elliptic_flow_histogram" );
	scaled_elliptic_flow_histogram -> SetTitle( "Elliptic flow with scaling;v_2;n_particles");
	this -> save_elliptic_flow_histograms();
}

void Elliptic_flow_simulation :: save_elliptic_flow_histograms()
{
	TCanvas* unscaled_elliptic_flow_histogram_canvas = new TCanvas();
	unscaled_elliptic_flow_histogram_canvas -> SetName( "Coloured_unscaled_elliptic_flow" );
	TCanvas* scaled_elliptic_flow_histogram_canvas   = new TCanvas();
	scaled_elliptic_flow_histogram_canvas   -> SetName( "Coloured_scaled_elliptic_flow" );
	unscaled_elliptic_flow_histogram_canvas -> cd();
	this -> unscaled_elliptic_flow_histogram -> Draw( "COLZ" );
	scaled_elliptic_flow_histogram_canvas   -> cd();
	this -> scaled_elliptic_flow_histogram  -> Draw( "COLZ" );
	this -> save_histogram( this -> unscaled_elliptic_flow_histogram );
	this -> save_histogram( this -> scaled_elliptic_flow_histogram   );
	this -> save_canvas( unscaled_elliptic_flow_histogram_canvas );
	this -> save_canvas( scaled_elliptic_flow_histogram_canvas );
}

#endif