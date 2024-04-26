#pragma once

#include <MyMatrix4x4.h>
#include <Transform.h>

/// 
/// いったん中身だけ
/// 
struct Camera {
	Transform transform_;
	MyMatrix4x4 vpMa_;
};

