#pragma once
#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include "Constants.hpp"
#include "melatonin_blur/melatonin/shadows.h"

class Lcd : public juce::Component, juce::Timer {
    public:
        Lcd()
        {
            startTimer(20);
        }

        void timerCallback() override
        {
            frame = (frame + 1) % 100;
            repaint();
        }

        void paint(juce::Graphics &g) override
        {

            std::vector<std::vector<bool>> rawPixels(60, std::vector<bool>(248));
            fillFrame(rawPixels);
            juce::Path p;
            juce::Image img(juce::Image::PixelFormat::RGB, 248*2, 60*2, false);

            for (uint8_t y = 0; y < 60; y++)
            {
                for (uint8_t x = 0; x < 248; x++)
                {
                    //const bool on = y == 0 || y == 59 || x == 0 || x == 247;
                    const bool on = rawPixels[y][x] || (x >= frame && x <= frame + 47 && y >= 5 && y <= 47);
                    drawLcdPixel(img, x, y, on);
                    if (!on) p.addRectangle(x, y, 1, 1);
                }
            }

            g.drawImage(img, getLocalBounds().toFloat());

            auto color = juce::Colours::white.withAlpha(1.0f);
            int radius = 2;
            juce::Point<int> offset = { 0, 0 };
            int spread = 1;
            melatonin::DropShadow shadow = { color, radius, offset, spread };
            p.applyTransform(juce::AffineTransform().scaled(getWidth() / (248.f), getHeight() / (60.f)));
            setPaintingIsUnclipped(true);
            shadow.render(g, p);
        }

    private:
        uint8_t frame = 0;

        void drawLcdPixel(juce::Image &img, const uint8_t lcdX, const uint8_t lcdY, const bool on)
        {
            const juce::Colour c1 = on ? Constants::lcdOn : Constants::lcdOff;
            const juce::Colour c2 = on ? Constants::lcdOnLight : Constants::lcdOff;
            img.setPixelAt(lcdX*2, lcdY*2, c1);
            img.setPixelAt(lcdX*2 + 1, lcdY*2, c2);
            img.setPixelAt(lcdX*2 + 1, lcdY*2 + 1, c2);
            img.setPixelAt(lcdX*2, lcdY*2 + 1, c2);
        }

        void fillFrame(std::vector<std::vector<bool>>& vec) {
            const int width = 248;
            const int height = 60;
            const double pi = 3.141592653589793;
            vec.assign(height, std::vector<bool>(width, false));

            double angleX = 2 * pi * frame / 100.0;
            double angleY = 2 * pi * frame / 100.0;

            double cubeSize = 1;

            std::vector<std::vector<double>> vertices = {
                {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
                {-1, -1,  1}, {1, -1,  1}, {1, 1,  1}, {-1, 1,  1}
            };

            int edges[12][2] = {
                {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Bottom face
                {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Top face
                {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Vertical edges
            };

            auto project = [&](double x, double y, double z) {
                double scale = 100 / (z + 3);
                return std::make_pair(int(width / 2 + scale * x), int(height / 2 - scale * y));
            };

            auto rotateX = [&](double& y, double& z, double angle) {
                double newY = y * cos(angle) - z * sin(angle);
                double newZ = y * sin(angle) + z * cos(angle);
                y = newY; z = newZ;
            };

            auto rotateY = [&](double& x, double& z, double angle) {
                double newX = x * cos(angle) + z * sin(angle);
                double newZ = -x * sin(angle) + z * cos(angle);
                x = newX; z = newZ;
            };

            std::vector<std::pair<int, int>> projected;

            for (auto& vertex : vertices) {
                double x = cubeSize * vertex[0];
                double y = cubeSize * vertex[1];
                double z = cubeSize * vertex[2];

                rotateX(y, z, angleX);
                rotateY(x, z, angleY);

                projected.push_back(project(x, y, z));
            }

            auto drawLine = [&](int x0, int y0, int x1, int y1) {
                int dx = abs(x1 - x0), dy = abs(y1 - y0);
                int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
                int err = dx - dy;
                while (true) {
                    if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) vec[y0][x0] = true;
                    if (x0 == x1 && y0 == y1) break;
                    int e2 = 2 * err;
                    if (e2 > -dy) { err -= dy; x0 += sx; }
                    if (e2 < dx) { err += dx; y0 += sy; }
                }
            };

            for (auto& edge : edges) {
                int p1 = edge[0], p2 = edge[1];
                drawLine(projected[p1].first, projected[p1].second, projected[p2].first, projected[p2].second);
            }
        }

};

