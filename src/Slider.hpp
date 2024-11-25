#include <juce_gui_basics/juce_gui_basics.h>

#include "RectangleLabel.hpp"
#include "Constants.hpp"

class Slider : public juce::Component {
    public:
        Slider(const std::function<float()> &getScaleToUse)
            : getScale(getScaleToUse)
        {
            rectangleLabel = new RectangleLabel(getScaleToUse, "NOTE\nVARIATION", "VARIATION", Constants::chassisColour, Constants::labelColour, 0.f, 7.f);
            addAndMakeVisible(rectangleLabel);
        }
        
        ~Slider() override
        {
            delete rectangleLabel;
        }

        void resized() override
        {
            juce::Grid grid;
            grid.templateRows = { juce::Grid::Fr(1), juce::Grid::Fr(8) };
            grid.templateColumns = { juce::Grid::Fr(1) };

            grid.items.add(juce::GridItem(rectangleLabel).withArea(1, 1, 1, 1));

            grid.performLayout(getLocalBounds());
        }

        void paint(juce::Graphics &g) override
        {
            g.setColour(Constants::darkLabelColour);

            const auto scale = getScale();
            const auto outer_rect = getLocalBounds().toFloat().withTop((Constants::BASE_FONT_SIZE + Constants::LINE_SIZE + 1) * scale);

            const auto line_thickness = std::ceil<float>(Constants::lineThickness1 * scale);
            
            g.drawRoundedRectangle(outer_rect.reduced(line_thickness), 0.4 * scale, line_thickness);

            const auto width = getWidth();
            const auto height = getHeight();

            const auto line_length = width * 0.2;
            const auto x_offset = width * 0.18;
            const auto y_interval = height * 0.05;
            const auto y_offset_bottom = height * 0.1;

            for (int i = 0; i < 11; i++)
            {
                const auto y_pos = height - (i * y_interval) - y_offset_bottom;
                g.drawLine(x_offset, y_pos, line_length + x_offset, y_pos);
                g.drawLine(width - line_length - x_offset, y_pos, width - x_offset, y_pos);
            }

            auto notch_rect = getLocalBounds().toFloat().reduced(width * 0.465, height * 0.205);
            notch_rect.setY(height - notch_rect.getHeight() - (y_offset_bottom / 2));

            g.setColour(juce::Colours::black);
            g.drawRoundedRectangle(notch_rect, 0.4f * scale, 1.f);
            g.fillRoundedRectangle(notch_rect, 0.4f * scale);

            Component::paint(g);
        }

    private:
        RectangleLabel *rectangleLabel = nullptr;
        const std::function<float()> &getScale;
};
