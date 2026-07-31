#pragma once

namespace sky_drone {
	class PluginProcessor : public juce::AudioProcessor {
	public:
		PluginProcessor();

		void prepareToPlay(double sampleRate, int expectedMaxFramesPerBlock) override;

		void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
		using AudioProcessor::processBlock;

		void releaseResources() override;

		juce::AudioProcessorEditor* createEditor() override;
		bool hasEditor() const override;

		const juce::String getName() const override;

		bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

		bool acceptsMidi() const override;
		bool producesMidi() const override;
		bool isMidiEffect() const override;
		double getTailLengthSeconds() const override;

		int getNumPrograms() override;
		int getCurrentProgram() override;
		void setCurrentProgram(int index) override;
		const juce::String getProgramName(int index) override;
		void changeProgramName(int index, const juce::String& newName) override;

		[[nodiscard]] Parameters& getParameterRefs() noexcept;


		void getStateInformation(juce::MemoryBlock& destData) override;
		void setStateInformation(const void* data, int sizeInBytes) override;		

	private:
		

		std::array<FFTProcessor, 2> ffts;
		Parameters parameters{ *this };

		juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lpFilters;
	

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
	};
}  // namespace sky_drone
