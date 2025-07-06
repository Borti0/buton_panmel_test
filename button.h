#pragma once
#include <stdint.h>
#include <iostream>
#include <thread>

#include "raylib.h"
#include "raymath.h"

class Button4StatesTiles
{
public:
	Button4StatesTiles(
		uint32_t OriginX,
		uint32_t OriginY,
		char * TextureFilePath,
		char * SoundFilePath1,
		char * SoundFilePath2,
		uint8_t InitialState
	);
	~Button4StatesTiles();

	//TODO:
	/*void AddButtonFunction(void* FuncPoit, void* Args);

	void SetButtonState(uint8_t State);

	void CheckButtonState(Vector2 MousePosition);
	void ExecuteButtonFuntion(void);
	void UpdateButtonTecture(void);
	*/

private:
	// origin possition in window
	uint32_t OriginX = 0u;
	uint32_t OriginY = 0u;

	// button objects for assets
	Texture2D ButtonTexture = {};
	Sound ButtonSoundON2OFF = {};
	Sound ButtonSoundOFF2ON = {};

	
	// buton size
	uint32_t ButtonHeight = 0u;
	uint32_t ButtonTotalWidth = 0u;
	uint32_t ButtonWidth = 0u;
	
	// button texture and screen bounds rectangles
	Rectangle ButtonTextureRectangle = { 0u, 0u, 0u, 0u }; // this one affect the texture position
	Rectangle ButtonBoundRectangle = { 0u, 0u, 0u, 0u }; //this one affects the buton position

	// button max states and current state
	uint8_t ButtonMaxStates = 4u;
	uint8_t ButtonCurrentState = 0u;
	
	// button binary state
	bool ButtonUsed = false;
	bool ButonExecute = false; // true when the button is presed, toggle to false in executeion update

	void* ButtonFunction = nullptr;
	void* ButtonFunctionArgument = nullptr;

	// button on2off function
	std::thread on2off;

	// button off2on function
	std::thread off2on;
};

#ifdef __BUTTON_IMPL__
Button4StatesTiles::Button4StatesTiles(
	uint32_t OriginX,
	uint32_t OriginY,
	char* TextureFilePath,
	char* SoundFilePath1,
	char* SoundFilePath2,
	uint8_t InitialState )
{
	// Load the origin point of the button
	this->OriginX = OriginX;
	this->OriginY = OriginY;

	// Load assets to the objects
	this->ButtonTexture = LoadTexture((const char*)TextureFilePath);
	this->ButtonSoundON2OFF = LoadSound((const char*)SoundFilePath1);
	this->ButtonSoundOFF2ON = LoadSound((const char*)SoundFilePath2);

	//Load initial state of the button
	this->ButtonCurrentState = InitialState;

	// Calculate:
	// Button used state
	if (this->ButtonCurrentState != 0) this->ButtonUsed = true;
	
	// Button texture sizes 
	this->ButtonHeight = this->ButtonTexture.height;
	this->ButtonTotalWidth = this->ButtonTexture.width;
	this->ButtonWidth = (uint32_t)(this->ButtonTotalWidth / this->ButtonMaxStates);
	
	// Button rectangel bounds
	this->ButtonTextureRectangle = {
		0u, 0u,
		(float)this->ButtonWidth,
		(float)this->ButtonHeight
	};
	this->ButtonBoundRectangle = {
		(float)this->OriginX,
		(float)this->OriginY,
		(float)this->ButtonWidth,
		(float)this->ButtonHeight
	};

	return;
}

Button4StatesTiles::~Button4StatesTiles()
{
	UnloadTexture(this->ButtonTexture);
	UnloadSound(this->ButtonSoundON2OFF);
	UnloadSound(this->ButtonSoundOFF2ON);
	return;
}
#endif