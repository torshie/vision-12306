#ifndef _CAPTCHA_CAPTCHA_HPP_INCLUDED_
#define _CAPTCHA_CAPTCHA_HPP_INCLUDED_

#include <nnet/FeedforwardNetwork.hpp>

namespace vision {

class Captcha {
public:
	enum {
		IMAGE_WIDTH = 60, IMAGE_HEIGHT = 20, CHAR_WIDTH = 15,
		OUTPUT_WIDTH = 36, PIXEL_COUNT = IMAGE_WIDTH * IMAGE_HEIGHT
	};

	// TODO: Is this network way too large ?
	typedef FeedforwardNetwork<CHAR_WIDTH * IMAGE_HEIGHT, 100, OUTPUT_WIDTH,
			double> NeuralNetwork;

	explicit Captcha(const char* filename);

	void readChars(const NeuralNetwork* network, char* chars) const {
		for (int i = 0; i < 4; ++i) {
			chars[i] = readSingleChar(network, i);
		}
	}

	void cleanUp();
	void segment();
	void print(FILE* file, int channel) const;
	double trainNetwork(NeuralNetwork* network, const char* chars) const;

private:
	enum {
		WHITE_BYTE = 0xff, WHITE_THRESHOLD = 0xb0
	};

	unsigned char image[PIXEL_COUNT * 3];
	int splitter[5];

	static void buildNetworkOutput(char ch, double* array);

	inline unsigned char getValue(int offX, int offY, int channel) const {
		return image[3 * (offY * IMAGE_WIDTH + offX) + channel];
	}

	void eraseNoise();
	int isWhiteLine(int line, int channel) const;
	void buildNetworkInput(int index, int channel, double* data) const;
	char readSingleChar(const NeuralNetwork* network, int index) const;
	char readChannel(const NeuralNetwork* network, int index, int channel) const;
};

} // namespace captcha

#endif // _CAPTCHA_CAPTCHA_HPP_INCLUDED_
