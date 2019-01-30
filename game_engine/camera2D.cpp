#include "camera2D.h"



namespace game_engine {


	camera2D::camera2D() :
		_screenWidth(500),
		_screenHeight(500),
		_needsMatrixUpdate(true),
		_scale(1.0f),
		_position(0.0f, 0.0f),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f)
	{
	}


	camera2D::~camera2D()
	{
	}


	void camera2D::init(int screenWidth, int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}


	void camera2D::update()
	{
		if (_needsMatrixUpdate)
		{
			//Camera Translation
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			//Camera Scale
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;
			_needsMatrixUpdate = false;
		}

	}

	glm::vec2 camera2D::convertScreenToWorld(glm::vec2 screenCoords)
	{
		//Invert Y direction
		screenCoords.y = _screenHeight - screenCoords.y;
		//make 0 the center of screen
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		//scale the coordinates
		screenCoords /= _scale;
		//translate with the camera position
		screenCoords += _position;

		return screenCoords;
	}


	//simple AABB test to see if a box(such as an agent) is in the camera view
	bool camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions)
	{

		glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / _scale;


		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		//center position of the parameters
		glm::vec2 centerPos = position + dimensions / 2.0f;
		//center position of the camera
		glm::vec2 centerCameraPos = _position;
		//vector from the input to the camera
		glm::vec2 distVec = centerPos - centerCameraPos;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);



		//if this is true, we are colliding
		if (xDepth > 0 && yDepth > 0)
		{
			//there was a collision
			return true;
		}
		return false;
	
	}



}



