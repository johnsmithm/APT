#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <memory>
#include <string>
#include <functional>

class Shape {
public:
	Shape(const std::string && myName)
		: myName(myName) {
		std::cout << "\tc'tor Shape\n";
	}
	~Shape() { std::cout << "\td'tor Shape\n"; }

	virtual double getArea() = 0;
	virtual void identify() { std::cout << myName << " reporting\n"; }

protected:
	const std::string myName;
};

class Circle : public Shape {
public:
	Circle(double r) : Shape("circle"), r(r) {
		std::cout << "\tc'tor Circle\n";
	}
	~Circle() { std::cout << "\td'tor Circle\n"; }

	double getArea() override { return M_PI * r * r; }
	//void identify() override { std::cout << "Circle reporting\n"; }

protected:
	double r;
};

class Square : public Shape {
public:
	Square(double e) : Shape("square"), e(e) {
		std::cout << "\tc'tor Square\n";
	}
	~Square() { std::cout << "\td'tor Square\n"; }

	double getArea() override { return e * e; }
	//void identify() override { std::cout << "Square reporting\n"; }

protected:
	double e;
};

class DirectShape {
public:
	DirectShape(const std::string && myName, const std::function<double(double)> areaFct, const double sizeParam)
		: myName(myName), areaFct(areaFct), sizeParam(sizeParam) {
		std::cout << "\tc'tor DirectShape\n";
	}
	~DirectShape() { std::cout << "\td'tor DirectShape\n"; }

	double getArea() { return areaFct(sizeParam); }
	void identify() { std::cout << myName << " reporting\n"; }

protected:
	const std::string myName;
	double sizeParam;
	const std::function<double(double)> areaFct;
};

int main(int argc, void** argv) {

	//std::shared_ptr<Shape> shape(new Shape("unidentified"));

	{
		std::shared_ptr<Shape> circle(new Circle(5.));
		std::shared_ptr<Shape> square(new Square(5.));

		circle->identify( );
		std::cout << "The area of our circle is " << circle->getArea( ) << std::endl;
		square->identify( );
		std::cout << "The area of our square is " << square->getArea( ) << std::endl;
	}
	{
		std::shared_ptr<DirectShape> circle(
			new DirectShape("Circle", [](double r) { return M_PI * r * r; }, 5.));
		std::shared_ptr<DirectShape> square(
			new DirectShape("Square", [](double r) { return r * r; }, 5.));

		circle->identify( );
		std::cout << "The area of our circle is " << circle->getArea( ) << std::endl;
		square->identify( );
		std::cout << "The area of our square is " << square->getArea( ) << std::endl;
	}

	system("pause");	
}
