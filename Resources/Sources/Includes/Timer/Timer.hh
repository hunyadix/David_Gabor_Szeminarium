#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <iostream>

class Timer
{
	private:
		time_t start_t;
		time_t current_t;
		double seconds_elapsed;	
	public:
		Timer();
		~Timer();
		void restart( std :: string  text_p );
		void print_seconds( std :: string pretext_p, std :: string post_text_p );
};

Timer :: Timer()
{

}

Timer :: ~Timer()
{

}

void Timer :: restart( std :: string  text_p )
{
	std :: cout << text_p << std :: endl;
	time( &(this -> start_t) );
}

void Timer :: print_seconds( std :: string pretext_p, std :: string post_text_p )
{
	time( &current_t );
	seconds_elapsed = difftime( current_t, start_t );
	std :: cout << pretext_p << seconds_elapsed << post_text_p << std :: endl;
}

#endif
