#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <iomanip>
#include <CanvasPoint.h>
#include <Colour.h>
#include <cstdlib>
#include <algorithm>
#include <TextureMap.h>
#include <ModelTriangle.h>
#include <sstream>
#include <glm/gtx/string_cast.hpp>
#include <map>
#include <string>

#define WIDTH 320
#define HEIGHT 240

void redNoise(DrawingWindow &window) {
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			float red = rand() % 256;
			float green = 0.0;
			float blue = 0.0;
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			window.setPixelColour(x, y, colour);
		}
	}
}

std::vector<float> interpolateSingleFloats(float from, float to, size_t numberOfValues) {
	std::vector<float> result;
	float step = (to - from) / (numberOfValues - 1);
	for (size_t i = 0; i < numberOfValues; i++) {
		result.push_back(from + (i * step));
	}
	return result;
}

void interpolateSingleFloatsExample() {
    std::vector<float> result = interpolateSingleFloats(2.2, 8.5, 7);
    for (size_t i = 0; i < result.size(); i++) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;
}

std::vector<glm::vec3> interpolateThreeElementValues(const glm::vec3& from, const glm::vec3& to, int numberOfValues) {
	std::vector<glm::vec3> result;
	float stepSize = (1.0f) / (numberOfValues - 1);
	for (int i = 0; i < numberOfValues; i++) {
		float t = stepSize * i;
		glm::vec3 interpolatedValue = from + (t * (to - from));
		result.push_back(interpolatedValue);
	}
	return result;
}

void interpolateThreeElementValuesExample() {
    glm::vec3 from(1.0, 4.0, 9.2);
    glm::vec3 to(4.0, 1.0, 9.8);
    std::vector<glm::vec3> result = interpolateThreeElementValues(from, to, 4);
    for (size_t i = 0; i < result.size(); i++) {
        std::cout << "(" << result[i].x << ", " << result[i].y << ", " << result[i].z << ") ";
    }
    std::cout << std::endl;
}

void singleDimensionGreyscaleInterpolation(DrawingWindow& window) {
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			float gradient = 1.0 - static_cast<float>(x) / window.width;
			uint8_t grey = gradient * 255;
			uint32_t colour = (255 << 24) + (grey << 16) + (grey << 8) + grey;
			window.setPixelColour(x, y, colour);
		}
	}
}

void twoDimensionalColourInterpolation(DrawingWindow& window) {
	glm::vec3 topLeft(255, 0, 0);
	glm::vec3 topRight(0, 0, 255);
	glm::vec3 bottomRight(0, 255, 0);
	glm::vec3 bottomLeft(255, 255, 0);
	std::vector<glm::vec3> leftColumnColours = interpolateThreeElementValues(topLeft, bottomLeft, window.height);
	std::vector<glm::vec3> rightColumnColours = interpolateThreeElementValues(topRight, bottomRight, window.height);
	for (size_t y = 0; y < window.height; y++) {
		std::vector<glm::vec3> rowColours = interpolateThreeElementValues(leftColumnColours[y], rightColumnColours[y], window.width);
		for (size_t x = 0; x < window.width; x++) {
            uint32_t finalColour = (255 << 24) + (int(rowColours[x].x) << 16) + (int(rowColours[x].y) << 8) + int(rowColours[x].z);
			window.setPixelColour(x, y, finalColour);
		}
	}
}

void drawLine(CanvasPoint from, CanvasPoint to, Colour colour, DrawingWindow& window) {
    float xDiff = to.x - from.x;
    float yDiff = to.y - from.y;
    float numberOfSteps = std::max(std::abs(xDiff), std::abs(yDiff));
    float xStepSize = xDiff / numberOfSteps;
    float yStepSize = yDiff / numberOfSteps;
    for (float i = 0.0; i < numberOfSteps; i++) {
        float x = from.x + (xStepSize * i);
        float y = from.y + (yStepSize * i);
        window.setPixelColour(round(x), round(y), colour.toUint32());
    }
}

void drawStraightLines(DrawingWindow& window) {
	CanvasPoint topLeft(0, 0);
	CanvasPoint center(window.width / 2, window.height / 2);
	CanvasPoint middle(window.width / 2, 0);
	Colour red(255, 0, 0);
	Colour green(0, 255, 0);
	Colour blue(0, 0, 255);
	drawLine(topLeft, center, red, window);
	drawLine(middle, CanvasPoint(middle.x, window.height), green, window);
	drawLine(CanvasPoint(window.width / 3, window.height / 2), CanvasPoint((2 * window.width) / 3, window.height / 2), blue, window);
}

void drawCanvasStrokedTriangle(CanvasTriangle triangle, Colour colour, DrawingWindow& window) {
	drawLine(triangle[0], triangle[1], colour, window);
	drawLine(triangle[1], triangle[2], colour, window);
	drawLine(triangle[2], triangle[0], colour, window);
}

void drawRandomTriangle(DrawingWindow& window) {
	CanvasPoint v1(rand() % window.width, rand() % window.height);
	CanvasPoint v2(rand() % window.width, rand() % window.height);
	CanvasPoint v3(rand() % window.width, rand() % window.height);
	Colour colour(rand() % 256, rand() % 256, rand() % 256);
	CanvasTriangle triangle(v1, v2, v3);
	drawCanvasStrokedTriangle(triangle, colour, window);
}

void drawFilledTriangle(CanvasTriangle triangle, Colour colour, DrawingWindow& window) {
    std::vector<CanvasPoint> sortedPoints = { triangle.v0(), triangle.v1(), triangle.v2() };
    std::sort(sortedPoints.begin(), sortedPoints.end(), [](const CanvasPoint& a, const CanvasPoint& b) {
        return a.y < b.y;
    });
    CanvasPoint top = sortedPoints[0];
    CanvasPoint middle = sortedPoints[1];
    CanvasPoint bottom = sortedPoints[2];
    float leftStepSize = (middle.x - top.x) / (middle.y - top.y);
    float rightStepSize = (bottom.x - top.x) / (bottom.y - top.y);
    for (int y = top.y; y <= middle.y; y++) {
        int startX = round(top.x + leftStepSize * (y - top.y));
        int endX = round(top.x + rightStepSize * (y - top.y));
        drawLine(CanvasPoint(startX, y), CanvasPoint(endX, y), colour, window);
    }
    float newLeftStepSize = (bottom.x - middle.x) / (bottom.y - middle.y);
    for (int y = middle.y; y <= bottom.y; y++) {
        int startX = round(middle.x + newLeftStepSize * (y - middle.y));
        int endX = round(top.x + rightStepSize * (y - top.y));
        drawLine(CanvasPoint(startX, y), CanvasPoint(endX, y), colour, window);
    }
    drawCanvasStrokedTriangle(triangle, Colour(255, 255, 255), window);
}

void drawRandomFilledTriangle(DrawingWindow& window) {
    CanvasTriangle triangle;
    Colour colour(rand() % 256, rand() % 256, rand() % 256);
    CanvasPoint v1(rand() % window.width, rand() % window.height);
    CanvasPoint v2(rand() % window.width, rand() % window.height);
    CanvasPoint v3(rand() % window.width, rand() % window.height);
    triangle = CanvasTriangle(v1, v2, v3);
    drawFilledTriangle(triangle, colour, window);
}

void drawTexturedLine(DrawingWindow& window, CanvasPoint from, CanvasPoint to, TextureMap texture) {
    float xDiff = to.x - from.x;
    float yDiff = to.y - from.y;
    float textureXDiff = to.texturePoint.x - from.texturePoint.x;
    float textureYDiff = to.texturePoint.y - from.texturePoint.y;
    float numberOfSteps = std::max(abs(xDiff), abs(yDiff));
    float xStepSize = xDiff / numberOfSteps;
    float yStepSize = yDiff / numberOfSteps;
    float textureXstepSize = textureXDiff / numberOfSteps;
    float textureYstepSize = textureYDiff / numberOfSteps;
    for (float i = 0.0; i < numberOfSteps; i++) {
        float x = from.x + (xStepSize * i);
        float y = from.y + (yStepSize * i);
        float textureX = from.texturePoint.x + (textureXstepSize * i);
        float textureY = from.texturePoint.y + (textureYstepSize * i);
        textureX = std::max(0.0f, std::min(textureX, static_cast<float>(texture.width - 1)));
        textureY = std::max(0.0f, std::min(textureY, static_cast<float>(texture.height - 1)));
        uint32_t color = texture.pixels[static_cast<int>(textureX) + (texture.width * static_cast<int>(textureY))];
        window.setPixelColour(static_cast<int>(round(x)), static_cast<int>(round(y)), color);
    }
}

float interpolation(float x, float x1, float x2, float y1, float y2) {
	return y1 + (x - x1) * ((y2 - y1) / (x2 - x1));
}

void drawTexturedTriangle(DrawingWindow& window, CanvasTriangle triangle, TextureMap& map) {
    if ((triangle.v0().y) > (triangle.v1()).y)
        std::swap(triangle.vertices[0], triangle.vertices[1]);
    if ((triangle.v0().y) > (triangle.v2()).y)
        std::swap(triangle.vertices[0], triangle.vertices[2]);
    if ((triangle.v1().y) > (triangle.v2()).y)
        std::swap(triangle.vertices[1], triangle.vertices[2]);
    float xDiff = (triangle.v2().x) - (triangle.v0().x);
    float yDiff = (triangle.v2().y) - (triangle.v0().y);
    float ratio = xDiff / yDiff;
    float x = (triangle.v1().y - triangle.v0().y) * (ratio);
    CanvasPoint subPoint = CanvasPoint((x + triangle.v0().x), triangle.v1().y);
    float firstDiff = triangle.v1().y - triangle.v0().y;
    float secondDiff = triangle.v2().y - triangle.v1().y;
    std::vector<float> sideA1 = interpolateSingleFloats(triangle.v0().x, subPoint.x, firstDiff);
    std::vector<float> sideA2 = interpolateSingleFloats(triangle.v0().x, triangle.v1().x, firstDiff);
    std::vector<float> sideB1 = interpolateSingleFloats(subPoint.x, triangle.v2().x, secondDiff);
    std::vector<float> sideB2 = interpolateSingleFloats(triangle.v1().x, triangle.v2().x, secondDiff);
    float subPointX = interpolation(subPoint.x, triangle.v2().x, triangle.v0().x, triangle.v2().texturePoint.x, triangle.v0().texturePoint.x);
    float subPointY = interpolation(subPoint.y, triangle.v2().y, triangle.v0().y, triangle.v2().texturePoint.y, triangle.v0().texturePoint.y);
    for (size_t i = 0; i < firstDiff; i++) {
		CanvasPoint point1(sideA1[i], i + triangle.v0().y);
		TexturePoint texturePoint1 = TexturePoint(interpolation(sideA1[i], subPoint.x, triangle.v0().x, subPointX, triangle.v0().texturePoint.x), interpolation(i + triangle.v0().y, subPoint.y, triangle.v0().y, subPointY, triangle.v0().texturePoint.y));
		point1.texturePoint = texturePoint1;
		CanvasPoint point2(sideA2[i], i + triangle.v0().y);
		TexturePoint texturePoint2 = TexturePoint(interpolation(sideA2[i], triangle.v0().x, triangle.v1().x, triangle.v0().texturePoint.x, triangle.v1().texturePoint.x), interpolation(i + triangle.v0().y, triangle.v0().y, triangle.v1().y, triangle.v0().texturePoint.y, triangle.v1().texturePoint.y));
		point2.texturePoint = texturePoint2;
        drawTexturedLine(window, point1, point2, map);
    }
    for (size_t i = 0; i < secondDiff; i++) {
		CanvasPoint point1(sideB1[i], i + triangle.v1().y);
		TexturePoint texturePoint1 = TexturePoint(interpolation(sideB1[i], subPoint.x, triangle.v2().x, subPointX, triangle.v2().texturePoint.x), interpolation(i + triangle.v1().y, subPoint.y, triangle.v2().y, subPointY, triangle.v2().texturePoint.y));
		point1.texturePoint = texturePoint1;
		CanvasPoint point2(sideB2[i], i + triangle.v1().y);
		TexturePoint texturePoint2 = TexturePoint(interpolation(sideB2[i], triangle.v2().x, triangle.v1().x, triangle.v2().texturePoint.x, triangle.v1().texturePoint.x), interpolation(i + triangle.v1().y, triangle.v2().y, triangle.v1().y, triangle.v2().texturePoint.y, triangle.v1().texturePoint.y));
		point2.texturePoint = texturePoint2;
        drawTexturedLine(window, point1, point2, map);
    }
    drawCanvasStrokedTriangle(triangle, Colour(255, 255, 255), window);
}

void drawTexturedTriangleExample(DrawingWindow& window) {
    TextureMap textureMap("texture.ppm");
    CanvasPoint v0(160, 10);
    v0.texturePoint = TexturePoint(195, 5);
    CanvasPoint v1(300, 230);
    v1.texturePoint = TexturePoint(395, 380);
    CanvasPoint v2(10, 150);
    v2.texturePoint = TexturePoint(65, 330);
    CanvasTriangle triangle(v0, v1, v2);
    drawTexturedTriangle(window, triangle, textureMap);
}

std::vector<ModelTriangle> loadOBJ(const std::string& filename, float scale) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filename << std::endl;
        return {};
    }
    std::vector<glm::vec3> vertices;
    std::vector<ModelTriangle> triangles;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.push_back(glm::vec3(x * scale, y * scale, z * scale));
        } else if (type == "f") {
            std::string v1, v2, v3;
            ss >> v1 >> v2 >> v3;
            int index1 = std::stoi(v1) - 1;
            int index2 = std::stoi(v2) - 1;
            int index3 = std::stoi(v3) - 1;
            triangles.push_back(ModelTriangle(vertices[index1], vertices[index2], vertices[index3], Colour()));
        }
    }
    return triangles;
}

void draw(DrawingWindow &window) {
	window.clearPixels();
	// redNoise(window);
	// interpolateSingleFloatsExample();
	// interpolateThreeElementValuesExample();
	// twoDimensionalColourInterpolation(window);
	// singleDimensionGreyscaleInterpolation(window);
	// drawStraightLines(window);
	// drawTexturedTriangleExample(window);
    std::vector<ModelTriangle> triangles = loadOBJ("cornell-box.obj", "cornell-box.mtl", 0.35);
    for (const auto& triangle : triangles) {
        std::cout << "Triangle: " << triangle << std::endl;
    }
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_LEFT)
            std::cout << "LEFT" << std::endl;
        else if (event.key.keysym.sym == SDLK_RIGHT)
            std::cout << "RIGHT" << std::endl;
        else if (event.key.keysym.sym == SDLK_UP)
            std::cout << "UP" << std::endl;
        else if (event.key.keysym.sym == SDLK_DOWN)
            std::cout << "DOWN" << std::endl;
        else if (event.key.keysym.sym == SDLK_u) 
			drawRandomTriangle(window);
		else if (event.key.keysym.sym == SDLK_f) 
			drawRandomFilledTriangle(window);
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        window.savePPM("output.ppm");
        window.saveBMP("output.bmp");
    }
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) 
			handleEvent(event, window);
		draw(window);	
		// Need to render the frame at the end, or nothing actually gets shown on the screen!
		window.renderFrame();
	}
	return 0;
}
