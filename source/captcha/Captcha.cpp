#include <cstring>
#include <cstdio>
#include <captcha/Captcha.hpp>
#include <common/except/InvalidArgument.hpp>
#include <common/wrapper/wrapper.hpp>

using namespace vision;

Captcha::Captcha(const char* filename) {
	FILE* file = wrapper::fopen_(filename, "rb");
	static const char IMAGE_HEADER[] = "P6\n60 20\n255\n";
	char buffer[16];
	wrapper::fread_(buffer, 1, sizeof(IMAGE_HEADER) - 1, file);
	if (std::memcmp(buffer, IMAGE_HEADER, sizeof(IMAGE_HEADER) - 1) != 0
			|| wrapper::fread_(image, 1, sizeof(image), file)
					!= sizeof(image)) {
		wrapper::fclose_(file);
		RAISE(InvalidArgument, "Invalid PPM header %s", filename);
	}
	wrapper::fclose_(file);
	std::memset(splitter, 0, sizeof(splitter));
}

void Captcha::cleanUp() {
	for (unsigned i = 0; i < sizeof(image); ++i) {
		if (image[i] >= WHITE_THRESHOLD) {
			image[i] = WHITE_BYTE;
		}
	}

	eraseNoise();
}

void Captcha::segment() {
	// Ignore the first line, so loop starts with 1, not 0
	for (int i = 1; i < IMAGE_WIDTH; ++i) {
		if (isWhiteLine(i, 0) + isWhiteLine(i, 1) + isWhiteLine(i, 2) >= 2) {
			continue;
		}
		splitter[0] = i;
		break;
	}
	for (int i = IMAGE_WIDTH - 1; i >= 0; --i) {
		if (isWhiteLine(i, 0) + isWhiteLine(i, 1) + isWhiteLine(i, 2) >= 2) {
			continue;
		}
		splitter[4] = i + 1;
		break;
	}
	double step = ((double)splitter[4] - splitter[0]) / 4;
	for (int i = 1; i < 4; ++i) {
		splitter[i] = (int)(step * i + splitter[0] + 0.5);
	}
}

void Captcha::print(FILE* file, int channel) const {
	static const char BLACK[] = "\x1b[37;40m \x1b[0m";
	static const char RED[] = "\x1b[37;41m \x1b[0m";
	static const char CROSS[] = "\x1b[37;41mX\x1b[0m";
	for (int i = 0; i < IMAGE_HEIGHT; ++i) {
		for (int j = 0; j < IMAGE_WIDTH; ++j) {
			bool red = false;
			if (j == splitter[0] || j == splitter[1] || j == splitter[2]
					|| j == splitter[3] || j == splitter[4]) {
				red = true;
			}
			bool white = (getValue(j, i, channel) == WHITE_BYTE);
			if (white) {
				if (red) {
					wrapper::fwrite_(RED, 1, sizeof(RED) - 1, file);
				} else {
					std::fputc(' ', file);
				}
			} else {
				if (red) {
					wrapper::fwrite_(CROSS, 1, sizeof(CROSS) - 1, file);
				} else {
					wrapper::fwrite_(BLACK, 1, sizeof(BLACK) - 1, file);
				}
			}
		}
		std::fputc('\n', file);
	}
	wrapper::fflush_(file);
}

double Captcha::trainNetwork(NeuralNetwork* network, const char* chars) const {
	double input[CHAR_WIDTH * IMAGE_HEIGHT];
	double expected[OUTPUT_WIDTH];
	double sum = 0;

	for (int i = 0; i < 4; ++i) {
		buildNetworkOutput(chars[i], expected);
		for (int channel = 0; channel < 3; ++channel) {
			buildNetworkInput(i, channel, input);
			sum += network->ingest(input, expected);
		}
	}
	return sum;
}

void Captcha::buildNetworkOutput(char ch, double* array) {
	std::memset(array, 0, sizeof(*array) * OUTPUT_WIDTH);
	if (ch >= '0' && ch <= '9') {
		array[ch - '0'] = 1;
	} else {
		array[ch - 'A' + 10] = 1;
	}
}

void Captcha::eraseNoise() {
	for (int i = 1; i < IMAGE_HEIGHT - 1; ++i) {
		for (int j = 1; j < IMAGE_WIDTH - 1; ++j) {
			for (int channel = 0; channel < 3; ++channel) {
				if (getValue(j, i, channel) == WHITE_BYTE) {
					continue;
				}
				if (getValue(j - 1, i - 1, channel) == WHITE_BYTE
						&& getValue(j - 1, i, channel) == WHITE_BYTE
						&& getValue(j - 1, i + 1, channel) == WHITE_BYTE
						&& getValue(j, i - 1, channel) == WHITE_BYTE
						&& getValue(j, i + 1, channel) == WHITE_BYTE
						&& getValue(j + 1, i - 1, channel) == WHITE_BYTE
						&& getValue(j + 1, i, channel) == WHITE_BYTE
						&& getValue(j + 1, i + 1, channel) == WHITE_BYTE) {
					image[3 * (i * IMAGE_WIDTH + j) + channel] = WHITE_BYTE;
				}
			}
		}
	}
}

int Captcha::isWhiteLine(int line, int channel) const {
	for (int i = 0; i < IMAGE_HEIGHT; ++i) {
		if (getValue(line, i, channel) != WHITE_BYTE) {
			return 0;
		}
	}
	return 1;
}

void Captcha::buildNetworkInput(int index, int channel, double* data) const {
	int begin = splitter[index];
	int end = splitter[index + 1];

	std::memset(data, 0, IMAGE_HEIGHT * CHAR_WIDTH * sizeof(*data));
	for (int i = 0; i < IMAGE_HEIGHT; ++i) {
		int offset = i * CHAR_WIDTH;
		for (int j = begin; j < end; ++j, ++offset) {
			if (getValue(j, i, channel) == WHITE_BYTE) {
				data[offset] = 0;
			} else {
				data[offset] = 1;
			}
		}
	}
}

char Captcha::readSingleChar(const NeuralNetwork* network, int index) const {
	char ch[3];
	for (int i = 0; i < 3; ++i) {
		ch[i] = readChannel(network, index, i);
	}
	if (ch[0] == ch[1] && ch[1] == ch[2]) {
		return ch[0];
	} else if (ch[0] != ch[1] && ch[1] != ch[2] && ch[0] != ch[2]) {
		return '?';
	}
	if (ch[0] == ch[1]) {
		return ch[0];
	} else if (ch[0] == ch[2]) {
		return ch[0];
	} else { // ch[1] == ch[2] guaranteed here.
		return ch[1];
	}
}

char Captcha::readChannel(const NeuralNetwork* network, int index,
		int channel) const {
	double input[CHAR_WIDTH * IMAGE_HEIGHT];
	double output[OUTPUT_WIDTH];

	buildNetworkInput(index, channel, input);
	network->execute(input, output);

	int max = 0;
	for (int i = 1; i < OUTPUT_WIDTH; ++i) {
		if (output[i] > output[max]) {
			max = i;
		}
	}
	return "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[max];
}
