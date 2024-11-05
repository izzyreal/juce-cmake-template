#include <juce_gui_basics/juce_gui_basics.h>

#include <vector>

struct node {
    std::string name;
    std::string svg;
    std::vector<node> children;
    float margin;
    std::string label;
    std::string direction;
    float max_height;

    juce::Component* svg_component;
    juce::Component* label_component;
};

class View : public juce::Component {

    public:
        View();
        ~View() override;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        std::string name = "default";
        std::vector<juce::Component*> components;
        node view_root;

        void addViewNodesAsJuceComponents(node& n);
        void createFlexBoxes(juce::FlexBox& parent, node &n, std::vector<std::unique_ptr<juce::FlexBox>> &flexBoxes);
};
