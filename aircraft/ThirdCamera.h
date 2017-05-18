#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp> 
#include <gtc/constants.hpp>
#include <gtx/transform.hpp>


// Useful constants.
const float kEpsilon = 1.0e-6f;
const float kPi = 3.1415926535897932384626433832795f;
const float kHalfPi = 1.5707963267948966192313216916398f;
const float kTwoPi = 6.283185307179586476925286766559f;
const float k180OverPi = 57.295779513082320876798154814105f;

class ThirdCamera
{
public:

	glm::mat4 Proj;
	glm::mat4 View;

	glm::vec3	mvPosition, mvView, mvUp;
	glm::vec3	mvFacing, mvCross;

	float		mfDistance, mfAzimuth, mfAltitude;

	float		mfTightness;



	ThirdCamera()
	{

		mvPosition = glm::vec3(0,0,0);
		mvView = glm::vec3(0,0,0);

		mvFacing = mvView - mvPosition;

		mvFacing = glm::normalize(mvFacing); 
		mvCross = glm::vec3(mvFacing.z, 0.0f, -mvFacing.x);

		mvUp = glm::cross(mvFacing, mvCross);
		mvUp = glm::normalize(mvUp);

		// Configure the third-person properties of the camera.
		mfDistance = 10.0f;         //10
		mfAzimuth = 180.0f;			// degrees
		mfAltitude = -30.0f;		// -20.0 degrees
		mfTightness = 2.0f;            //2

		// Convert the azimuth/altitude into radians.
		mfAzimuth = glm::radians(mfAzimuth);
		mfAltitude = glm::radians(mfAltitude);


		Proj = glm::perspective(45.0f,4.0f/3.0f,0.1f,1000.0f);

	}
	~ThirdCamera(){}

	float VectorAngle2v( glm::vec3 vVector1, glm::vec3 vVector2 )
	{
		float fCos = vVector1.x * vVector2.x + vVector1.z * vVector2.z / sqrtf(((vVector1.x * vVector1.x) + (vVector1.z * vVector1.z)) * ((vVector2.x * vVector2.x) + (vVector2.z * vVector2.z)));
		if (fCos > 1.0f)
			fCos = 1.0f;
		else if (fCos < -1.0f)
			fCos = -1.0f;

		if (vVector1.x * vVector2.z - vVector1.z * vVector2.x < 0.0f)
			return -acosf(fCos);

		return acosf(fCos);
	}


	void UpdatePosition(glm::vec3 vTargetPosition, glm::vec3 vTargetOrientation)
	{

		float fAngle = VectorAngle2v(vTargetOrientation, glm::vec3(0.0f, 0.0f, 1.0f));


		fAngle -= mfAzimuth;


		glm::vec3 vDesiredPosition;

		vDesiredPosition.x = mfDistance * glm::sin(fAngle) * glm::sin(mfAltitude + glm::half_pi<float>()) + vTargetPosition.x;

		vDesiredPosition.y = mfDistance * cosf(mfAltitude + glm::half_pi<float>()) + vTargetPosition.y;

		vDesiredPosition.z = mfDistance * cosf(fAngle) * sinf(mfAltitude + glm::half_pi<float>()) + vTargetPosition.z;

		glm::vec3 vMovementDirection = vDesiredPosition - mvPosition;
		float fLength = glm::length(vMovementDirection); 

		float fDistanceToMove = mfTightness * (fLength / 2);

		vMovementDirection = glm::normalize(vMovementDirection);

		mvPosition += vMovementDirection * fDistanceToMove;
		mvView = vTargetPosition;	

		mvFacing = mvView - mvPosition;
		mvFacing = glm::normalize(mvFacing);

		mvCross = glm::vec3(mvFacing.z, 0.0f, -mvFacing.x);
		mvUp = glm::cross(mvFacing, mvCross);
		mvUp = glm::normalize(mvUp);
	}

	void Look()
	{
		View = glm::lookAt(glm::vec3(mvPosition.x, mvPosition.y, mvPosition.z),
			glm::vec3(mvView.x, mvView.y + 1, mvView.z),
			glm::vec3(mvUp.x, mvUp.y, mvUp.z));
	}

	void RotateVectorArbitrary(glm::vec3 &Orintation ,  glm::vec3 vCenter, float fAngle, glm::vec3 vAxis )
	{	

		glm::vec3 vNewPosition;
		glm::vec3 vTemp = glm::vec3( Orintation.x,Orintation.y, Orintation.z );

		glm::vec3 vPos = vTemp - vCenter;

		float fSinTheta, fCosTheta;
		fSinTheta = sinf( fAngle );
		fCosTheta = cosf( fAngle );

		float fX = vAxis.x, fY = vAxis.y, fZ = vAxis.z;

		vNewPosition.x  = ( fCosTheta + ( 1 - fCosTheta ) * fX * fX )		* vPos.x;
		vNewPosition.x += ( ( 1 - fCosTheta ) * fX * fY - fZ * fSinTheta )	* vPos.y;
		vNewPosition.x += ( ( 1 - fCosTheta ) * fX * fZ + fY * fSinTheta )	* vPos.z;

		vNewPosition.y  = ( ( 1 - fCosTheta ) * fX * fY + fZ * fSinTheta )	* vPos.x;
		vNewPosition.y += ( fCosTheta + ( 1 - fCosTheta ) * fY * fY )		* vPos.y;
		vNewPosition.y += ( ( 1 - fCosTheta ) * fY * fZ - fX * fSinTheta )	* vPos.z;

		vNewPosition.z  = ( ( 1 - fCosTheta ) * fX * fZ - fY * fSinTheta )	* vPos.x;
		vNewPosition.z += ( ( 1 - fCosTheta ) * fY * fZ + fX * fSinTheta )	* vPos.y;
		vNewPosition.z += ( fCosTheta + ( 1 - fCosTheta ) * fZ * fZ )		* vPos.z;

		Orintation.x = vCenter.x + vNewPosition.x;
		Orintation.y = vCenter.y + vNewPosition.y;
		Orintation.z = vCenter.z + vNewPosition.z;
	}



};