/*
 * Texture.hpp
 *
 *  Created on: Jul 28, 2012
 *      Author: sebastian
 */

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <string>
#include "../Utils/SharedPtr.h"

enum class Interpolation{
  Nearest=0,Linear=1
};

enum class TextureWrap{
  ClampToEdge = 0,
    Repeat = 1,
    Mirror = 2
};

enum class TextureDataType{
  UnsignedByte = 0
};

enum class PixelFormat{
  RGBA = 0,
    RGB = 1,
    Grey = 2,
    Grey16 = 3,
    RGBA32F = 4,
    RGBA16F = 5,
    Depth = 6,
    Depth16 = 7,
    Depth24 = 8,
    Depth32 = 9
};

enum class DataFormat{
	RGBA=0,
	Depth=1,
};


class Texture{
  class TextureObject{
    
  public:
    int glReference;
    TextureObject(int glRef = -1);
    void Dispose();
    
  };
  SharedPtr<TextureObject> textureObject;
public:
  int width, height, n_tex; 
  
  
  Texture(int width, int height,void * data=NULL,
	    Interpolation interpolation = Interpolation::Linear,
	    TextureWrap wrap = TextureWrap::Repeat, 
	    PixelFormat pixelFormat = PixelFormat::RGBA,  
	    DataFormat dataFormat=DataFormat::RGBA,
	    TextureDataType dataType = TextureDataType::UnsignedByte);

  static Texture FromFile(std::string path,
			  Interpolation interpolation = Interpolation::Linear,
			  TextureWrap wrap = TextureWrap::Repeat, 
			  PixelFormat pixelFormat = PixelFormat::RGBA,  
			  TextureDataType dataType = TextureDataType::UnsignedByte);

  Texture();
  
  void Bind(int channel);
  void Unbind(int channel);
  void Write2Texture(int width,int height,int x,int y,PixelFormat pixelFormat, TextureDataType type,void * data);  
  int unsafeOpenGLTextureRef(){return textureObject.Get().glReference;} //Be cautious when using this function, the reference might get deleted if all instances of the Texture class are deleted
};


#endif /* TEXTURE_HPP_ */
