#pragma once
#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include "Constants.hpp"
#include "melatonin_blur/melatonin/shadows.h"

class Lcd : public juce::Component {
    public:
        void paint(juce::Graphics &g) override
        {
            g.fillAll(Constants::lcdOffBacklit);
            g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);
            std::vector<std::vector<bool>> rawPixels(60, std::vector<bool>(248));
            fillFrame(rawPixels);

            juce::Path p;
            juce::Image img(juce::Image::PixelFormat::RGB, 248*2, 60*2, false);

            const auto asp_ratio = 60.f/248.f;
            const auto w = float(getWidth()) * 0.55f;
            const auto h = w * asp_ratio;

            const auto img_scale = w / (248 * 2);
            const auto unused_h_px = getWidth() - w;
            const auto unused_v_px = getHeight() - h;
            const auto x_offset = unused_h_px * 0.5f;
            const auto y_offset = unused_v_px * 0.5f;

            for (uint8_t y = 0; y < 60; y++)
            {
                for (uint8_t x = 0; x < 248; x++)
                {
                    const bool on1 = y == 0 || y == 59 || x == 0 || x == 247;
                    const bool on = on1 || rawPixels[y][x] || (false && x >= frame && x <= frame + 47 && y >= 5 && y <= 47);
                    drawLcdPixel(img, x, y, on);
                    if (!on) p.addRectangle(x*2, y*2, 2, 2);
                }
            }

            auto color = Constants::lcdOffBacklit.brighter().withAlpha(0.4f);
            int radius = (int) std::round<float>((float)getWidth() / 248.f);
            juce::Point<int> offset = { 0, 0 };
            int spread = 0.f;
            melatonin::DropShadow shadow = { color, radius, offset, spread };
            
            juce::AffineTransform t;
            t = t.scaled(img_scale);
            t = t.translated(x_offset, y_offset);

            p.applyTransform(t);
            g.drawImageTransformed(img, t);

            /*if (frame < 50)*/ shadow.render(g, p);
        }

    private:
        uint8_t frame = 0;

        void drawLcdPixel(juce::Image &img, const uint8_t lcdX, const uint8_t lcdY, const bool on)
        {
            const juce::Colour c1 = on ? Constants::lcdOn : Constants::lcdOffBacklit;
            const juce::Colour c2 = on ? Constants::lcdOnLight : Constants::lcdOffBacklit;
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

