/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "IAAEffectProcessor.h"
#include "IAAEffectEditor.h"


IAAEffectProcessor::IAAEffectProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                       .withOutput ("Output", AudioChannelSet::stereo(), true)),
       parameters (*this, nullptr)
{
    parameters.createAndAddParameter ("gain",
                                      "Gain",
                                      String(),
                                      NormalisableRange<float> (0.0f, 1.0f),
                                      (float) (1.0 / 3.14),
                                      nullptr,
                                      nullptr);

    parameters.state = ValueTree (Identifier ("InterAppAudioEffect"));
    
    
    // use a default samplerate and vector size here, reset it later
    m_C74PluginState = (CommonState *)C74_GENPLUGIN::create(44100, 64);
    C74_GENPLUGIN::reset(m_C74PluginState);
    
    m_InputBuffers = new t_sample *[C74_GENPLUGIN::num_inputs()];
    m_OutputBuffers = new t_sample *[C74_GENPLUGIN::num_outputs()];
    
    for (int i = 0; i < C74_GENPLUGIN::num_inputs(); i++) {
        m_InputBuffers[i] = NULL;
    }
    for (int i = 0; i < C74_GENPLUGIN::num_outputs(); i++) {
        m_OutputBuffers[i] = NULL;
    }
    
}

IAAEffectProcessor::~IAAEffectProcessor()
{
    C74_GENPLUGIN::destroy(m_C74PluginState);
}

//==============================================================================
const String IAAEffectProcessor::getName() const
{
    return JucePlugin_Name;
}

bool IAAEffectProcessor::acceptsMidi() const
{
    return false;
}

bool IAAEffectProcessor::producesMidi() const
{
    return false;
}

double IAAEffectProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int IAAEffectProcessor::getNumPrograms()
{
    return 1;
}

int IAAEffectProcessor::getCurrentProgram()
{
    return 0;
}

void IAAEffectProcessor::setCurrentProgram (int)
{
}

const String IAAEffectProcessor::getProgramName (int)
{
    return String();
}

void IAAEffectProcessor::changeProgramName (int, const String&)
{
}

//==============================================================================
void IAAEffectProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    previousGain = *parameters.getRawParameterValue ("gain");
    
    // initialize samplerate and vectorsize with the correct values
    m_C74PluginState->sr = sampleRate;
    m_C74PluginState->vs = samplesPerBlock;
    
}

void IAAEffectProcessor::releaseResources()
{
}

bool IAAEffectProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() != AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void IAAEffectProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    const float gain = *parameters.getRawParameterValue ("gain");

    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    const int numSamples = buffer.getNumSamples();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    // C74 Process code ---- start
        assureBufferSize(buffer.getNumSamples());
        
        // fill input buffers
        for (int i = 0; i < C74_GENPLUGIN::num_inputs(); i++) {
            if (i < totalNumInputChannels) {
                for (int j = 0; j < m_CurrentBufferSize; j++) {
                    m_InputBuffers[i][j] = buffer.getReadPointer(i)[j];
                }
            } else {
                memset(m_InputBuffers[i], 0, m_CurrentBufferSize *  sizeof(double));
            }
        }
        
        // process audio
        C74_GENPLUGIN::perform(m_C74PluginState,
                               m_InputBuffers,
                               C74_GENPLUGIN::num_inputs(),
                               m_OutputBuffers,
                               C74_GENPLUGIN::num_outputs(),
                               buffer.getNumSamples());
        
        // fill output buffers
        for (int i = 0; i < totalNumOutputChannels; i++) {
            if (i < C74_GENPLUGIN::num_outputs()) {
                for (int j = 0; j < buffer.getNumSamples(); j++) {
                    buffer.getWritePointer(i)[j] = m_OutputBuffers[i][j];
                }
            } else {
                buffer.clear (i, 0, buffer.getNumSamples());
            }
        }
    // C74 Process code ---- end
    
    
    // Apply the gain to the samples using a ramp to avoid discontinuities in
    // the audio between processed buffers.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        buffer.applyGainRamp (channel, 0, numSamples, previousGain, gain);

        meterListeners.call (&IAAEffectProcessor::MeterListener::handleNewMeterValue,
                             channel,
                             buffer.getMagnitude (channel, 0, numSamples));
    }

    previousGain = gain;

    // Now ask the host for the current time so we can store it to be displayed later.
    updateCurrentTimeInfoFromHost (lastPosInfo);
}

//==============================================================================
bool IAAEffectProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* IAAEffectProcessor::createEditor()
{
    return new IAAEffectEditor (*this, parameters);
}

//==============================================================================
void IAAEffectProcessor::getStateInformation (MemoryBlock& destData)
{
    auto xml = std::unique_ptr<XmlElement> (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void IAAEffectProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto xmlState = std::unique_ptr<XmlElement> (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.state = ValueTree::fromXml (*xmlState);
}

bool IAAEffectProcessor::updateCurrentTimeInfoFromHost (AudioPlayHead::CurrentPositionInfo &posInfo)
{
    if (AudioPlayHead* ph = getPlayHead())
    {
        AudioPlayHead::CurrentPositionInfo newTime;

        if (ph->getCurrentPosition (newTime))
        {
            posInfo = newTime;  // Successfully got the current time from the host.
            return true;
        }
    }

    // If the host fails to provide the current time, we'll just reset our copy to a default.
    lastPosInfo.resetToDefault();

    return false;
}

//==============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IAAEffectProcessor();
}


//==============================================================================
// C74 added methods

void IAAEffectProcessor::assureBufferSize(long bufferSize)
{
    if (bufferSize > m_CurrentBufferSize) {
        for (int i = 0; i < C74_GENPLUGIN::num_inputs(); i++) {
            if (m_InputBuffers[i]) delete m_InputBuffers[i];
            m_InputBuffers[i] = new t_sample[bufferSize];
        }
        for (int i = 0; i < C74_GENPLUGIN::num_outputs(); i++) {
            if (m_OutputBuffers[i]) delete m_OutputBuffers[i];
            m_OutputBuffers[i] = new t_sample[bufferSize];
        }
        
        m_CurrentBufferSize = bufferSize;
    }
}
