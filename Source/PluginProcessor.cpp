/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Plugin3323AudioProcessor::Plugin3323AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
	gainVolume(-1.0f),
	treeState(*this, nullptr)
// Body du constructeur
#endif
{
	// Normalisation du gain
	float minGain = -48.0f;
	float maxGain = 0.0f;
	float defaultGain = /*mettre une valeur*/;
	NormalisableRange<float>gainRange(minGain, maxGain);
	treeState.createAndAddParameter(GAIN_ID, GAIN_NAME, GAIN_NAME, gainRange, 0.5f, nullptr, nullptr);
	// Normalisation des filtres | definit un valeur de depart et de fin
	float minFreq = 20.0f;
	float maxFreq = 20000.0f;
	NormalisableRange<float> cutoffRange(minFreq, maxFreq);
	NormalisableRange<float> resonanceRange(1.0f, 5.0f);
	NormalisableRange<float> menuItems(0, 2);
	// Creation de nouveaux element
	// ("parameterID", "parameterName", "label", cutoffRange, float value, default value, value to text)
	treeState.createAndAddParameter("cutoff", "Cutoff", "cutoff", cutoffRange, 600.0f, nullptr, nullptr);
	treeState.createAndAddParameter("resonance", "Resonance", "resonance", resonanceRange, 1.0f, nullptr, nullptr);
	treeState.createAndAddParameter("filterMenu", "FilterMenu", "filterMenu", menuItems, 0, nullptr, nullptr);
	// Sauvegarder les paramettres du plugin en XML, tjrs mettre ca a la fin
	treeState.state = ValueTree("sauvegardeSettings");
}

Plugin3323AudioProcessor::~Plugin3323AudioProcessor()
{
}

//==============================================================================
const String Plugin3323AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugin3323AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugin3323AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugin3323AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugin3323AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugin3323AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugin3323AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugin3323AudioProcessor::setCurrentProgram (int index)
{
}

const String Plugin3323AudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugin3323AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugin3323AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    
	lastSampleRate = sampleRate;
	dsp::ProcessSpec spec;
	spec.sampleRate = lastSampleRate;
	// maximum de samples qui vont etre dans le block envoyer a process()
	spec.maximumBlockSize = samplesPerBlock;
	// numChannels = le nombre de channel qui seront utilisé par process() - estimation
	// getMainBusNumOutputChannels() = numbre de output
	spec.numChannels = getMainBusNumOutputChannels();

	// on arrete -> on update -> on repart
	stateVariableFilter.reset();
	updateFilter();
	stateVariableFilter.prepare(spec);
}

void Plugin3323AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugin3323AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugin3323AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool Plugin3323AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugin3323AudioProcessor::createEditor()
{
    return new Plugin3323AudioProcessorEditor (*this);
}

//==============================================================================
void Plugin3323AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugin3323AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugin3323AudioProcessor();
}
