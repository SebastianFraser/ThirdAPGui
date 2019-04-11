
/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThirdApGuiAudioProcessor::ThirdApGuiAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", AudioChannelSet::stereo(), true)
#endif
                  )
#endif
{
    currentSampleRate = 0.0f;
    currentAngle = 0.0f;
    angleDelta = 0.0f;
    sinFreq = 300.0f;
    
    
    
}



ThirdApGuiAudioProcessor::~ThirdApGuiAudioProcessor()
{
}

//==============================================================================
const String ThirdApGuiAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ThirdApGuiAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ThirdApGuiAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ThirdApGuiAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ThirdApGuiAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ThirdApGuiAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int ThirdApGuiAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ThirdApGuiAudioProcessor::setCurrentProgram (int index)
{
}

const String ThirdApGuiAudioProcessor::getProgramName (int index)
{
    return {};
}




void ThirdApGuiAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ThirdApGuiAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    currentSampleRate = sampleRate;
    sinFreq = 300.0f;
    
    updateAngleDelta();
    
    
    
    //initilize mixLevel
    mixLevel.reset(sampleRate, 0.1f);
    mixLevel.setTargetValue(0.25f);
    
    //initilize freqlevel
    freqLevel.reset(sampleRate, 0.01f);
    freqLevel.setTargetValue(0.0);
    
    
    
    gain.setGainDecibels(12.0f);
    
    String message;
    message << "Preparing to play..." << newLine;
    message << "My sample rate is" << currentSampleRate << newLine;
    message << "Buffer size is" << samplesPerBlock << newLine;
    Logger::getCurrentLogger()->writeToLog(message);
}


void ThirdApGuiAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ThirdApGuiAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}
#endif

void ThirdApGuiAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    AudioBuffer<float> wetBuffer(getTotalNumInputChannels(), buffer.getNumSamples());
    wetBuffer.makeCopyOf(buffer);
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        auto* wetData = wetBuffer.getWritePointer(channel);
        
        
        
        for(int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            //generating a set of random values to modulate input aplitude
            //float modulator = random.nextFloat() * 0.25 - 0.125;
            //wetData[sample] = wetData[sample] * modulator;
            
            
            //Ring Mod \/
            
            sinFreq = freqLevel.getNextValue();
            updateAngleDelta();
            
            // Best to use a slow piano sample as it does cool stuff to the frequencies and also can kind of sound like a vinyl crackle sim. Doesnt work well with drums as it picks out som gross frequencies
            
            //calculate value to put into buffer
            auto currentSinSample = (float) std::sin(currentAngle);
            currentAngle += angleDelta; //currentAngle = currentAngle + angleDelta
            
            float deviations = random.nextFloat() * 0.1 - 0.05; //lets make our sin wave wiggle
            wetData[sample] = wetData[sample] * (currentSinSample + deviations);
            //adding original dry signal with processed signal into our output buffer (aka input buffer)
            wetData[sample] = wetData[sample] * currentSinSample;
            
            //auto shapedSample = (float) std::tanh(wetData[sample]) ;
            
            
            //wetData[sample] = shapedSample;
            
            // Hard Clip Change value to -0.1 and 0.1 for more dist
            
            //Wet/dry Mix
            
            //channelData[sample] = channelData[sample] * 0.4 + wetData[sample] * 0.6f;
            
            
            
            //Gain down so no clipping
            
            //channelData[sample] = channelData[sample] * 0.4;
            
            //channelData[sample] = channelData[sample] * 0.5f + wetData[sample] 0.5f;
            channelData[sample] = channelData[sample] * (1.0f - (mixLevel.getNextValue()*0.01f)) + ((wetData[sample] * mixLevel.getNextValue()) * 0.01f);
            
            channelData[sample] = channelData[sample] *0.4f;
        }
    }
    dsp::AudioBlock<float> output(buffer);
    gain.process(dsp::ProcessContextReplacing<float>(output));
}




//==============================================================================
bool ThirdApGuiAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ThirdApGuiAudioProcessor::createEditor()
{
    return new ThirdApGuiAudioProcessorEditor (*this);
}

//==============================================================================
void ThirdApGuiAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ThirdApGuiAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}



//==============================================================================
// User Functions

void ThirdApGuiAudioProcessor:: updateAngleDelta()
{
    //Calculate no. of cycles we will need to complete for each output sample
    auto cyclePerSample = sinFreq / currentSampleRate;
    //auto cyclesPerSample = freqLevel.getValue() / currentSampleRate;
    // Multiply by the length of a wholse sin wave cycle
    angleDelta = cyclePerSample *  MathConstants<float>::twoPi;
    
}


// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThirdApGuiAudioProcessor();
    
}



