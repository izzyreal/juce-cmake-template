#include <juce_gui_basics/juce_gui_basics.h>

#include <vector>

struct node {
    std::string name;
    std::string svg;
    std::vector<node> children;
    juce::Component* component;
    float margin;
    std::string label;
};

class View : public juce::Component {

    public:
        View();
        ~View() override;
        
        void resized() override;

    private:
        std::string name = "default";
        std::vector<juce::Component*> components;
        node view_root;

        void addViewNodesAsJuceComponents(node& n);
        void createFlexBoxes(juce::FlexBox& parent, node &n, std::vector<juce::FlexBox> &flexBoxes);
};
