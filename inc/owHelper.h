/*******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2011, 2013 OpenWorm.
 * http://openworm.org
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the MIT License
 * which accompanies this distribution, and is available at
 * http://opensource.org/licenses/MIT
 *
 * Contributors:
 *     	OpenWorm - http://openworm.org/people.html
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *******************************************************************************/

#ifndef OW_HELPER_H
#define OW_HELPER_H

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "owConfigProperty.h"
#include "owOpenCLConstant.h"
#include "owPhysicsConstant.h"
#include "VectorMath.h"
#if defined(_WIN32) || defined (_WIN64)
	#include <windows.h>
#elif defined(__linux__)
	#include <time.h>
#elif defined(__APPLE__)
    #include <stddef.h>
#endif

class owHelper
{
public:
	owHelper(void);
	~owHelper(void);
	static void generateConfiguration( int stage, float *position, float *velocity, float *& elasticConnectionsData_cpp, int *membraneData_cpp, int & numOfLiquidP, int & numOfElasticP, int & numOfBoundaryP, int & numOfElasticConnections, int & numOfMembranes, int * particleMembranesList_cpp, owConfigProrerty * config);
	static void preLoadConfiguration( int & numOfMembranes, owConfigProrerty * config, int & numOfLiquidP, int & numOfElasticP, int & numOfBoundaryP );
	static void loadConfiguration( float *position_cpp, float *velocity_cpp, float *& elasticConnections,int & numOfLiquidP, int & numOfElasticP, int & numOfBoundaryP, int & numOfElasticConnections, int & numOfMembranes,int * membraneData_cpp, int *& particleMembranesList_cpp, owConfigProrerty * config );
	static void loadConfigurationFromOneFile(float * position, float  * velocity, float *& elasticConnectionsData_cpp, int & numOfLiquidP, int & numOfElasticP, int & numOfBoundaryP, int & numOfElasticConnections);
	static void loadConfigurationToFile(float * position, owConfigProrerty * config, std::vector<int> & filter, float * connections=NULL, int * membranes=NULL, bool firstIteration = true, char * position_file = "./buffers/position_buffer.txt", char * connection_file = "./buffers/connection_buffer.txt", char * membrane_file = "./buffers/membranes_buffer.txt");
	static void loadConfigurationFromFile(float *& position, float *& connections, int *& membranes, int iteration = 0);
	static int loadConfigurationFromFile_experemental(float *& position, float *& connections, int *& membranes, owConfigProrerty * config,int iteration = 0);
	void watch_report(const char *str);
	double get_elapsedTime() { return elapsedTime; };
	void refreshTime();
	//For output buffer
	//Create File in which line element_size elements
	//global_size - size of buffer / element_size
	template<typename T> static void log_buffer(const T * buffer, const int element_size, const int global_size, const char * fileName)
	{
		try{
			std::ofstream outFile (fileName);
			for(int i = 0; i < global_size; i++)
			{
				for(int j = 0; j < element_size; j++)
				{
					if(j < element_size - 1 )
						outFile << buffer[ i * element_size + j ] << "\t";
					else
						outFile << buffer[ i * element_size + j ] << "\n";
				}
			}
			outFile.close();
		}catch(std::exception &e){
			std::cout << "ERROR: " << e.what() << std::endl;
			exit( -1 );
		}
	}
	static void log_buffer11(const std::vector< std::vector<Vector3D> > & buffer, const char * fileName)
	{
		try{
			std::ofstream outFile (fileName);
			for(int i = 0; i < buffer.size(); i++)
			{
				std::vector<Vector3D> v = buffer[i];
				for(int j=0;j<v.size();j++)
					outFile << v[j].x << "\t" << v[j].y << "\t" << v[j].z << "\n";
			}
			outFile.close();
		}catch(std::exception &e){
			std::cout << "ERROR: " << e.what() << std::endl;
			exit( -1 );
		}
	}
	static void log_buffer_m(float * buffer, const int element_size, int iteration, const char * fileName)
	{
		try{
			if(iteration == 0){
				std::ofstream outFile (fileName, std::ios::trunc);
				for(int j = 0; j < element_size; j++)
				{
					if(j < element_size - 1 )
						outFile << buffer[ j ] << "\t";
					else
						outFile << buffer[ j ] << "\n";
				}
				outFile.close();
			}else{
				std::ofstream outFile (fileName, std::ios::app);
				for(int j = 0; j < element_size; j++)
				{
					if(j < element_size - 1 )
						outFile << buffer[ j ] << "\t";
					else
						outFile << buffer[ j ] << "\n";
				}
				outFile.close();
			}
		}catch(std::exception &e){
			std::cout << "ERROR: " << e.what() << std::endl;
			exit( -1 );
		}
	}
	static std::string path;
	static std::string suffix;
private:
	double elapsedTime;
#if defined(_WIN32) || defined (_WIN64)
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER t0, t1, t2;				// ticks
	LARGE_INTEGER t3,t4;
#elif defined(__linux__)
	timespec t0, t1, t2;
	timespec t3,t4;
	double us;
#elif defined(__APPLE__)
    unsigned long t0, t1, t2;
#endif
};
#endif // #ifndef OW_HELPER_H
