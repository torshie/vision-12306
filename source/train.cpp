#include <cstring>
#include <cstdio>
#include <list>
#include <utility>
#include <unistd.h>
#include <captcha/Captcha.hpp>
#include <common/wrapper/wrapper.hpp>

using namespace vision;

bool isValidNetworkData(const char* filename) {
	if (access(filename, F_OK | R_OK | W_OK) != 0) {
		return false;
	}
	struct stat s;
	stat(filename, &s);
	return s.st_size == sizeof(Captcha::NeuralNetwork);
}

int main(int argc, char** argv) {
	typedef std::list<std::pair<Captcha*, const char*> > CaptchaList;

	CaptchaList db;
	char name[] = "    .ppm";
	for (int i = 1; i < argc; ++i) {
		std::memcpy(name, argv[i], 4);
		Captcha* captcha = new Captcha(name);
		captcha->cleanUp();
		captcha->segment();
		db.push_back(std::make_pair(captcha, argv[i]));
	}

	Captcha::NeuralNetwork network;
	if (isValidNetworkData("network.data")) {
		FILE* file = wrapper::fopen_("network.data", "rb");
		wrapper::fread_(&network, 1, sizeof(network), file);
		wrapper::fclose_(file);
	} else {
		network.initialize();
	}

	for (int loop = 0; loop < 100; ++loop) {
		double sum = 0;
		for (CaptchaList::iterator i = db.begin(), e = db.end();
				i != e; ++i) {
			sum += i->first->trainNetwork(&network, i->second);
		}
		std::printf("Loop %d, error: %g\n", loop, sum);
	}

	FILE* file = wrapper::fopen_("network.data", "wb");
	wrapper::fwrite_(&network, 1, sizeof(network), file);
	wrapper::fclose_(file);
}
