#pragma once

#ifndef IMAGIO_POINT_CLOUD_CAMERA_MODEL
#define IMAGIO_POINT_CLOUD_CAMERA_MODEL

#include "cmath"
#define GLM_FORCE_RADIANS
#pragma warning(push, 0)       
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

#pragma warning(push, 0)       
#include "softkinetic/skv.h"
#pragma warning(pop)	


namespace imagio
{

struct camera_intrinsic_parameters
{
	/// the width of the map when the frame was captured
	int width;
	/// the height of the map when the frame was captured
	int height;
	/// the focal length along the x axis, expressed in pixel units
	float fx;
	/// the focal length along the y axis, expressed in pixel units
	float fy;
	/// the central point along the x axis, expressed in pixel units
	float cx;
	/// the central point along the y axis, expressed in pixel units
	float cy;
	/// the first radial distortion coefficient
	float k1;
	/// the second radial distortion coefficient
	float k2;
	/// the third radial distortion coefficient
	float k3;
	/// the fourth radial distortion coefficient
	float k4;
	/// the first tangential distortion coefficient
	float p1;
	/// the second tangential distortion coefficient
	float p2;
	camera_intrinsic_parameters(skv_pinhole_model& pinhole_model, skv_distortion_model& distortion_model,
								int image_width, int image_height)
	{
		fx = distortion_model.fx;
		fy = distortion_model.fy;
		k1 = distortion_model.k1;
		k2 = distortion_model.k2;
		k3 = distortion_model.k3;
		k4 = 0.0f;
		p1 = distortion_model.p1;
		p2 = distortion_model.p2;
		cx = pinhole_model.cx;
		cy = pinhole_model.cy;
		width = image_width;
		height = image_height;
	}
};

struct camera_extrinsic_parameters
{
	/// the r11 parameter
	float r11;
	/// the r12 parameter
	float r12;
	/// the r13 parameter
	float r13;
	/// the r21 parameter
	float r21;
	/// the r22 parameter
	float r22;
	/// the r32 parameter
	float r23;
	/// the r31 parameter
	float r31;
	/// the r32 parameter
	float r32;
	/// the r33 parameter
	float r33;
	/// the t1 parameter
	float t1;
	/// the t2 parameter
	float t2;
	/// the t3 parameter
	float t3;
};


class fisheye_camera
{
private:
	int		LUT_SIZE = 8600;
	float	MAX_RAD2_DEPTH = 13.9282f;
	float	RAD_STEP2_DEPTH = MAX_RAD2_DEPTH / LUT_SIZE;
	float	MAX_RAD2_DEPTH_COLOR = 13.9282f;
	float	RAD_STEP2_DEPTH_COLOR = MAX_RAD2_DEPTH / LUT_SIZE;

	struct lookup_table_element
	{
		float k;
		// This is the radius in the input
		float r2;
	};

	lookup_table_element* lut;
	const camera_intrinsic_parameters* intrinsics;

public:
	fisheye_camera(skv_pinhole_model& pinhole_model, skv_distortion_model& distortion_model, int image_width, int image_height)
	{
		intrinsics = new camera_intrinsic_parameters(pinhole_model, distortion_model, image_width, image_height);
		lut = new lookup_table_element[LUT_SIZE];
	}

	~fisheye_camera()
	{
		delete lut;
	}

	int findindex(const float r2)
	{
		if(r2 < 0.0f)
		{
			assert(false);
			return 0;
		}

		for(int i = 0; i < LUT_SIZE - 1; i++)
		{
			if(r2 >= lut[i].r2 && r2 < lut[i + 1].r2) return i;
		}
		return LUT_SIZE - 1;
	}


	void build_lookup_table()
	{
		for(int i = 0; i < LUT_SIZE; ++i)
		{
			float r2 = float(i) * RAD_STEP2_DEPTH;
			float r = std::sqrt(r2);
			float theta = std::atan(r);
			float theta2 = theta * theta;
			float theta3 = theta2 * theta;
			float theta4 = theta3 * theta;
			float theta5 = theta4 * theta;
			float theta6 = theta5 * theta;
			float theta7 = theta6 * theta;
			float theta8 = theta7 * theta;
			float theta9 = theta8 * theta;
			float theta_d = theta + intrinsics->k1 * theta3 + intrinsics->k2 * theta5 +
				intrinsics->k3 * theta7 + intrinsics->k4 * theta9;

			float cdist = r > 1e-8 ? theta_d / r : 1;

			auto& el = lut[i];
			el.k = 1 / cdist;
			el.r2 = r2 * cdist * cdist;
		}
	}

	void transform2dto3d(float x, float y, float depth, glm::vec3* point3d_out)
	{
		float x1 = (x - intrinsics->cx) / (intrinsics->fx);
		float y1 = (intrinsics->cy - y) / (intrinsics->fy);

		float r2 = x1 * x1 + y1 * y1;

		float k = lut[findindex(r2)].k;

		point3d_out->x = x1 * k * depth;
		point3d_out->y = y1 * k * depth;
		point3d_out->z = depth;
	}


};

}

#endif