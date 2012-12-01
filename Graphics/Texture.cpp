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
	n_tex=0;
	width=0;
	height=0;
}

unsigned int interpolationTable [] = {GL_NEAREST, GL_LINEAR};
unsigned int wrapTable [] =  {GL_CLAMP_TO_EDGE, GL_REPEAT};
unsigned int dataTypeTable [] = {GL_UNSIGNED_BYTE};
unsigned int pixelFormatTable[] = {GL_RGBA, GL_RGB, GL_LUMINANCE, 
				   GL_LUMINANCE16, GL_RGBA32F, 
				   GL_RGBA16F,GL_DEPTH_COMPONENT, 
				   GL_DEPTH_COMPONENT16,GL_DEPTH_COMPONENT24,
				   GL_DEPTH_COMPONENT32,GL_DEPTH24_STENCIL8};
unsigned int ilDataTypeTable [] = {IL_UNSIGNED_BYTE};
unsigned int ilPixelFormatTable[] = {IL_RGBA, IL_RGB, IL_LUMINANCE, 
				     IL_LUMINANCE, IL_RGBA, IL_RGBA};
unsigned int bytesPerPixel [] = {4,3,1,2,4,4};
unsigned int dataFormatTable[] = {GL_RGBA,GL_DEPTH_COMPONENT,GL_DEPTH_STENCIL};

bool Texture::HasData(){
  return (*textureObject).glReference != -1;
}
void Texture::Release(){
  textureObject = TextureObject();
}

Texture::Texture(int width, int height,void * data,
		     Interpolation interpolation,
		     TextureWrap wrap, 
		     PixelFormat pixelFormat,  
		     DataFormat dataFormat,
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

  glTexImage2D(GL_TEXTURE_2D, 0, pixelFormatTable[(int)pixelFormat], 
	       width, height, 
	       0, dataFormatTable[(int)dataFormat],
	       dataTypeTable[(int)dataType],data );

  this->width = width;
  this->height = height;
  glBindTexture(GL_TEXTURE_2D, 0);
  
  textureObject = TextureObject(glRef);
}

Texture::Texture(int width,int height,
		     Interpolation interpolation,
		     TextureWrap wrap, 
		     PixelFormat pixelFormat,  
		 DataFormat dataFormat,
		 TextureDataType dataType):Texture(width,height,NULL,interpolation,wrap,pixelFormat,dataFormat,dataType){

}

void Texture::Bind(int channel){
  if((*textureObject).glReference == -1){
    return;
  }
  glActiveTexture(GL_TEXTURE0+channel);
  glBindTexture(GL_TEXTURE_2D, (unsigned int) (*textureObject).glReference);
}


void Texture::Unbind(int channel){
  glActiveTexture(GL_TEXTURE0+channel);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Write2Texture(int w,int h,int x,int y,
			    PixelFormat format,
			    TextureDataType type,void * data){
  Bind(0);
  glTexSubImage2D(GL_TEXTURE_2D,0,
		  x,y,w,h,pixelFormatTable[(int)format],
		  dataTypeTable[(int)type],data);
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
  Texture tex = Texture(width,height,data,
			interpolation,wrap,
			pixelFormat);
  ilDeleteImage(texid);
  
  return tex;
}

Texture::TextureObject::TextureObject(int glRef){
  glReference = glRef;
}

void Texture::TextureObject::Dispose(){
  if(glReference != -1){
    std::cout << "Deleting texture.. \n";
    unsigned int ref = glReference;
    glDeleteTextures(1,&ref);
  }
}
