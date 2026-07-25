#pragma once

namespace sky_drone {
	class PluginEditor : public juce::AudioProcessorEditor {
	public:
		explicit PluginEditor(PluginProcessor&);

		void resized() override;
		void paint(juce::Graphics&) override;

	private:

		// PluginProcessor& pluginProcessor;
		// void sliderValueChanged(juce::Slider* slider) override;
		juce::Slider parameterOneSlider;
		juce::SliderParameterAttachment parameterOneAttachment;

		juce::Slider parameterTwoSlider;
		juce::SliderParameterAttachment parameterTwoAttachment;

		juce::ImageComponent background;
		juce::ImageComponent logo;	

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
	};
}  // namespace sky_drone
