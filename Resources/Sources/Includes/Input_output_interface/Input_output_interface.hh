#ifndef INPUT_OUTPUT_INTERFACE_H
#define INPUT_OUTPUT_INTERFACE_H

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

class Input_output_interface
{
	protected:
		TFile* input_file;
		TFile* output_file;
		
	public:
		Input_output_interface();
		~Input_output_interface();

		void open_input_file( std :: string filename_p );
		void close_input_file();
		void set_output_file( std :: string filename_p, std :: string options_p = "RECREATE" );
		void close_output_file();
		void save_histogram( TH1* histo_p );
		void save_histogram( TH2* histo_p );
		void save_canvas( TCanvas* canvas_p );
};

Input_output_interface :: Input_output_interface()
{

}

Input_output_interface :: ~Input_output_interface()
{
	this -> close_output_file();
	this -> close_input_file();
	//std :: cout << "Debug: Input_output_interface destructor call." << std :: endl;
}

void Input_output_interface :: open_input_file( std :: string filename_p )
{
	this -> input_file = new TFile( filename_p.c_str(), "READ" );
	if( !((this -> input_file) -> IsOpen()) )
	{
		std :: cerr << "Warning! Error opening the input file: " << filename_p << "." << std :: endl;
	}
}

void Input_output_interface :: set_output_file( std :: string filename_p, std :: string options_p )
{
	this -> output_file = new TFile( filename_p.c_str(), options_p.c_str() );
	if( !((this -> output_file) -> IsOpen()) )
	{
		std :: cerr << "Warning! Error opening the output file: " << filename_p << "." << std :: endl;
	}
}

void Input_output_interface :: save_histogram( TH1* histo_p )
{
	this -> output_file -> cd();
	histo_p -> Write();
}

void Input_output_interface :: save_histogram( TH2* histo_p )
{
	this -> output_file -> cd();
	histo_p -> Write();
}

void Input_output_interface :: save_canvas( TCanvas* canvas_p )
{
	this -> output_file -> cd();
	canvas_p -> Write();
}

void Input_output_interface :: close_input_file()
{
	if( this -> input_file -> IsOpen() )
	{
		this -> input_file -> Close();
	}
	else
	{
		std :: cout << "Warning: The attempt to close the input_file failed (maybe it was already closed?)..." << std :: endl;
	}
}

void Input_output_interface :: close_output_file()
{
	if( this -> output_file -> IsOpen() )
	{
		this -> output_file -> Close();
	}
	else
	{
		std :: cout << "Warning: The attempt to close the output_file failed (maybe it was already closed?)..." << std :: endl;
	}
}

#endif