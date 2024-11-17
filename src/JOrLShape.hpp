#include <juce_gui_basics/juce_gui_basics.h>

#include "Constants.hpp"

class JOrLShape : public juce::Component {
    public:
        enum Shape { J, L };

        JOrLShape(const Shape shapeToUse) : shape(shapeToUse)
    {}

        void paint(juce::Graphics &g) override
        {
            const auto line_thickness = 3.f;
            const auto half_line_thickness = line_thickness / 2;
            const auto line_top = half_line_thickness;
            const auto line_left = half_line_thickness;
            const auto line_right = getWidth() - half_line_thickness;
            const auto line_bottom = getHeight() - half_line_thickness;
            g.setColour(Constants::darkLabelColour);
            g.drawLine(line_left, line_bottom, line_right, line_bottom, line_thickness);

            switch (shape)
            {
                case Shape::J:
                    g.drawLine(line_right, line_top, line_right, line_bottom, line_thickness);
                    g.drawEllipse(line_right, line_bottom, line_thickness, line_thickness, line_thickness);
                    break;
                case Shape::L:
                default:
                    g.drawLine(line_left, line_top, line_left, line_bottom, line_thickness);
                    g.drawEllipse(line_left, line_bottom, line_thickness, line_thickness, line_thickness);
                    break;
            }
        }

    private:
        const Shape shape;
};
