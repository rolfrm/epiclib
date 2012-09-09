/*
 * Texture.cpp
 *
 *  Created on: Jul 28, 2012
 *      Author: sebastian
 */

#include "Texture.hpp"
#include <IL/il.h>
#include <iostream>


Texture2D::Texture2D(std::string path,GLint interpolation,GLint wrap){
	reference=new GLuint;
	ILuint texid;
	ilGenImages(1,reference);
	ilBindImage(*reference);

	if(ilLoadImage(path.c_str())){
			std::cout<<"texture "<<path<<" loaded\n";
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	}else{
		std::cout << "Problems converting image" <<path<<"\n";
	}

	glGenTextures(1,reference);
	glBindTexture(GL_TEXTURE_2D, *reference);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,interpolation);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,interpolation);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, IL_RGBA,GL_UNSIGNED_BYTE, ilGetData());

	width=ilGetInteger(IL_IMAGE_WIDTH);
	height=ilGetInteger(IL_IMAGE_HEIGHT);
	ilDeleteImage(texid);
	glBindTexture(GL_TEXTURE_2D, 0);

	count=new GLuint;
	*count=1;
}

Texture2D::Texture2D(GLuint width,GLuint height,GLuint texture_format,GLuint copies,GLuint interpolation,GLuint wrap,GLuint data_type,void * data,GLuint data_format){

	reference=new GLuint[copies];

	glGenTextures(copies,reference);
	for(int i=0;i<copies;i++){
		glBindTexture(GL_TEXTURE_2D, reference[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,interpolation);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,interpolation);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

		glTexImage2D(GL_TEXTURE_2D, 0, texture_format, width, height, 0, data_format,data_type, data);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	count=new GLuint;
	*count=1;
	this->width=width;
	this->height=height;
	this->n_tex=copies;
}

Texture2D::Texture2D(const Texture2D & original){
	reference=original.reference;
	width=original.width;
	height=original.height;
	count=original.count;
	*count+=1;
	n_tex=original.n_tex;
}

Texture2D::~Texture2D(){
	*count-=1;
	if(*count==0){
		delete count;
		glDeleteTextures(n_tex,reference);
		delete [] reference;
	}
}

void Texture2D::BindTexture(GLuint channel,GLuint texture){
	glActiveTexture(GL_TEXTURE0+channel);
	glBindTexture(GL_TEXTURE_2D, reference[texture]);
}

void Texture2D::UnbindTexture(GLuint channel){
	glActiveTexture(GL_TEXTURE0+channel);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Write2Texture(GLuint w,GLuint h,GLuint x,GLuint y,GLuint format,GLuint type,void * data){
	//BindTexture(3);
	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,format,type,data);
}

