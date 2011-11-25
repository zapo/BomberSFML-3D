/*
 * VectorUtils.h
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */


#ifndef VECTORUTILS_H_
#define VECTORUTILS_H_


#define _UP_ (sf::Vector3f(0.f, 1.f, 0.f))


namespace Bomber {

namespace utils {

template <class T>
float RadianToDegree(T rad) {
	return rad * 180 / M_PI;
}

template <class T>
sf::Vector3<T> Normalize(const sf::Vector3<T> & source) {

    float length = sqrt((source.x * source.x) + (source.y * source.y) + (source.z * source.z));
    if (length != 0)
        return sf::Vector3<T>(source.x / length, source.y / length, source.z / length);
    else
        return source;
}

template <class T>
sf::Vector2<T> Normalize(const sf::Vector2<T> & source) {

    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return sf::Vector2<T>(source.x / length, source.y / length);
    else
        return source;
}

template <class T>
T DotProduct(const sf::Vector2<T> & v1, const sf::Vector2<T> & v2) {

	return v1.x * v2.x + v1.y * v2.y;
}

template <class T>
T DotProduct(const sf::Vector3<T> & v1, const sf::Vector3<T> & v2) {

	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <class T>
sf::Vector3<T> CrossProduct(const sf::Vector3<T> & v1, const sf::Vector3<T> & v2) {
	sf::Vector3<T> t;
	t.x = v1.y*v2.z - v1.z*v2.y;
	t.y = v1.z*v2.x - v1.x*v2.z;
	t.z = v1.x*v2.y - v1.y*v2.x;

	return t;
}

template <class T>
float VectorLength(const sf::Vector3<T> & v1, const sf::Vector3<T> & v2) {
	return (fabs(v2.x - v1.x) + fabs(v2.y - v1.y) + fabs(v2.z - v1.z));
}

template <class T>
sf::Vector3<T> PointsVector(const sf::Vector3<T> & p1, const sf::Vector3<T> & p2) {
	return sf::Vector3f(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
}

template <class T>
sf::Vector3<T> PointsNormal(const sf::Vector3<T> & p1, const sf::Vector3<T> & p2, const sf::Vector3<T> & p3) {
	sf::Vector3f v1 = PointsVector(p2, p1);
	sf::Vector3f v2 = PointsVector(p3, p2);

	return Normalize(CrossProduct(v2, v1));
}


template <class T>
float VectorsAngle(const sf::Vector2<T> & v1, const sf::Vector2<T> & v2) {

	float angle = RadianToDegree(acos(DotProduct(Normalize(v1), Normalize(v2))));
	return angle;
}


}

template <class T>
std::ostream & operator<<(std::ostream & stream, const sf::Vector3<T> & vec) {

	stream << "x : " << vec.x << " y : " << vec.y << " z : " << vec.z;
	return stream;

}

template <class T>
std::ostream & operator<<(std::ostream & stream, const sf::Vector2<T> & vec) {

	stream << "x : " << vec.x << " y : " << vec.y;
	return stream;

}



}


#endif /* VECTORUTILS_H_ */
