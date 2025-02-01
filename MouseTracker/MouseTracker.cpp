#include <iostream>
#include <Windows.h>
#include <cmath>

using namespace std;

void static setTextColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

double static getChange(int x, int y) {
	return sqrt(pow(x, 2)) + sqrt(pow(y, 2));
}

int main() {
	POINT point = { 0, 0 };

	while (TRUE) {
		POINT thisPoint;

		if (!GetCursorPos(&thisPoint))
			continue;

		int deltaX = thisPoint.x - point.x;
		int deltaY = thisPoint.y - point.y;

		double change = getChange(deltaX, deltaY);

		if (change <= 0)
			continue;

		if (change < 10) {
			setTextColor(0x0A);
		}
		else {
			setTextColor(0x0C);
		}

		if (deltaX != 0 || deltaY != 0) {
			cout << "[" << change << "] ";

			setTextColor(0x0F);

			cout << "(" << thisPoint.x << ", " << thisPoint.y << ") ";
			cout << "-> (" << point.x << ", " << point.y << ")\n";

			point = thisPoint;
		}

		Sleep(100);
	}

	return EXIT_SUCCESS;
}