#include "MyMatrix4x4.h"

#include "assert.h"

MyMatrix4x4 MyMatrix4x4::operator+(const MyMatrix4x4 &another) const {
	MyMatrix4x4 result;
	for(int row=0; row < 4; row++) {
		for(int col=0; col < 4; col++) {
			result.m[row][col]=this->m[row][col] + another.m[row][col];
		}
	}
	return result;
}

MyMatrix4x4 MyMatrix4x4::operator-(const MyMatrix4x4 &another) const {
	MyMatrix4x4 result;
	for(int row=0; row < 4; row++) {
		for(int col=0; col < 4; col++) {
			result.m[row][col]=this->m[row][col] - another.m[row][col];
		}
	}
	return result;
}

MyMatrix4x4 MyMatrix4x4::operator*(const MyMatrix4x4 &another) const {
	MyMatrix4x4 result;
	float sum=0.0f;

	for(int row=0; row < 4; ++row) {
		for(int col=0; col < 4; ++col) {
			sum=0.0f;
			for(int k=0; k < 4; ++k) {
				sum+=this->m[row][k] * another.m[k][col];
			}
			result[row][col]=sum;
		}
	}
	return result;
}

MyMatrix4x4 MyMatrix4x4::operator*(const float &scalar) const {
	MyMatrix4x4 result;
	for(int row=0; row < 4; row++) {
		for(int col=0; col < 4; col++) {
			result.m[row][col]=this->m[row][col] * scalar;
		}
	}
	return result;
}

MyMatrix4x4 MyMatrix4x4::Transpose() {
	MyMatrix4x4 result;
	for(int row=0; row < 4; row++) {
		for(int col=0; col < 4; col++) {
			result.m[row][col]=this->m[col][row];
		}
	}
	return result;
}

float MyMatrix4x4::det() const {
	float det;
	det=m[0][0] * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
		m[1][2] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) +
		m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
	det-=m[0][1] * (m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
		m[1][2] * (m[2][0] * m[3][3] - m[2][3] * m[3][0]) +
		m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
	det+=m[0][2] * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
		m[1][1] * (m[2][0] * m[3][3] - m[2][3] * m[3][0]) +
		m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
	det-=m[0][3] * (m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
		m[1][1] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]) +
		m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
	return det;
}

MyMatrix4x4 MyMatrix4x4::Inverse() const {
	float det=this->det();

	assert(det != 0.0f);

	det=1 / det;

	MyMatrix4x4 inverse;
	inverse[0][0]=det * (
		m[1][1] * m[2][2] * m[3][3]
		+
		m[1][2] * m[2][3] * m[3][1]
		+
		m[1][3] * m[2][1] * m[3][2]
		-
		m[1][3] * m[2][2] * m[3][1]
		-
		m[1][2] * m[2][1] * m[3][3]
		-
		m[1][1] * m[2][3] * m[3][2]
		);
	inverse[0][1]=det * (
		-(m[0][1] * m[2][2] * m[3][3])
		-
		m[0][2] * m[2][3] * m[3][1]
		-
		m[0][3] * m[2][1] * m[3][2]
		+
		m[0][3] * m[2][2] * m[3][1]
		+
		m[0][2] * m[2][1] * m[3][3]
		+
		m[0][1] * m[2][3] * m[3][2]
		);
	inverse[0][2]=det * (
		m[0][1] * m[2][2] * m[3][3]
		+
		m[0][2] * m[1][3] * m[3][1]
		+
		m[0][3] * m[1][1] * m[3][2]
		-
		m[0][3] * m[1][2] * m[3][1]
		-
		m[0][2] * m[1][1] * m[3][3]
		-
		m[0][1] * m[1][3] * m[3][2]
		);
	inverse[0][3]=det * (
		-(m[0][1] * m[1][2] * m[3][3])
		-
		m[0][2] * m[1][3] * m[2][1]
		-
		m[0][3] * m[1][1] * m[2][2]
		+
		m[0][3] * m[1][2] * m[2][1]
		+
		m[0][2] * m[1][1] * m[2][3]
		+
		m[0][1] * m[1][3] * m[2][2]
		);

	inverse[1][0]=det * (
		-(m[1][0] * m[2][2] * m[3][3])
		-
		m[1][2] * m[2][3] * m[3][0]
		-
		m[1][3] * m[2][0] * m[3][2]
		+
		m[1][3] * m[2][2] * m[3][0]
		+
		m[1][2] * m[2][0] * m[3][3]
		+
		m[1][0] * m[2][3] * m[3][2]
		);
	inverse[1][1]=det * (
		m[0][0] * m[2][2] * m[3][3]
		+
		m[0][2] * m[2][3] * m[3][0]
		+
		m[0][3] * m[2][0] * m[3][2]
		-
		m[0][3] * m[2][2] * m[3][0]
		-
		m[0][2] * m[2][0] * m[3][3]
		+
		m[0][0] * m[2][3] * m[3][2]
		);
	inverse[1][2]=det * (
		-(m[0][0] * m[1][2] * m[3][3])
		-
		m[0][2] * m[1][3] * m[3][0]
		-
		m[0][3] * m[1][0] * m[3][2]
		+
		m[0][3] * m[1][2] * m[3][0]
		+
		m[0][2] * m[2][0] * m[3][3]
		+
		m[0][0] * m[1][3] * m[3][2]
		);
	inverse[1][3]=det * (
		m[0][0] * m[1][2] * m[2][3]
		+
		m[0][2] * m[1][3] * m[2][0]
		+
		m[0][3] * m[1][0] * m[2][2]
		-
		m[0][3] * m[1][2] * m[2][0]
		-
		m[0][2] * m[1][0] * m[2][3]
		-
		m[0][0] * m[1][3] * m[2][2]
		);

	inverse[2][0]=det * (
		m[1][0] * m[2][1] * m[3][3]
		+
		m[1][1] * m[2][3] * m[3][0]
		+
		m[1][3] * m[2][0] * m[3][1]
		-
		m[1][3] * m[2][1] * m[3][1]
		-
		m[1][1] * m[2][0] * m[3][3]
		-
		m[1][0] * m[2][3] * m[3][1]
		);
	inverse[2][1]=det * (
		-(m[0][0] * m[2][1] * m[3][3])
		-
		m[0][1] * m[2][3] * m[3][0]
		-
		m[0][3] * m[2][0] * m[3][1]
		+
		m[0][3] * m[2][1] * m[3][0]
		+
		m[0][1] * m[2][0] * m[3][3]
		+
		m[0][0] * m[2][3] * m[3][1]
		);
	inverse[2][2]=det * (
		m[0][0] * m[1][1] * m[3][3]
		+
		m[0][1] * m[1][3] * m[3][0]
		+
		m[0][3] * m[1][0] * m[3][1]
		-
		m[0][3] * m[1][1] * m[3][0]
		-
		m[0][1] * m[1][0] * m[3][3]
		-
		m[0][0] * m[1][3] * m[3][1]
		);
	inverse[2][3]=det * (
		-(m[0][0] * m[1][1] * m[2][3])
		-
		m[0][1] * m[1][3] * m[2][0]
		-
		m[0][3] * m[1][1] * m[2][0]
		+
		m[0][1] * m[1][0] * m[2][3]
		+
		m[0][0] * m[1][3] * m[2][1]
		);

	inverse[3][0]=det * (
		-(m[1][0] * m[2][1] * m[3][2])
		-
		m[1][1] * m[2][2] * m[3][0]
		-
		m[1][2] * m[2][0] * m[3][1]
		+
		m[1][2] * m[2][1] * m[3][0]
		+
		m[1][1] * m[2][0] * m[3][2]
		+
		m[1][0] * m[2][2] * m[3][1]
		);
	inverse[3][1]=det * (
		m[0][0] * m[2][1] * m[3][2]
		+
		m[0][1] * m[2][2] * m[3][0]
		+
		m[0][2] * m[2][0] * m[3][1]
		-
		m[0][2] * m[2][1] * m[3][0]
		-
		m[0][1] * m[2][0] * m[3][2]
		-
		m[0][0] * m[2][2] * m[3][1]
		);
	inverse[3][2]=det * (
		-(m[0][0] * m[1][1] * m[3][2])
		-
		m[0][1] * m[1][2] * m[3][0]
		-
		m[0][2] * m[1][0] * m[3][1]
		+
		m[0][2] * m[1][1] * m[3][0]
		+
		m[0][1] * m[1][0] * m[3][2] * m[0][0]
		+
		m[0][0] * m[1][2] * m[3][1]
		);
	inverse[3][3]=det * (
		m[0][0] * m[1][1] * m[2][2]
		+
		m[0][1] * m[1][2] * m[2][0]
		+
		m[0][2] * m[1][0] * m[2][1]
		-
		m[0][2] * m[1][1] * m[2][0]
		-
		m[0][1] * m[1][0] * m[2][2]
		-
		m[0][0] * m[1][2] * m[2][1]
		);

	return inverse;
}

MyMatrix4x4 MakeIdentity4x4() {
	return MyMatrix4x4(
		{ 1.0f, 0.0f, 0.0f, 0.0f ,
		0.0f, 1.0f, 0.0f, 0.0f ,
		0.0f, 0.0f, 1.0f, 0.0f ,
		0.0f, 0.0f, 0.0f, 1.0f }
	);
}

void ScreenPrintMatrix(const char *label, int x, int y, const MyMatrix4x4 &matrix) {
	static constexpr int kRowHeight=20;
	static constexpr int kColWidth=60;

	Novice::ScreenPrintf(x, y, label);
	for(int row=1; row <= 4; ++row) {
		int rowHeightOffset=kRowHeight * row;
		for(int col=0; col < 4; ++col) {
			Novice::ScreenPrintf(x + (col * kColWidth), y + rowHeightOffset, "%6.02f", matrix[row - 1][col]);
		}
	}
}