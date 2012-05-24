#ifndef _NNET_FEEDFORWARD_NETWORK_HPP_INCLUDED_
#define _NNET_FEEDFORWARD_NETWORK_HPP_INCLUDED_

#include <cstdlib>
#include <ctime>
#include <nnet/Activator.hpp>

namespace vision {

#define LEARNING_RATE 0.5

template<int INPUT_WIDTH, int HIDDEN_WIDTH, int OUTPUT_WIDTH,
		typename Number = float,
		template<typename T = Number> class Activator = Sigmoid>
class FeedforwardNetwork {
public:
	void initialize() {
		std::srand(std::time(NULL));

		for (int i = 0; i < HIDDEN_WIDTH; ++i) {
			for (int j = 0; j < INPUT_WIDTH + 1; ++j) {
				inputWeight[i][j] = getRandomFloat();
			}
		}
		for (int i = 0; i < OUTPUT_WIDTH; ++i) {
			for (int j = 0; j < HIDDEN_WIDTH + 1; ++j) {
				outputWeight[i][j] = getRandomFloat();
			}
		}
	}

	void execute(const Number* input, Number* output) const {
		Number hidden[HIDDEN_WIDTH];
		runWeightMatrix<HIDDEN_WIDTH, INPUT_WIDTH>(input, inputWeight,
				hidden);
		for (int i = 0; i < HIDDEN_WIDTH; ++i) {
			hidden[i] = Activator<>::activate(hidden[i]);
		}

		runWeightMatrix<OUTPUT_WIDTH, HIDDEN_WIDTH>(hidden, outputWeight,
				output);
		for (int i = 0; i < OUTPUT_WIDTH; ++i) {
			output[i] = Activator<>::activate(output[i]);
		}
	}

	Number ingest(const Number* input, const Number* expected) {
		Number hiddenInput[HIDDEN_WIDTH];
		runWeightMatrix<HIDDEN_WIDTH, INPUT_WIDTH>(input, inputWeight,
				hiddenInput);
		Number hiddenOutput[HIDDEN_WIDTH];
		for (int i = 0; i < HIDDEN_WIDTH; ++i) {
			hiddenOutput[i] = Activator<>::activate(hiddenInput[i]);
		}
		Number finalInput[OUTPUT_WIDTH];
		runWeightMatrix<OUTPUT_WIDTH, HIDDEN_WIDTH>(hiddenOutput,
				outputWeight, finalInput);
		Number finalOutput[OUTPUT_WIDTH];
		for (int i = 0; i < OUTPUT_WIDTH; ++i) {
			finalOutput[i] = Activator<>::activate(finalInput[i]);
		}

		Number outputError[OUTPUT_WIDTH];
		for (int i = 0; i < OUTPUT_WIDTH; ++i) {
			outputError[i] = (expected[i] - finalOutput[i])
					* Activator<>::derivative(finalInput[i]);
		}
		Number hiddenError[HIDDEN_WIDTH];
		for (int i = 0; i < HIDDEN_WIDTH; ++i) {
			hiddenError[i] = 0;
			for (int j = 0; j < OUTPUT_WIDTH; ++j) {
				hiddenError[i] += outputWeight[j][i] * outputError[j]
						* Activator<>::derivative(hiddenInput[i]);
			}
		}

		for (int i = 0; i < OUTPUT_WIDTH; ++i) {
			for (int j = 0; j < HIDDEN_WIDTH; ++j) {
				outputWeight[i][j] += outputError[i] * LEARNING_RATE
						* hiddenOutput[j];
			}
			outputWeight[i][HIDDEN_WIDTH] += LEARNING_RATE * outputError[i];
		}
		for (int i = 0; i < HIDDEN_WIDTH; ++i) {
			for (int j = 0; j < INPUT_WIDTH; ++j) {
				inputWeight[i][j] += hiddenError[i] * LEARNING_RATE
						* input[j];
			}
			inputWeight[i][INPUT_WIDTH] += LEARNING_RATE * hiddenError[i];
		}

		return getSquareSum<OUTPUT_WIDTH>(outputError);
	}

// TODO The data members are made public just to make the whole class a POD
//    class, although in C++11 this class is a POD class even when the data
//    members are private.
// private:
	Number inputWeight[HIDDEN_WIDTH][INPUT_WIDTH + 1];
	Number outputWeight[OUTPUT_WIDTH][HIDDEN_WIDTH + 1];

private:
	static Number getRandomFloat() {
		Number tmp = std::rand() / (Number)RAND_MAX;
		return (tmp - 0.5) * 2;
	}

	template<int LENGTH>
	static Number getSquareSum(const Number* vector) {
		Number sum = 0;
		for (int i = 0; i < LENGTH; ++i) {
			sum += vector[i] * vector[i];
		}
		return sum;
	}

	template<int RESULT_LENGTH, int INPUT_LENGTH>
	static inline void runWeightMatrix(const Number* input,
			const Number (*matrix)[INPUT_LENGTH + 1], Number* output) {
		for (int i = 0; i < RESULT_LENGTH; ++i) {
			Number tmp = 0;
			for (int j = 0; j < INPUT_LENGTH; ++j) {
				tmp += matrix[i][j] * input[j];
			}
			output[i] = tmp + matrix[i][INPUT_LENGTH];
		}
	}
};

#undef LEARNING_RATE

} // namespace vision

#endif // _NNET_FEEDFORWARD_NETWORK_HPP_INCLUDED_
