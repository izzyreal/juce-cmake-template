#include <juce_gui_basics/juce_gui_basics.h>

#include <vector>
#include <functional>

struct node {
    std::string name;
    std::string node_type;
    std::string svg;
    std::vector<node> children;
    float margin;
    std::string label;
    std::string label_style;
    std::string direction;
    float flex_grow;
    std::string align_items;

    juce::Component* svg_component;
    juce::Component* label_component;
};

class View : public juce::Component {

    public:
        View(const std::function<float()>& getScale);
        ~View() override;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        std::string name = "default";
        std::vector<juce::Component*> components;
        node view_root;
        const std::function<float()> getScale;

        void processFlexBox(
                juce::FlexBox& parent,
                const std::vector<node>& children,
                std::vector<std::unique_ptr<juce::FlexBox>>& flexBoxes,
                const std::string& alignItems,
                const std::string& direction,
                const float flexGrow);

        void processSvgWithLabel(
                juce::FlexBox& parent,
                std::vector<std::unique_ptr<juce::FlexBox>>& flexBoxes,
                const float minWidth,
                const float minHeight,
                const float flexGrow,
                const std::string& labelText,
                juce::Component* labelComponent,
                juce::Component* svgComponent);

        void addViewNodesAsJuceComponents(node& n);
        void processChildren(juce::FlexBox& parent, const std::vector<node>& children, std::vector<std::unique_ptr<juce::FlexBox>> &flexBoxes);
        float getLabelHeight(const std::string& text);
};
