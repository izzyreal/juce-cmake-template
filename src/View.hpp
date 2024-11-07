#include <juce_gui_basics/juce_gui_basics.h>

#include <vector>
#include <functional>

struct node {
    std::string name;
    std::string svg;
    std::vector<node> children;
    float margin;
    std::string label;
    std::string label_style;
    std::string direction;
    float flex_grow;
    bool spacer;
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

        void addViewNodesAsJuceComponents(node& n);
        void createFlexBoxes(juce::FlexBox& parent, node &n, std::vector<std::unique_ptr<juce::FlexBox>> &flexBoxes);
        float getLabelHeight(const std::string& text);
};
