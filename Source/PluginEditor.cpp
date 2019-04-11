/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThirdApGuiAudioProcessorEditor::ThirdApGuiAudioProcessorEditor (ThirdApGuiAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setOpaque(true);
    setSize (400, 300);
    
    freqSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    freqSlider.setRange(20.0f, 2000.0f);
    freqSlider.setTextValueSuffix("Hz");
    freqSlider.setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::orchid);
    
    freqSlider.addListener(this);
    addAndMakeVisible(freqSlider);
    freqSlider.setValue(300.0f);
    
    addAndMakeVisible(freqLabel);
    freqLabel.setColour(Label::textColourId, Colours::lightgrey);
    freqLabel.setFont (Font("Calibri", 15.0f, Font::italic));
    freqLabel.setText("Frequency", dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, true);
    
    
    
    
    mixSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    mixSlider.setRange(0, 100.0);
    mixSlider.setTextValueSuffix("%");
    mixSlider.setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::orchid);
    
    mixSlider.addListener(this);
    addAndMakeVisible(mixSlider);
    
    addAndMakeVisible(mixLabel);
    mixLabel.setColour(Label::textColourId, Colours::lightgrey);
    mixLabel.setFont (Font("Calibri", 15.0f, Font::italic));
    mixLabel.setText("Wet/Dry Mix", dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, true);
    
}

ThirdApGuiAudioProcessorEditor::~ThirdApGuiAudioProcessorEditor()
{
}

//==============================================================================
void ThirdApGuiAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::mediumvioletred);
    
    g.setColour(Colours::purple);
    g.drawRect(0, 10 , getWidth(), 40);
    g.fillRect(0,10,getWidth(),40);
    
    
    g.setColour (Colours::hotpink);
    g.setFont (Font("Calibri", 45.0f, Font::bold));
    g.drawFittedText ("RING MOD", 10,10, getWidth(), 40, Justification::centredTop, 1);
    
    g.setColour(Colours::mediumpurple);
    g.drawRect(60, 75, 100, 175);
    g.fillRect(60, 75, 100, 175);
    
    g.setColour(Colours::deeppink);
    g.drawRect(150, 75, 150, 175);
    g.fillRect(150, 75, 150, 175);
    
}

void ThirdApGuiAudioProcessorEditor::resized()
{
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mixSlider.setBounds(150, 50, 150, 150);
    
    freqSlider.setBounds(150, 150, 150, 150);
    
}
void ThirdApGuiAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &mixSlider)
    {
        //get the value on our current slider, Pass that value to the audio processor
        processor.mixLevel.setTargetValue(mixSlider.getValue());
        
    } else if (slider ==&freqSlider){
        
        //get the value on our current slider, Pass that value to the audio processor
        processor.freqLevel.setTargetValue(freqSlider.getValue());
    }
    
}

