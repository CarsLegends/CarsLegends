#ifndef COMPONENTS_SPRITE_HPP
#define COMPONENTS_SPRITE_HPP

struct Sprite
{
	unsigned int mTextureID;
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mChannels;
	unsigned int mTextureUnit;
	bool mIsTransparent;
	bool mIsVisible;

	Sprite();
	Sprite(unsigned int textureID, unsigned int width, unsigned int height, unsigned int channels, unsigned int textureUnit, bool isTransparent, bool isVisible);
};

inline Sprite::Sprite()
{
	this->mTextureID = 0;
	this->mWidth = 0;
	this->mHeight = 0;
	this->mChannels = 0;
	this->mTextureUnit = 0;
	this->mIsTransparent = false;
	this->mIsVisible = true;
}

inline Sprite::Sprite(unsigned int textureID, unsigned int width, unsigned int height, unsigned int channels, unsigned int textureUnit, bool isTransparent, bool isVisible)
{
	this->mTextureID = textureID;
	this->mWidth = width;
	this->mHeight = height;
	this->mChannels = channels;
	this->mTextureUnit = textureUnit;
	this->mIsTransparent = isTransparent;
	this->mIsVisible = isVisible;
}

#endif // !COMPONENTS_TRANSFORM_HPP