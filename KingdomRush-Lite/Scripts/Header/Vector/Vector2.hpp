#ifndef _VECTOR_2_HPP_
#define _VECTOR_2_HPP_

#include <cmath>

class Vector2
{
public:
	double x = 0;
	double y = 0;

public:
	Vector2(double, double);
	Vector2() = default;
	~Vector2() = default;

	double Length() const;                     //获取向量模长
	Vector2 Normalized() const;                //返回标准化的单位向量
	bool ApproxZero() const;                   //向量接近为零的判断

	bool operator==(const Vector2&) const;     //向量的判等
	bool operator!=(const Vector2&) const;     //向量的判不等
	bool operator>(const Vector2&) const;      //左侧向量模长大于右侧
	bool operator>=(const Vector2&) const;     //左侧向量模长大于等于右侧
	bool operator<(const Vector2&) const;      //左侧向量模长小于右侧
	bool operator<=(const Vector2&) const;     //左侧向量模长小于等于右侧

	Vector2 operator+(const Vector2&) const;   //加法
	Vector2 operator-(const Vector2&) const;   //减法
	Vector2 operator*(const double&) const;    //常数乘法
	double operator*(const Vector2&) const;    //向量乘法

	void operator+=(const Vector2&);           //加等于
	void operator-=(const Vector2&);           //减等于
	void operator*=(const double&);            //乘等于
};

Vector2::Vector2(double _x, double _y) :x(_x), y(_y) {}

double Vector2::Length() const
{
	return sqrt(x * x + y * y);
}

Vector2 Vector2::Normalized() const
{
	//若向量为零向量，则返回零向量即可
	if (Length() == 0)
		return Vector2(0, 0);
	//反之则返回该向量方向上的单位向量（模长为1的向量）
	return Vector2(x / Length(), y / Length());
}

bool Vector2::ApproxZero() const
{
	//判断两个物体之间是否重合时（比如某怪物是否到达目标地点），直接将向量的浮点数元素x、y和0作比较是不精确的，会出现二者永远无法被判定为重合的情况
	return Length() < 0.000001;
}

bool Vector2::operator==(const Vector2& _compare) const
{
	return (x == _compare.x) && (y == _compare.y);
}

bool Vector2::operator!=(const Vector2& _compare) const
{
	return !(*this == _compare);
}

bool Vector2::operator>(const Vector2& _compare) const
{
	return Length() > _compare.Length();
}

bool Vector2::operator>=(const Vector2& _compare) const
{
	return Length() >= _compare.Length();
}

bool Vector2::operator<(const Vector2& _compare) const
{
	return Length() < _compare.Length();
}

bool Vector2::operator<=(const Vector2& _compare) const
{
	return Length() <= _compare.Length();
}

Vector2 Vector2::operator+(const Vector2& _plus) const
{
	return Vector2(x + _plus.x, y + _plus.y);
}

void Vector2::operator+=(const Vector2& _plus)
{
	x += _plus.x;
	y += _plus.y;
}

Vector2 Vector2::operator-(const Vector2& _minus) const
{
	return Vector2(x - _minus.x, y - _minus.y);
}

void Vector2::operator-=(const Vector2& _minus)
{
	x -= _minus.x;
	y -= _minus.y;
}

Vector2 Vector2::operator*(const double& _mtp) const
{
	return Vector2(x * _mtp, y * _mtp);
}

void Vector2::operator*=(const double& _mtp)
{
	x *= _mtp;
	y *= _mtp;
}

double Vector2::operator*(const Vector2& _mtp) const
{
	return (x * _mtp.x) + (y * _mtp.y);
}

#endif
