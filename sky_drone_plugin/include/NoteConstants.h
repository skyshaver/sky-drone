namespace sky_drone {

	struct NoteInfo {
		std::string_view name;
		const float frequency;
		const int midiNote;
	};

	// https://en.wikipedia.org/wiki/Piano_key_frequencies
	// https://studiocode.dev/resources/midi-middle-c/
	// Eo3 indicates natural, keeping 3 char strings is easier for parsing stuff
	// string_view allows constexpr, the extra {} in to_array is needed for std::array construction
	// this allows us to create constexpr arrays without having to specify size in the init args

	constexpr auto guitarNotes = std::to_array<NoteInfo> ({
		{ "Co2", 65.41f, 36 },
		{ "C#2", 69.30f, 37 },
		{ "Do2", 73.42f, 38 },
		{ "D#2", 77.78f, 39 },
		{ "Eo2", 82.41f, 40 },
		{ "Fo2", 87.31f, 41 },
		{ "F#2", 92.50f, 42 },
		{ "Go2", 98.00f, 43 },
		{ "G#2", 103.83f, 44 },
		{ "Ao2", 110.00f, 45 },
		{ "A#2", 116.54f, 46 },
		{ "Bo2", 123.47f, 47 },
		{ "Co3", 130.81f, 48 },
		{ "C#3", 138.59f, 49 },
		{ "Do3", 146.83f, 50 },
		{ "D#3", 155.56f, 51 },
		{ "Eo3", 164.81f, 52 },
		{ "Fo3", 174.61f, 53 },
		{ "F#3", 185.00f, 54 },
		{ "Go3", 196.00f, 55 },
		{ "G#3", 207.65f, 56 },
		{ "Ao3", 220.00f, 57 },
		{ "A#3", 233.08f, 58 },
		{ "Bo3", 246.94f, 59 },
		{ "Co4", 261.63f, 60 },
	});			

	constexpr auto pianoNotes = std::to_array<NoteInfo>({
		{ "CoN", 8.18f, 0 },
		{ "C#N", 8.66f, 1 },
		{ "DoN", 9.18f, 2 },
		{ "D#N", 9.72f, 3 },
		{ "EoN", 10.30f, 4 },
		{ "FoN", 10.91f, 5 },
		{ "F#N", 11.56f, 6 },
		{ "GoN", 12.25f, 7 },
		{ "G#N", 12.98f, 8 },
		{ "AoN", 13.75f, 9 },
		{ "A#N", 14.57f, 10 },
		{ "BoN", 15.43f, 11 },
		{ "0oN", 16.35f, 12 },
		{ "C#N", 17.32f, 13 },
		{ "DoN", 18.35f, 14 },
		{ "D#N", 19.45f, 15 },
		{ "EoN", 20.60f, 16 },
		{ "FoN", 21.83f, 17 },
		{ "F#N", 23.12f, 18 },
		{ "GoN", 24.50f, 19 },
		{ "G#N", 25.96f, 20 },
		{ "Ao0", 27.50f, 21 },
		{ "A#0", 29.14f, 22 },
		{ "Bo0", 30.87f, 23 },
		{ "Co1", 32.70f, 24 },
		{ "C#1", 34.65f, 25 },
		{ "Do1", 36.71f, 26 },
		{ "D#1", 38.89f, 27 },
		{ "Eo1", 41.20f, 28 },
		{ "Fo1", 43.65f, 29 },
		{ "F#1", 46.25f, 30 },
		{ "Go1", 49.00f, 31 },
		{ "G#1", 51.91f, 32 },
		{ "Ao1", 55.00f, 33 },
		{ "A#1", 58.27f, 34 },
		{ "Bo1", 61.74f, 35 },
		{ "Co2", 65.41f, 36 },
		{ "C#2", 69.30f, 37 },
		{ "Do2", 73.42f, 38 },
		{ "D#2", 77.78f, 39 },
		{ "Eo2", 82.41f, 40 },
		{ "Fo2", 87.31f, 41 },
		{ "F#2", 92.50f, 42 },
		{ "Go2", 98.00f, 43 },
		{ "G#2", 103.83f, 44 },
		{ "Ao2", 110.00f, 45 },
		{ "A#2", 116.54f, 46 },
		{ "Bo2", 123.47f, 47 },
		{ "Co3", 130.81f, 48 },
		{ "C#3", 138.59f, 49 },
		{ "Do3", 146.83f, 50 },
		{ "D#3", 155.56f, 51 },
		{ "Eo3", 164.81f, 52 },
		{ "Fo3", 174.61f, 53 },
		{ "F#3", 185.00f, 54 },
		{ "Go3", 196.00f, 55 },
		{ "G#3", 207.65f, 56 },
		{ "Ao3", 220.00f, 57 },
		{ "A#3", 233.08f, 58 },
		{ "Bo3", 246.94f, 59 },
		{ "Co4", 261.63f, 60 },
		{ "C#4", 277.18f, 61 },
		{ "Do4", 293.66f, 62 },
		{ "D#4", 311.13f, 63 },
		{ "Eo4", 329.63f, 64 },
		{ "Fo4", 349.23f, 65 },
		{ "F#4", 369.99f, 66 },
		{ "Go4", 392.00f, 67 },
		{ "G#4", 415.30f, 68 },
		{ "Ao4", 440.00f, 69 },
		{ "A#4", 466.16f, 70 },
		{ "Bo4", 493.88f, 71 },
		{ "Co5", 523.25f, 72 },
		{ "C#5", 554.37f, 73 },
		{ "Do5", 587.33f, 74 },
		{ "D#5", 622.25f, 75 },
		{ "Eo5", 659.26f, 76 },
		{ "Fo5", 698.46f, 77 },
		{ "F#5", 739.99f, 78 },
		{ "Go5", 783.99f, 79 },
		{ "G#5", 830.61f, 80 },
		{ "Ao5", 880.00f, 81 },
		{ "A#5", 932.33f, 82 },
		{ "Bo5", 987.77f, 83 },
		{ "Co6", 1046.5f, 84 },
		{ "C#6", 1108.7f, 85 },
		{ "Do6", 1174.6f, 86 },
		{ "D#6", 1244.5f, 87 },
		{ "Eo6", 1318.5f, 88 },
		{ "Fo6", 1396.9f, 89 },
		{ "F#6", 1479.9f, 90 },
		{ "Go6", 1567.9f, 91 },
		{ "G#6", 1661.2f, 92 },
		{ "Ao6", 1760.0f, 93 },
		{ "A#6", 1864.6f, 94 },
		{ "Bo6", 1975.5f, 95 },
		{ "Co7", 2093.0f, 96 },
		{ "C#7", 2217.4f, 97 },
		{ "Do7", 2349.3f, 98 },
		{ "D#7", 2489.0f, 99 },
		{ "Eo7", 2637.0f, 100 },
		{ "Fo7", 2793.8f, 101 },
		{ "F#7", 2959.9f, 102 },
		{ "Go7", 3135.9f, 103 },
		{ "G#7", 3322.4f, 104 },
		{ "Ao7", 3520.0f, 105 },
		{ "A#7", 3729.3f, 106 },
		{ "Bo7", 3951.0f, 107 },
		{ "Co8", 4186.0f, 108 },
	});


	/*
	I'd like this to return a reference into the noteInfo range but can't figure out how to handle
	the array iterators properly, tried std::span but I may be missing something
	could do it all with pointers but yeah, at least it won't create templates
	On MSVC we have to use .data() to get underlying pointers, passing .end() and .begin to the ptr version works on clang
	can also be const auto* (without the * it returns the value)
	const NoteInfo* closestPtr = findClosestNoteInfoPtr(83.41f, guitarNotes.data(), guitarNotes.data() + std::distance(guitarNotes.begin(), guitarNotes.end()));
	godbolt for further experimentation https://godbolt.org/z/dG7j9dE9E
	*/
	template<class InputIterator>
	static NoteInfo findClosestNoteInfo(float frequency, InputIterator begin, InputIterator end) {
		auto lowerBound = std::lower_bound(begin, end, std::floorf(frequency), [](auto& info, float value) { return value > std::floorf(info.frequency); });
		if (lowerBound == begin)
			return *begin;
		if (lowerBound == end)
			return *(end - 1);

		auto prev = std::prev(lowerBound);
		auto prevDelta = frequency - prev->frequency;
		auto lbDelta = lowerBound->frequency - frequency;

		if (lbDelta < prevDelta)
			return *lowerBound;
		else
			return *prev;
		
	}

	const NoteInfo* findClosestNoteInfoPtr(float frequency, const NoteInfo* begin, const NoteInfo* end) {
		auto lowerBound = std::lower_bound(begin, end, std::floorf(frequency), [](auto& info, float value) { return value > std::floorf(info.frequency); });
		if (lowerBound == begin)
			return begin;
		if (lowerBound == end)
			return end - 1;

		auto prev = std::prev(lowerBound);
		auto prevDelta = frequency - prev->frequency;
		auto lbDelta = lowerBound->frequency - frequency;

		if (lbDelta < prevDelta)
			return lowerBound;
		else
			return prev;
	}
}		


	
	
	
	
	
	
	
	
	
	
	
	
	