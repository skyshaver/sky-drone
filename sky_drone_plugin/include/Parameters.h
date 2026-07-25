#pragma once

namespace sky_drone {
	struct Parameters {
		explicit Parameters(juce::AudioProcessor&);

		juce::AudioParameterFloat& parameterOne;
		
		juce::AudioParameterFloat& parameterTwo;
		
		// juce::AudioParameterBool& bypassed;
		// juce::AudioParameterChoice& waveform;

		JUCE_DECLARE_NON_COPYABLE(Parameters)
		JUCE_DECLARE_NON_MOVEABLE(Parameters)
	};
}  // namespace tremolo
