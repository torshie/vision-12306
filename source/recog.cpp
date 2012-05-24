#include <cstdio>
#include <captcha/Captcha.hpp>
#include <common/wrapper/wrapper.hpp>

using namespace vision;

int main(int argc, char** argv) {
	if (argc != 2) {
		std::fprintf(stderr, "Usage: %s <.ppm file>\n", argv[0]);
		return 1;
	}
	Captcha captcha(argv[1]);
	captcha.cleanUp();
	captcha.segment();

	for (int i = 0; i < 3; ++i) {
		captcha.print(stderr, i);
	}

	FILE* file = wrapper::fopen_("network.data", "rb");
	Captcha::NeuralNetwork network;
	wrapper::fread_(&network, 1, sizeof(network), file);
	wrapper::fclose_(file);
	char chars[6] = "    \n";
	captcha.readChars(&network, chars);
	wrapper::fwrite_(chars, 1, sizeof(chars) - 1, stdout);
	return 0;
}
