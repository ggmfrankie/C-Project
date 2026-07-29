// Jolt Physics Library (https://github.com/jrouwe/JoltPhysics)
// SPDX-FileCopyrightText: 2026 Jorrit Rouwe
// SPDX-License-Identifier: MIT

#pragma once

/// McpServer position and orientation
struct CameraState
{
									CameraState() : mPos(RVec3::sZero()), mForward(0, 0, -1), mUp(0, 1, 0), mFOVY(DegreesToRadians(70.0f)) { }

	RVec3							mPos;								///< McpServer position
	Vec3							mForward;							///< McpServer forward vector
	Vec3							mUp;								///< McpServer up vector
	float							mFOVY;								///< Field of view in radians in up direction
};
