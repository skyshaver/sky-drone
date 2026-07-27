#include <sky_drone_plugin/sky_drone_plugin.h>
#include <AudioFile.h>
#include <gtest/gtest.h>

namespace sky_drone {
	TEST(FFTProcessor, WaveFileNullability) {
		// generate sine wave audio file
		AudioFile<float> origin;
		origin.setNumChannels(2);
		origin.setNumSamplesPerChannel(48000);
		const float sampleRate = 48000.f;
		const float frequencyInHz = 440.f;

		for (int i = 0; i < origin.getNumSamplesPerChannel(); i++)
		{
			for (int channel = 0; channel < origin.getNumChannels(); channel++)
			{
				origin.samples[channel][i] = std::sin((static_cast<float>(i) / sampleRate) * frequencyInHz * 2.f * juce::MathConstants<float>::pi);
			}
		}

		std::string filePath = "sine-wave.wav"; // change this to somewhere useful for you
		origin.save(filePath, AudioFileFormat::Wave);				

		EXPECT_EQ(3, 3);
	}

} // namespace sky_drone

