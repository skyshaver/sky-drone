#pragma once

namespace sky_drone {

	// https://audiodev.blog/fft-processing/
	class FFTProcessor {

	public:
		FFTProcessor();
		void prepare(float sr);
		// use this to setLatencySamples in PluginProcessor::prepareToPlay
		int getLatencyInSamples() const;
		float processSample(float& sample, bool bypassed);
		void processFrame(bool bypassed);
		void processSpectrum(float* data);
		void reset();

	private:
		static constexpr int fftOrder = 12;
		static constexpr int fftSize = 1 << fftOrder;		// 1024 Samples	
		static constexpr int numBins = fftSize / 2 + 1;		// 513 Bins
		static constexpr int overlap = 4;					// 75% overlap
		static constexpr int hopSize = fftSize / overlap;	// 256 Samples
		static constexpr float windowGainCorrection = 2.f / 3.f; 
		float sampleRate;

		/*
		When using a Hann window twice, with 75% overlap, the resulting amplitude is 1.5× too large. 
		Recall that the average value of the Hann window is 0.5.
		Applying a window twice means we’re effectively multiplying by the square of the window, 
		and the squared Hann window has an average value of 0.375 or 3/8.
		Due to the overlap factor, the total gain is 4 × 3/8 = 1.5.
		To compensate we should divide by this value, which is equivalent to multiplying by 2/3. 
		For other windows and other overlap factors, this correction will be different.
		*/

		juce::dsp::FFT fft;
		juce::dsp::WindowingFunction<float> window;

		int count{};								// will count up to hopsize
		int writePosition{};
		std::array<float, fftSize> inputFifo;
		std::array<float, fftSize> outputFifo;
		std::array<float, fftSize * 2> fftData;
		std::array<float, numBins> lastPhase;

		struct bin {
			float idealFrequency;
			float frequency;
			float magnitude; 
		};
		std::array<bin, numBins> bins;

	};

} // namepace sky_drone