
namespace sky_drone {

	using windowingMethod = juce::dsp::WindowingFunction<float>::WindowingMethod;

	FFTProcessor::FFTProcessor() : fft(fftOrder), window(fftSize + 1, windowingMethod::hann, false) {
		
	}

	void FFTProcessor::prepare(float sr) {
		sampleRate = sr;
		reset();
	}
	
	int FFTProcessor::getLatencyInSamples() const {
		return 0;
	}


	float FFTProcessor::processSample(float& sample, bool bypassed) {
	
		inputFifo[writePosition] = sample;
		float outputSample = outputFifo[writePosition];
		outputFifo[writePosition] = 0.f;

		writePosition++;
		if (writePosition == fftSize) {
			writePosition = 0;
		}

		count++;
		if (count == hopSize) {
			count = 0;
			processFrame(bypassed);			
		}

		return outputSample;
	}

	void FFTProcessor::processFrame(bool bypassed) {
	
		// auto biggestBin = std::max_element(bins.begin(), bins.end(), [](auto a, auto b) { return a.magnitude < b.magnitude; });
		// DBG(biggestBin->frequency);
		const float* inputPtr = inputFifo.data();
		float* fftPtr = fftData.data();

		std::memcpy(fftPtr, inputPtr + writePosition, (fftSize - writePosition) * sizeof(float));
		if (writePosition > 0) {
			std::memcpy(fftPtr + fftSize - writePosition, inputPtr, writePosition * sizeof(float));
		}

		window.multiplyWithWindowingTable(fftPtr, fftSize);

		if (!bypassed) {
			fft.performRealOnlyForwardTransform(fftPtr, true);
			processSpectrum(fftPtr);
			fft.performRealOnlyInverseTransform(fftPtr);
		}

		window.multiplyWithWindowingTable(fftPtr, fftSize);

		// apply gain correction for windowing
		for (int i = 0; i < fftSize; i++) {
			fftPtr[i] *= windowGainCorrection; // why use a pointer here?
		}

		// write to outputFifo
		/*
		Do this in two parts as the current write index may be located anywhere in the circular buffer 
		Implemented with a simple for-loop,could be optimized using JUCE’s FloatVectorOperations class
		*/

		for (int i = 0; i < writePosition; i++) {
			outputFifo[i] += fftData[i + fftSize - writePosition];
		}
		for (int i = 0; i < fftSize - writePosition; i++) {
			outputFifo[i + writePosition] += fftData[i];
		}
	}

	void FFTProcessor::processSpectrum(float* data) {
		
		auto* cdata = reinterpret_cast<std::complex<float>*>(data);		

		for (int i = 0; i < numBins; ++i) {
			float magnitude = std::abs(cdata[i]);
			float phase = std::arg(cdata[i]);

			// This is where you'd do your spectral processing...			

			//-------------------------------------------------------------
			// https://stackoverflow.com/questions/4633203/extracting-precise-frequencies-from-fft-bins-using-phase-change-between-frames
			// https://blogs.zynaptiq.com/bernsee/pitch-shifting-using-the-ft/
			// https://blogs.zynaptiq.com/bernsee/repo/smbPitchShift.cpp
			/* compute phase difference */
			float phaseDiff = phase - lastPhase[i];
			lastPhase[i] = phase;

			/* subtract expected phase difference */
			// osamp is oversampling factor 
			/*
			2. osamp is the STFT
			* oversampling factor which also determines the overlap between adjacent STFT
			* frames. It should at least be 4 for moderate scaling ratios. A value of 32 is
			* recommended for best quality.
			*/
			float osamp = 4.f;
			float binPhaseOffset = juce::MathConstants<float>::twoPi * (float)i / osamp;
			float deltaPhase = phaseDiff - binPhaseOffset;

			/* map delta phase into [-Pi, Pi) interval */
			// better, but obfuscatory...
			//    deltaPhase -= M_TWOPI * floor(deltaPhase / M_TWOPI + .5);

			while (deltaPhase >= juce::MathConstants<float>::pi)
				deltaPhase -= juce::MathConstants<float>::twoPi;
			while (deltaPhase < -juce::MathConstants<float>::pi)
				deltaPhase += juce::MathConstants<float>::twoPi;

			// Get deviation from bin frequency from the +/- Pi interval 
			// Compute the k-th partials' true frequency    

			// Start with bin's ideal frequency
			float bin0Freq = sampleRate / fftSize;
			bins[i].idealFrequency = (float)i * bin0Freq;

			// Add deltaFreq
			float sampleTime = 1.f / sampleRate;
			float samplesInStep = fftSize / osamp;
			float stepTime = sampleTime * samplesInStep;
			float deltaTime = stepTime;

			// Definition of frequency is rate of change of phase, i.e. f = dϕ/dt
			// double deltaPhaseUnit = deltaPhase / M_TWOPI; // range [-.5, .5)
			float freqAdjust = (1.f / juce::MathConstants<float>::twoPi) * deltaPhase / deltaTime;
			
			bins[i].frequency = bins[i].idealFrequency + freqAdjust;
			bins[i].magnitude = magnitude;						
			//-------------------------------------------------------------



			cdata[i] = std::polar(magnitude, phase);
		}
	
	}

	void FFTProcessor::reset() {
		count = 0;
		writePosition = 0;
		std::fill(inputFifo.begin(), inputFifo.end(), 0.f);
		std::fill(outputFifo.begin(), outputFifo.end(), 0.f);
		std::fill(lastPhase.begin(), lastPhase.end(), 0.f);
	
	}
} // namespace sky_drone