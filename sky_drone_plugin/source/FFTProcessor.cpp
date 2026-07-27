
namespace sky_drone {

	using windowingMethod = juce::dsp::WindowingFunction<float>::WindowingMethod;

	FFTProcessor::FFTProcessor() : fft(fftOrder), window(fftSize + 1, windowingMethod::hann, false) {
		
	}
	
	int FFTProcessor::getLatencyInSamples() const {
		return 0;
	}

	void FFTProcessor::reset() {
		count = 0;
		writePosition = 0;
		std::fill(inputFifo.begin(), inputFifo.end(), 0.f);
		std::fill(outputFifo.begin(), outputFifo.end(), 0.f);
	
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
	
		const float* inputPtr = inputFifo.data();
		float* fftPtr = fftData.data();

		std::memcpy(fftPtr, inputPtr + writePosition, (fftSize - writePosition) * sizeof(float));
		if (writePosition > 0) {
			std::memcpy(fftPtr + fftSize - writePosition, inputPtr, writePosition * sizeof(float));
		}

		window.multiplyWithWindowingTable(fftPtr, fftSize);

		if (!bypassed) {
			fft.performRealOnlyForwardTransform(fftPtr, true);
			// processSpectrum(fftPtr, numBins);
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

	void FFTProcessor::processSpectrum(float* data, int numBins) {
		
		auto* cdata = reinterpret_cast<std::complex<float>*>(data);

		for (int i = 0; i < numBins; ++i) {
			float magnitude = std::abs(cdata[i]);
			float phase = std::arg(cdata[i]);

			// This is where you'd do your spectral processing...

			cdata[i] = std::polar(magnitude, phase);
		}
	
	}

} // namespace sky_drone