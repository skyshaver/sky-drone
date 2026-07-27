#include <sky_drone_plugin/sky_drone_plugin.h>
#include <AudioFile.h>
#include <gtest/gtest.h>

namespace sky_drone {
	TEST(FFTProcessor, WaveFileNullability) {
		
		FFTProcessor fft;
		fft.reset();


		AudioFile<float> working;
		const float sampleRate = 48000.f;
		const float frequencyInHz = 440.f;
		working.setNumChannels(2);
		working.setNumSamplesPerChannel(int(sampleRate) * 3);

		for (int i = 0; i < working.getNumSamplesPerChannel(); i++)
		{
			for (int channel = 0; channel < working.getNumChannels(); channel++)
			{
				working.samples[channel][i] = std::sin((static_cast<float>(i) / sampleRate) * frequencyInHz * 2.f * juce::MathConstants<float>::pi);
			}
		}

		std::string originalPath = "original-sine-wave.wav"; 
		working.save(originalPath, AudioFileFormat::Wave);		

		// make sure the spectral processing is off, this test
		// just tests that the fft windowing isn't altering the workingal signal
		for (int i = 0; i < working.getNumSamplesPerChannel(); i++)
		{
			for (int channel = 0; channel < working.getNumChannels(); channel++)
			{
				 fft.processSample(working.samples[channel][i], false);
			}
		}

		std::string processedPath = "processed-sine-wave.wav";
		working.save(processedPath, AudioFileFormat::Wave);

		// invert the processed file
		for (int i = 0; i < working.getNumSamplesPerChannel(); i++)
		{
			for (int channel = 0; channel < working.getNumChannels(); channel++)
			{
				working.samples[channel][i] = working.samples[channel][i] * -1.f;
			}
		}

		std::string invertedPath = "inverted-processed-sine-wave.wav";
		working.save(invertedPath, AudioFileFormat::Wave);


		// sum original and inverted processed and test that values are within .01 of 0
		AudioFile<float> original;
		AudioFile<float> invertedProcessed;
		AudioFile<float> summed;
		
		bool ok = original.load(originalPath);
		EXPECT_EQ(true, ok);
		ok = invertedProcessed.load(invertedPath);
		EXPECT_EQ(true, ok);

		summed.setNumChannels(original.getNumChannels());
		summed.setNumSamplesPerChannel(original.getNumSamplesPerChannel());

		for (int i = 0; i < original.getNumSamplesPerChannel(); i++)
		{
			for (int channel = 0; channel < original.getNumChannels(); channel++)
			{
				auto summedSample = original.samples[channel][i] + invertedProcessed.samples[channel][i];
				
				EXPECT_NEAR(summedSample, 0.f, 0.001f);

				summed.samples[channel][i] = summedSample;
			}
		}
		std::string summedPath = "summed-inv-og.wave";
		summed.save(summedPath);
		
	}

} // namespace sky_drone

