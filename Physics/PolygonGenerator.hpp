/*
 * PolygonGenerator.hpp
 *
 *  Created on: Aug 17, 2011
 *      Author: sebastian
 */

#ifndef POLYGONGENERATOR_HPP_
#define POLYGONGENERATOR_HPP_

#include "Polygon.h"
#include <vector>
#include <list>


	Polygon GenerateBox(double x,double y,double width,double height);

	Polygon GenerateGeneralPolygon(double x,double y,double Vertex[],unsigned int vertices);

	std::list<Polygon> GenerateLand(char * hlist,unsigned int size,double Length,double Height,double Depth);
	void split(std::vector<float> Input,std::vector<std::vector<float> > * Output);


#endif /* POLYGONGENERATOR_HPP_ */
