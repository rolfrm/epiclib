/*
 * Texture.cpp
 *
 *  Created on: Jul 28, 2012
 *      Author: sebastian
 */

#include <GL/gl.h>
#include <string>
#include "../Utils/SharedPtr.h"
#include "Texture.hpp"
#include <IL/il.h>

Texture::Texture(){

}

unsigned int interpolationTable [] = {GL_NEAREST, GL_LINEAR};
unsigned int wrapTable [] =  {GL_CLAMP_TO_EDGE, GL_REPEAT};
unsigned int dataTypeTable [] = {GL_UNSIGNED_BYTE};
unsigned int pixelFormatTable[] = {GL_RGBA, GL_RGB, GL_LUMINANCE, 
				   GL_LUMINANCE16, GL_RGBA32F, GL_RGBA16F};
unsigned int ilDataTypeTable [] = {IL_UNSIGNED_BYTE};
unsigned int ilPixelFormatTable[] = {IL_RGBA, IL_RGB, IL_LUMINANCE, 
				     IL_LUMINANCE, IL_RGBA, IL_RGBA};
unsigned int bytesPerPixel [] = {4,3,1,2,4,4};

Texture::Texture(void * data, int width, int height,
		     Interpolation interpolation,
		     TextureWrap wrap, 
		     PixelFormat pixelFormat,  
		     TextureDataType dataType){
  unsigned int glRef;
  this->width = width;
  this->height = height;
  glGenTextures(1,&glRef);
  glBindTexture(GL_TEXTURE_2D, glRef);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		  interpolationTable[(int)interpolation]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		  interpolationTable[(int)interpolation]);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrapTable[(int)wrap]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTable[(int)wrap]);

  glTexImage2D(GL_TEXTURE_2D, 0, bytesPerPixel[(int)pixelFormat], 
	       width, height, 
	       0, pixelFormatTable[(int)pixelFormat],
	       dataTypeTable[(int)dataType],data );

  width = ilGetInteger(IL_IMAGE_WIDTH);
  height= ilGetInteger(IL_IMAGE_HEIGHT);
  
  glBindTexture(GL_TEXTURE_2D, 0);
  
  gl_reference = new unsigned int(glRef);
}

#include <iostream>
Texture::~Texture(){
  if(gl_reference.GetRefCount() == 1){
    std::cout << "Deleting texture.. \n";
    glDeleteTextures(n_tex,(*gl_reference));
  }
}

void Texture::Bind(int channel){
  if(*gl_reference == NULL){
    return;
  }
  glActiveTexture(GL_TEXTURE0+channel);
  glBindTexture(GL_TEXTURE_2D, (*gl_reference)[0]);
}


void Texture::Unbind(int channel){
  glActiveTexture(GL_TEXTURE0+channel);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Write2Texture(int w,int h,int x,int y,
			    PixelFormat format,
			    TextureDataType type,void * data){
  Bind(0);
  glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,pixelFormatTable[(int)format],dataTypeTable[(int)type],data);
}

Texture Texture::FromFile(std::string path,
			  Interpolation interpolation,
			  TextureWrap wrap, 
			  PixelFormat pixelFormat,  
			  TextureDataType dataType){
  
  ILuint texid;
  ilGenImages(1,&texid);
  ilBindImage(texid);

  if(ilLoadImage(path.c_str())){
    ilConvertImage(ilPixelFormatTable[(int)pixelFormat], 
		   ilDataTypeTable[(int)dataType]);
    
  }else{
    throw "Problems converting image";
  }
  void * data = ilGetData();
  int width = ilGetInteger(IL_IMAGE_WIDTH);
  int height = ilGetInteger(IL_IMAGE_HEIGHT);
  Texture tex = Texture(data,width,height, interpolation,wrap,pixelFormat,dataType);
  ilDeleteImage(texid);
  
  return tex;
}
