#pragma once

#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_core/juce_core.h>

#include <cassert>

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
                    svgDrawable->getDrawableBounds();
                    svgFile = f;
                    randomColor = juce::Colour::fromRGB(juce::String(svgFile.getFileName()).hashCode() & 0xFF,
                            (juce::String(svgFile.getFileName()).hashCode() >> 8) & 0xFF,
                            (juce::String(svgFile.getFileName()).hashCode() >> 16) & 0xFF);
                }
            }
        }

    public:
        SvgComponent(std::string svg_path, juce::Component *commonParentWithShadowToUse)
            : commonParentWithShadow(commonParentWithShadowToUse)
        {
            loadSvgFile(juce::File("/Users/izmar/projects/VMPC2000XL/vector UI/views/" + svg_path));

            class ParentSizeAndPositionListener : public juce::ComponentListener {
                public:
                    SvgComponent *svgComponent = nullptr;
                    void componentMovedOrResized (Component &component, bool wasMoved, bool wasResized) {
                        svgComponent->syncShadowSiblingSizeAndPosition();
                    }
            };

            auto listener = new ParentSizeAndPositionListener();
            listener->svgComponent = this;
            parentSizeAndPositionListener = listener;
            commonParentWithShadow->addComponentListener(parentSizeAndPositionListener);
        }

        ~SvgComponent() override
        {
            delete parentSizeAndPositionListener;
        }

        juce::Rectangle<float> getDrawableBounds()
        {
            return svgDrawable == nullptr ? juce::Rectangle<float>() : svgDrawable->getDrawableBounds();
        }

        juce::Path getShadowPath()
        {
            if (svgDrawable == nullptr) return juce::Path();

            juce::Path shadowPath = svgDrawable->getOutlineAsPath();
            auto centred = juce::RectanglePlacement(juce::RectanglePlacement::centred);
            auto transform = centred.getTransformToFit(getDrawableBounds(), getLocalBounds().toFloat());
            shadowPath.applyTransform(transform);
            return shadowPath;
        }

        void paint(juce::Graphics& g) override
        {
            //g.fillAll(randomColor);
            if (svgDrawable == nullptr)
            {
                g.fillAll(juce::Colours::red);
                return;
            }

            svgDrawable->drawWithin(g, getLocalBounds().toFloat(), juce::RectanglePlacement::centred, 1.0f);
        }

        juce::Component *shadow = nullptr;

        void syncShadowSiblingSizeAndPosition()
        {
            if (shadow == nullptr) return;
            
            auto globalTopLeft = localPointToGlobal(juce::Point<int>(0, 0));
            auto relativeTopLeft = commonParentWithShadow->getLocalPoint(nullptr, globalTopLeft);
            juce::Rectangle<int> boundsInCommonParent(relativeTopLeft.x, relativeTopLeft.y, getWidth(), getHeight()); 

            // Expand either by a fixed number that covers the biggest of shadows, or let each shadow expand
            // only what is necessary given its radius, offset and spread. For now, we just use a really big
            // number to cover the biggest of shadows.
            boundsInCommonParent.expand(25.f, 25.f);

            shadow->setBounds(boundsInCommonParent);
        }

        void resized() override
        {
            syncShadowSiblingSizeAndPosition();
        }

    private:
        juce::File svgFile;
        std::unique_ptr<juce::Drawable> svgDrawable;
        juce::Colour randomColor;
        juce::Component *commonParentWithShadow = nullptr;
        juce::ComponentListener *parentSizeAndPositionListener = nullptr;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SvgComponent)
};
