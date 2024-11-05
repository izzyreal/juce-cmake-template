#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_core/juce_core.h>

class SvgComponent : public juce::Component
{
    private:
        void loadSvgFile(juce::File f)
        {
            juce::FileInputStream svgStream(f);

            if (svgStream.openedOk())
            {   
                auto svgXml = juce::XmlDocument::parse(f);
                if (svgXml != nullptr)
                {   
                    svgDrawable = juce::Drawable::createFromSVG(*svgXml);
                    svgFile = f;
                    randomColor = juce::Colour::fromRGB(juce::String(svgFile.getFileName()).hashCode() & 0xFF,
                                          (juce::String(svgFile.getFileName()).hashCode() >> 8) & 0xFF,
                                          (juce::String(svgFile.getFileName()).hashCode() >> 16) & 0xFF);
                }
            }
        }
    public:
        SvgComponent(std::string svg_path)
        {
            loadSvgFile(juce::File("/Users/izmar/projects/VMPC2000XL/vector UI/views/" + svg_path));
        }

        void paint(juce::Graphics& g) override
        {
            //g.fillAll(randomColor);
            if (svgDrawable != nullptr)
            {
                svgDrawable->drawWithin(g, getLocalBounds().toFloat(), juce::RectanglePlacement::yTop, 1.0f);
            }
            else
            {
                g.fillAll(juce::Colours::red);
            }
        }

    private:
        juce::File svgFile;
        std::unique_ptr<juce::Drawable> svgDrawable;
        juce::Colour randomColor;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SvgComponent)
};
