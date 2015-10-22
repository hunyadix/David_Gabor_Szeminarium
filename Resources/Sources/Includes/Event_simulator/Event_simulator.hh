#ifndef EVENT_SIMULATOR_H
#define EVENT_SIMULATOR_H

#include "TF1.h"
#include "TH1.h"

class Event_simulator
{
	private:
		TF1* distribution_used = nullptr;
		TF1* distribution_estimation = nullptr;
		TH1F* event_histogram  = nullptr;
	public:
		 Event_simulator();
		~Event_simulator();
		void set_distribution( TF1* distribution_p );
		TF1* get_distribution();
		void set_estimating_function( TF1* distribution_p );
		TF1* get_estimating_function();
		void set_bin_settings( int numbins_p, float minbin_p, float maxbix_p );
		void reset_event_histogram();
		float simulate_event( int n_particles_p );
		float simulate_n_events( int n_p, int n_particles_p );
		float simulate_event_fit_method( int n_particles_p );
		float simulate_n_events_fit_method( int n_p, int n_particles_p );
		TH1F* get_distribution_sample( int n_particles_p );
};

Event_simulator :: Event_simulator()
{

}

Event_simulator ::~Event_simulator()
{
	if( this -> event_histogram ) {	delete this -> event_histogram; }
}

void Event_simulator :: set_distribution( TF1* distribution_p )
{
	this -> distribution_used = distribution_p;
}

TF1* Event_simulator :: get_distribution()
{
	return this -> distribution_used;
}

void Event_simulator :: set_estimating_function( TF1* function_p )
{
	this -> distribution_estimation = function_p;
}

TF1* Event_simulator :: get_estimating_function()
{
	return this -> distribution_estimation;
}


void Event_simulator :: set_bin_settings( int numbins_p, float minbin_p, float maxbix_p )
{
	this -> event_histogram = new TH1F( "event_simulator_dummy_histo", "event_simulator_dummy_histo", numbins_p, minbin_p, maxbix_p );
}

void Event_simulator :: reset_event_histogram()
{
	this -> event_histogram -> Reset();
}

float Event_simulator :: simulate_event( int n_particles_p )
{
	this -> reset_event_histogram();
	float cosine_zero_mean_difference = (M_PI / 2);
	for( int current_particle_index = 0; current_particle_index < n_particles_p; ++current_particle_index )
	{
		float position = this -> distribution_used -> GetRandom();
		this -> event_histogram -> Fill( position );
	}
	float result = fabs( this -> event_histogram -> GetMean() - cosine_zero_mean_difference );
	return result;
}

float Event_simulator :: simulate_n_events( int n_p, int n_particles_p )
{
	float result = 0;
	for( int i = 0; i < n_p; ++i )
	{
		result = result + this -> simulate_event( n_particles_p );
	}
	result = result / n_p;
	return result;
}

float Event_simulator :: simulate_event_fit_method( int n_particles_p )
{
	this -> get_estimating_function() -> SetParameter( 1, -0.2 * M_PI );
	this -> reset_event_histogram();
	for( int current_particle_index = 0; current_particle_index < n_particles_p; ++current_particle_index )
	{
		float position = this -> distribution_used -> GetRandom();
		this -> event_histogram -> Fill( position );
	}
	this -> event_histogram -> Fit( this -> distribution_estimation, "WWQN0C" );
	float result = fabs( this -> distribution_estimation -> GetParameter( 1 ) - this -> distribution_used -> GetParameter( 1 ) );
	int num_periods = static_cast<int>(result / (M_PI * 2));
	result = result - num_periods * 2 * M_PI;
	return result;
}

float Event_simulator :: simulate_n_events_fit_method( int n_p, int n_particles_p )
{
	float result = 0;
	for( int i = 0; i < n_p; ++i )
	{
		result = result + this -> simulate_event_fit_method( n_particles_p );
	}
	result = result / n_p;
	return result;
}

TH1F* Event_simulator :: get_distribution_sample( int n_particles_p )
{
	this -> reset_event_histogram();
	for( int current_particle_index = 0; current_particle_index < n_particles_p; ++current_particle_index )
	{
		float position = this -> distribution_used -> GetRandom();
		this -> event_histogram -> Fill( position );
	}
	return (TH1F*)(this -> event_histogram -> Clone( "event_simulator_distribution_sample" ));
}

#endif