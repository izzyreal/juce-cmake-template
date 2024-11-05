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
                }    
            }
        }
    public:
        SvgComponent(std::string svg_path)
        {
            printf("Creating SvgComponent for %s\n", svg_path.c_str());
            loadSvgFile(juce::File("/Users/izmar/projects/VMPC2000XL/vector UI/views/" + svg_path));
        }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::yellow);
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

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SvgComponent)
};
